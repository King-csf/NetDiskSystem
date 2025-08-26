#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListWidget)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
}

ListWidget::~ListWidget()
{
    delete ui;
}

void ListWidget::initTableView()
{
    //更新treeView的当前路径，用于下载
    ui->treeView->curDirPath = this->curFilePath;

    // 创建模型
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"文件名", "大小", "修改时间"});

    ui->treeView->setModel(model);  // 用 QTreeView
    ui->treeView->setRootIsDecorated(false); // 去掉树形展开箭头
    ui->treeView->setItemsExpandable(false);
    ui->treeView->setAllColumnsShowFocus(true);

    ui->treeView->setFont(QFont("Microsoft YaHei", 14)); // 字体名与字号

    // 设置列宽模式
    //ui->treeView->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    //无法编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 点击后文字不会被省略/消失
    ui->treeView->setTextElideMode(Qt::ElideNone);
    ui->treeView->setSelectionMode(QAbstractItemView::NoSelection);

    // 隐藏网格线
    ui->treeView->setStyleSheet(
        "QTreeView::item { border: none; }"
        "QTreeView::branch { border: none; }"
        );

    // 填充数据
    for(auto it = curFileList.begin(); it!= curFileList.end();it++)
    {
        QList<QStandardItem*> row;
        row << new QStandardItem(QIcon(":/new/prefix1/image/文件.png"),it->fileName)
            << new QStandardItem(it->fileSize)
            << new QStandardItem(it->lastModTime)
            ;
        model->appendRow(row);
    }

    connect(ui->treeView,&MyTreeView::haveAddDownloadFile,this,[this](QString fileName){
        emit haveAddDownloadFile(fileName);
    });
    //提交进度
    connect(ui->treeView,&MyTreeView::haveWriteSig,this,[this]
            (const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite){
                emit haveWriteSig(filename, chunkId, chunkSize, haveWrite);
            });
    //删除文件
    connect(ui->treeView,&MyTreeView::haveDelFile,this,[this]
            ( QString filename){
        for(auto it = curFileList.begin();it != curFileList.end();it++)
        {
            if(it->fileName == filename)
            {
                curFileList.erase(it);
                break;
            }
        }
        reSetFileList();
            });
}

void ListWidget::parsePageFileName(QString str)
{

    qDebug() << "JSON: "<<str;
    if(str == "NOT DIR"){
        curFilePath.pop_back();
        this->filetrans->curFilePath.pop_back();
        return ;
    }
    curFileList.clear();
    //解析json格式
    json j = json::parse(str.toStdString());

    for (const auto &item : j)
    {

        FileInfo f;
        f.fileName = QString::fromStdString(item["filename"].get<std::string>());
        f.fileSize = QString::fromStdString(item["size"].get<std::string>());
        f.lastModTime = QString::fromStdString(item["time"].get<std::string>());

        curFileList.push_back(f);

    }

}

void ListWidget::sendCreateDirReq(QString dirName)
{
    QNetworkRequest request(QUrl("http://192.168.111.200:8888/createDir"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["dirName"] = getCurInDir(curFilePath) + dirName;
    QByteArray jsonData = QJsonDocument(json).toJson();

    QNetworkReply *reply = manager->post(request, jsonData);
    qDebug() << "jsonData : " << jsonData;

    connect(reply, &QNetworkReply::finished, this, [this,dirName](){
        //qDebug() << "回复成功";
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray response = reply->readAll();
            qDebug() << "Response:" << response;
            //创建成功
            if(response.toStdString() != "createDir failed")
            {
                flushListView(dirName,QString::fromStdString(response.toStdString()));
            }
            //失败
            else
            {
                QMessageBox::critical(
                    this,                      // 父窗口指针（可为 nullptr）
                    "错误",                    // 标题
                    "文件夹创建失败"            // 内容
                    );
                return;

            }

        }
        else
        {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });

}

void ListWidget::flushListView(QString fileName, QString lastTime)
{

    FileInfo f;
    f.fileName = fileName;
    f.fileSize = "-";
    f.lastModTime = lastTime;

    curFileList.push_back(f);
    QList<QStandardItem*> row;
    row << new QStandardItem(QIcon(":/new/prefix1/image/文件.png"),f.fileName)
        << new QStandardItem(f.fileSize)
        << new QStandardItem(f.lastModTime);
    model->appendRow(row);
}

QString ListWidget::getCurInDir(QVector<QString> curFilePath)
{

    QString path;
    for(auto dir : curFilePath) {
        path += dir + "/";
    }
    return path;
}

void ListWidget::getCurFileList()
{

    QString path = getCurInDir(curFilePath);
    qDebug() << path;
    //去除"/"
    path.erase(path.end()-1);
    curPath = path;

    QUrl u("http://192.168.111.200:8888/getFileList/" + path);
    QNetworkRequest req(u);
    auto reply = manager->get(req);

    connect(reply, &QNetworkReply::finished, this, [this](){
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        QString content = QString::fromStdString(reply->readAll().toStdString());
        if(content == "NO DIR")
            return;
        parsePageFileName(content);
        emit getCurFileListFinish(this->curPath);
    });

    connect(this,&ListWidget::getCurFileListFinish,this,[this](QString path){
        reSetFileList();
    });
}

void ListWidget::reSetFileList()
{
    model->clear();
    model->setHorizontalHeaderLabels({"文件名", "大小", "修改时间"});
    // 填充数据
    for(auto it = curFileList.begin(); it!= curFileList.end();it++)
    {
        QList<QStandardItem*> row;
        row << new QStandardItem(QIcon(":/new/prefix1/image/文件.png"),it->fileName)
            << new QStandardItem(it->fileSize)
            << new QStandardItem(it->lastModTime)
            ;
        model->appendRow(row);
    }

}

void ListWidget::CreateDir()
{

    CreateDirDialog  createDirDlg ;
    if(createDirDlg.exec() == QDialog::Accepted)
    {
        QString dirName = createDirDlg.getEditLineText();
        for(auto it : curFileList)
        {
            //文件夹已经存在
            if(it.fileName == dirName)
            {
                QMessageBox::critical(
                    this,                      // 父窗口指针（可为 nullptr）
                    "错误",                    // 标题
                    "文件夹已经存在"  // 内容
                    );
                return;
            }
        }

        sendCreateDirReq(dirName);
    }
}

void ListWidget::addCurFilePath(QString dir)
{
    this->curFilePath.push_back(dir);
    ui->treeView->curDirPath.push_back(dir);
}


void ListWidget::on_treeView_doubleClicked(const QModelIndex &index)
{
    QModelIndex fileIndex = ui->treeView->model()->index(index.row(), 0);
    auto fileName = fileIndex.data();
    qDebug() << fileName;
    std::string stdFileName = fileName.toString().toStdString();
    auto pos = stdFileName.find_last_of(".") + 1;
    //视频播放
    if(stdFileName.substr(pos) == "mp4" || stdFileName.substr(pos) == "flv" || stdFileName.substr(pos) == "mp3")
    {
        QString filePath = getCurInDir(curFilePath) + QString::fromStdString(stdFileName);
        //qDebug() << "[DEBUG]传入参数：" <<
        Player * player = new Player(filePath);
        player->setAttribute(Qt::WA_DeleteOnClose);
        player->show();
        player->start();
    }
    else
    {
        //更新当前路径
        this->curFilePath.push_back(fileName.toString());
        this->filetrans->curFilePath.push_back(fileName.toString());
        ui->treeView->curDirPath.push_back(fileName.toString());

        getCurFileList();

    }
}


void ListWidget::on_toolButton_clicked()
{
    if(curFilePath.size() == 1)
    {
        return;
    }
    this->filetrans->curFilePath.pop_back();
    curFilePath.pop_back();
    ui->treeView->curDirPath.pop_back();

    QString path = getCurInDir(curFilePath);
    //去除"/"
    path.erase(path.end()-1);
    curPath = path;

    QUrl u("http://192.168.111.200:8888/getFileList/" + path);
    QNetworkRequest req(u);
    auto reply = manager->get(req);

    connect(reply, &QNetworkReply::finished, this, [this](){
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        QString content = QString::fromStdString(reply->readAll().toStdString());
        if(content == "NO DIR")
            return;
        parsePageFileName(content);
        emit getCurFileListFinish(this->curPath);
    });

    connect(this,&ListWidget::getCurFileListFinish,this,[this](QString path){
        reSetFileList();
    });
}




