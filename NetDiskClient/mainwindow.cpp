#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filetrans.h"
#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>

QString mainwindow::userRootFilePath = "";
QString mainwindow::account = "";

mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainwindow)
{

}



mainwindow::mainwindow(QString userRootFilePath,QString account,QWidget *parent )
    : QWidget(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);

    filetrans = new FileTrans(THREAD_NUM);
    manager = new QNetworkAccessManager(this);

    transferWidget = new TransferWidget(this);
    listWidget = new ListWidget(this);
    downloadWidget =  new TransferWidget(this);

    ui->stackedWidget->removeWidget(ui->page);
    ui->stackedWidget->removeWidget(ui->page_2);
    ui->stackedWidget->addWidget(listWidget);
    ui->stackedWidget->addWidget(transferWidget);
    ui->stackedWidget->addWidget(downloadWidget);
    ui->stackedWidget->setCurrentIndex(2);
    ui->listWidget->setCurrentRow(0);

    //有文件下载时，添加进度
    connect(this->listWidget,&ListWidget::haveAddDownloadFile,this,[this](QString fileName){
        this->downloadWidget->addDowloadFile(fileName);

    });
    //提交进度
    connect(this->listWidget,&ListWidget::haveWriteSig,this,[this]
            (const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite){
                this->downloadWidget->updateProgress(filename, chunkId, chunkSize, haveWrite);
            });
    // //删除文件
    // connect(this->listWidget,&ListWidget::haveDelFile,this,[this]
    //         (QString filename){

    //         });

    this->userRootFilePath = account;
    filetrans->curFilePath.push_back(account);

    listWidget->userRootFilePath = account;
    listWidget->addCurFilePath(account);
    listWidget->filetrans = this->filetrans;

    listWidget->parsePageFileName(userRootFilePath);
    listWidget->initTableView();

    // parsePageFileName(userRootFilePath);
    // //curFileList = firstFileList;
    // initTableView();
    auto threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(THREAD_NUM);
    //获取上次未传输完的文件
    getLastUnfinishFile();
}

mainwindow::~mainwindow()
{
    isExit = true;
    qDebug() << "[DEBUG]运行析构函数";

    recordUnfinish();
    
    // 清理所有活跃的传输对象
    for(FileTrans* trans : activeTransfers) {
        if(trans) {
            trans->deleteLater();
        }
    }
    activeTransfers.clear();
}



QString mainwindow::getCurInDir(QVector<QString> curFilePath)
{
    QString path;
    for(auto dir : curFilePath) {
        path += dir + "/";
    }
    return path;
}


QString mainwindow::getUserRootFilePath()
{
    return userRootFilePath;
}



void mainwindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    if(currentText == "我的文件")
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(currentText == "上传")
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(currentText == "下载")
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
}



// 上传文件夹的主要方法
void mainwindow::uploadFolder(const QString& folderPath)
{
    QDir dir(folderPath);
    if (!dir.exists()) {
        QMessageBox::warning(this, "错误", "选择的文件夹不存在");
        return;
    }
    
    // 检查文件夹是否为空
    if (dir.isEmpty()) {
        QMessageBox::information(this, "提示", "选择的文件夹为空");
        return;
    }
    
    // 获取文件夹名称作为相对路径的起始
    QString folderName = QFileInfo(folderPath).fileName();
    
    // 显示开始上传的消息
    QMessageBox::information(this, "开始上传", QString("开始上传文件夹: %1").arg(folderName));
    
    // 初始化文件夹上传状态
    folderUploadStatus[folderName] = 0;
    
    // 开始递归上传文件夹中的所有文件
    uploadFilesInFolder(folderPath, folderName);
}

// 递归遍历文件夹并上传所有文件
void mainwindow::uploadFilesInFolder(const QString& folderPath, const QString& relativePath)
{
    QDirIterator iterator(folderPath,
                          QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    
    // 获取根文件夹名称用于状态跟踪
    QString rootFolderName = relativePath.split("/").first();
    
    while (iterator.hasNext())
    {
        QString itemPath = iterator.next();
        QFileInfo fileInfo(itemPath);
        
        if (fileInfo.isDir())
        {
            // 如果是目录，先创建远程目录，然后递归处理
            QString newRelativePath = relativePath + "/" + fileInfo.fileName();
            createRemoteDirectory(newRelativePath);
            uploadFilesInFolder(itemPath, newRelativePath);
        }
        else if (fileInfo.isFile())
        {
            // 增加该文件夹的文件计数
            folderUploadStatus[rootFolderName]++;
            // 如果是文件，创建上传任务
            FileTrans* currentFileTrans = new FileTrans(THREAD_NUM);
            currentFileTrans->curFilePath = filetrans->curFilePath;
            
            // 添加相对路径到当前路径
            QStringList pathParts = relativePath.split("/", Qt::SkipEmptyParts);
            for (const QString& part : pathParts)
            {
                currentFileTrans->curFilePath.append(part);
            }
            
            currentFileTrans->reInit(itemPath);
            uploadList[currentFileTrans->url] = 0;
            
            qDebug() << "上传文件:" << itemPath << "到路径:" << currentFileTrans->url;
            
            // 添加到活跃传输列表
            activeTransfers.append(currentFileTrans);
            transferWidget->addTranserFile(currentFileTrans);
            
            // 连接分片完成信号
            connect(currentFileTrans, &FileTrans::splitFinish, this, [this, currentFileTrans, rootFolderName]() {
                auto threadPool = QThreadPool::globalInstance();
                
                QString currentUrl = currentFileTrans->url;
                QString currentFilename = currentFileTrans->filename;
                
                for (auto& chunk : currentFileTrans->fileChunkList) {
                    auto task = new ThreadTask(chunk);
                    threadPool->start(task);
                    
                    connect(task, &ThreadTask::chunkFinish, this, [this, currentUrl, currentFileTrans, rootFolderName]
                            (QString filename, QString url, int chunkId) {
                        uploadList[url]++;
                        if (uploadList[url] == THREAD_NUM) {
                            qDebug() << "文件上传完成:" << filename;
                            QUrl u(url);
                            QNetworkRequest req(u);
                            req.setRawHeader("Result", "END");
                            this->manager->post(req, nullptr);
                            
                            // 传输完成，清理FileTrans对象
                            activeTransfers.removeOne(currentFileTrans);
                            currentFileTrans->deleteLater();
                            
                            // 减少文件夹中剩余文件计数
                            folderUploadStatus[rootFolderName]--;
                            checkFolderUploadComplete(rootFolderName);
                        }
                    });
                    
                    connect(task, &ThreadTask::chunkSend, this, [this]
                            (QString filename, int chunkId, qint64 chunkSize, qint64 haveSend) {
                        transferWidget->updateProgress(filename, chunkId, chunkSize, haveSend);
                    });
                }
            });
        }
    }
}

// 创建远程目录
void mainwindow::createRemoteDirectory(const QString& dirPath)
{
    // 构建创建目录的请求
    QUrl url("http://127.0.0.1:8888/createDir");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    // 构建当前路径
    QString fullPath = getCurInDir(filetrans->curFilePath) + dirPath;
    
    json j;
    j["dirName"] = fullPath.toStdString();
    
    QByteArray data = QByteArray::fromStdString(j.dump());
    
    QNetworkReply* reply = manager->post(request, data);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply, dirPath]()
            {
        if (reply->error() == QNetworkReply::NoError)
        {
            qDebug() << "成功创建远程目录:" << dirPath;
        } else {
            qDebug() << "创建远程目录失败:" << dirPath << ", 错误:" << reply->errorString();
        }
        reply->deleteLater();
     });
}

// 检查文件夹上传是否完成
void mainwindow::checkFolderUploadComplete(const QString& folderName)
{
    if (folderUploadStatus.contains(folderName) && folderUploadStatus[folderName] <= 0)
    {
        // 文件夹上传完成
        folderUploadStatus.remove(folderName);
        
        QMessageBox::information(this, "上传完成", QString("文件夹 '%1' 上传完成！").arg(folderName));
        qDebug() << "文件夹上传完成:" << folderName;
        
        // 刷新文件列表显示
        if (listWidget)
        {
            listWidget->getCurFileList();
        }
    }
}

void mainwindow::recordUnfinish()
{
    auto downloadList = downloadWidget->transferList;

    //记录未下载完的文件
    for(auto& transInfo:downloadList)
    {
        json j;
        j["fileName"] = transInfo.filename.toStdString();
        j["url"] = transInfo.url.toStdString();
        j["savePath"] = transInfo.savePath.toStdString();
        j["threadNum"] = transInfo.threadNum;
        j["filePath"] = transInfo.filePath.toStdString();
        j["size"] = transInfo.size;

        for(int i = 0;i < transInfo.threadNum;i++)
        {
            if(transInfo.haveTrans[i] < transInfo.chunkSize[i])
            {
                //写入大小
                j["chunk_" + std::to_string(i)] = transInfo.haveTrans[i];
            }
        }
        qDebug() << "Current working dir:" << QDir::currentPath();

        QFile f("./temp/"+transInfo.filename + ".temp");
        if (!f.open(QFile::ReadWrite))
        {
            qDebug() << "无法写入进度" ;
            return;
        }

        f.write(j.dump().c_str());
        f.close();
    }

    //告诉服务器客户端退出，记录上传进度
    auto uploadList = transferWidget->transferList;
    for(auto& transInfo:uploadList)
    {
        qDebug() << "[DEBUG]transferList:" <<transferWidget->transferList.size();

        json j;
        j["fileName"] = transInfo.filename.toStdString();
        j["url"] = transInfo.url.toStdString();
        //j["savePath"] = transInfo.savePath.toStdString();
        j["threadNum"] = transInfo.threadNum;
        j["filePath"] = transInfo.filePath.toStdString();
        j["size"] = transInfo.size;

        for(int i = 0;i < transInfo.threadNum;i++)
        {
            if(transInfo.haveTrans[i] < transInfo.chunkSize[i] && transInfo.haveTrans[i] != 0)
            {
                //写入大小
                j["chunk_" + std::to_string(i)] = transInfo.haveTrans[i];
            }
        }

        qDebug() << "[DEBUG]JSON: " << j.dump();

        QUrl u("http://192.168.111.200:8888/resumeUpload/" + this->userRootFilePath);
        QNetworkRequest req(u);
        auto reply = manager->post(req,j.dump().c_str());
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        qDebug() << "[DEBUG]reply: " << reply->headers();
    }

}

void mainwindow::getLastUnfinishFile()
{
    getLastDownloadFile();
    getLastTransFile();
}

void mainwindow::getLastDownloadFile()
{
    QDir tempDir("./temp");

    QStringList files = tempDir.entryList(QDir::Files);
    for (const QString& file : files)
    {
        QFile f(tempDir.filePath(file));
        if (!f.open(QFile::ReadWrite))
        {
            qDebug() << "无法获取进度" ;
            return;
        }
        auto buf = f.readAll().toStdString();

        if (!nlohmann::json::accept(buf))
        {
            qDebug() << "非法的 JSON 文件，跳过：" << file;
            f.close();
            continue;
        }
        qDebug() << "JSON文件:" << buf;
        json j = json::parse(buf);

        TransferInfo transInfo;
        if (j.contains("url") && !j["url"].is_null()) {
            transInfo.url = QString::fromStdString(j["url"].get<std::string>());
        } else {
            transInfo.url = "";
            qDebug() << "警告：url字段为空或格式错误";
        }
        
        if (j.contains("size") && !j["size"].is_null()) {
            // 安全地获取size字段，处理可能的字符串类型
            if (j["size"].is_string()) {
                std::string sizeStr = j["size"].get<std::string>();
                transInfo.size = std::stoll(sizeStr);
            } else {
                transInfo.size = j["size"].get<qint64>();
            }
        } else {
            transInfo.size = 0;
            qDebug() << "警告：size字段为空或格式错误";
        }
        
        if (j.contains("savePath") && !j["savePath"].is_null()) {
            transInfo.savePath = QString::fromStdString(j["savePath"].get<std::string>());
            qDebug() << "savePath: " << transInfo.savePath;
        } else {
            transInfo.savePath = "";
            qDebug() << "警告：savePath字段为空或格式错误";
        }
        
        if (j.contains("filePath") && !j["filePath"].is_null()) {
            transInfo.filePath = QString::fromStdString(j["filePath"].get<std::string>());
        } else {
            transInfo.filePath = "";
            qDebug() << "警告：filePath字段为空或格式错误";
        }
        
        if (j.contains("fileName") && !j["fileName"].is_null()) {
            transInfo.filename = QString::fromStdString(j["fileName"].get<std::string>());
        } else {
            transInfo.filename = "";
            qDebug() << "警告：fileName字段为空或格式错误";
        }
        if (j.contains("threadNum") && !j["threadNum"].is_null()) {
            transInfo.threadNum = j["threadNum"].get<int>();
        } else {
            transInfo.threadNum = 4; // 默认4个线程
            qDebug() << "警告：threadNum字段为空或格式错误，使用默认值4";
        }
        transInfo.chunkSize = QVector<qint64>(transInfo.threadNum,transInfo.size/transInfo.threadNum);
        transInfo.chunkSize[transInfo.threadNum-1] = transInfo.size - (transInfo.threadNum-1) * transInfo.chunkSize[0];
        transInfo.haveTrans = QVector<qint64>(transInfo.threadNum,0);
        for(int i = 0; i < transInfo.threadNum;i++)
        {
            if(j.contains("chunk_"+std::to_string(i)))
            {
                // chunk_i 存储的是该块已下载的字节数
                // 安全地获取chunk字段，处理可能的字符串类型
                qint64 chunkDownloaded;
                std::string chunkKey = "chunk_" + std::to_string(i);
                if (j[chunkKey].is_string()) {
                    std::string chunkStr = j[chunkKey].get<std::string>();
                    chunkDownloaded = std::stoll(chunkStr);
                } else {
                    chunkDownloaded = j[chunkKey].get<qint64>();
                }
                transInfo.haveTrans[i] = chunkDownloaded;
                qDebug() << "块" << i << "已下载：" << chunkDownloaded << "字节";
            }
            else
            {
                // 如果没有记录，说明该块还没有开始下载
                transInfo.haveTrans[i] = 0;
                qDebug() << "块" << i << "未开始下载";
            }
        }

        downloadList[transInfo.filename] = transInfo;
        downloadWidget->addDowloadFile(transInfo.filename);

        addLastDownloadTask(downloadList[transInfo.filename]);

        f.close();

        tempDir.remove(file);
    }
}

void mainwindow::getLastTransFile()
{
    QUrl u("http://192.168.111.200:8888/resumeUpload/" + userRootFilePath);
    QNetworkRequest req(u);

    auto reply = manager->get(req);

    connect(reply,&QNetworkReply::finished,this,[this](){
        auto reply = qobject_cast<QNetworkReply * >(sender());

        auto content = reply->readAll().toStdString();

        qDebug() << "[DEBUG]继续上传：" << content;

        if (!nlohmann::json::accept(content))
        {
            //JSON非法
            return;
        }

        json j = json::parse(content);

        for(auto item : j)
        {
            FileTrans * filetrans = new FileTrans();
            filetrans->filename = QString::fromStdString(item["fileName"].get<std::string>());
            filetrans->filePath = QString::fromStdString(item["filePath"].get<std::string>());
            filetrans->url = QString::fromStdString(item["url"].get<std::string>());
            filetrans->threadNum = item["threadNum"].get<int>();
            filetrans->size = item["size"].get<qint64>();
            // 首先初始化fileChunkList
            filetrans->fileChunkList.clear();
            
            for(int i = 0;i < filetrans->threadNum;i++)
            {
                FileChunk fileChunk{
                    i,
                    filetrans->filename,
                    filetrans->url,
                    filetrans->filePath,
                    filetrans->size/filetrans->threadNum * i,
                    filetrans->size/filetrans->threadNum,
                    filetrans->size,
                    0
                };
                if(item.contains("chunk_"+std::to_string(i)))
                {
                    fileChunk.haveTrans = item["chunk_"+std::to_string(i)].get<qint64>();
                }
                
                // 将FileChunk添加到FileTrans对象的fileChunkList中
                filetrans->fileChunkList.append(fileChunk);

                //添加到线程池
                auto threadPool = QThreadPool::globalInstance();
                ThreadTask * task = new ThreadTask(fileChunk);
                threadPool->start(task);

                //传输完成
                connect(task,&ThreadTask::chunkFinish,this,[this,filetrans]
                        (QString filename,QString url,int chunkId){
                            uploadList[url]++;
                            if(uploadList[url] == THREAD_NUM)
                            {

                                QUrl u(url);
                                QNetworkRequest req(u);
                                req.setRawHeader("Result","END");
                                auto reply = this->manager->post(req,nullptr);
                                
                                connect(reply,&QNetworkReply::finished,this,
                                        [this,filetrans,url](){
                                    // 传输完成，清理FileTrans对象
                                    qDebug() << "url : " << url << "发送完成";
                                    activeTransfers.removeOne(filetrans);
                                    filetrans->deleteLater();
                                });

                            }
                        });

                connect(task,&ThreadTask::chunkSend,this,[this]
                        (QString filename,int chunkId, qint64 chunkSize,qint64 haveSend){
                            // 使用独立的文件名和块ID更新进度
                            transferWidget->updateProgress(filename, chunkId, chunkSize, haveSend);
                        });
            }

            // 添加到活跃传输列表进行内存管理
            activeTransfers.append(filetrans);
            transferWidget->addTranserFile(filetrans);
        }

    });

}

//添加到线程池
void mainwindow::addLastDownloadTask(TransferInfo transInfo)
{
    // 添加调试信息
    qDebug() << "开始恢复下载任务：" << transInfo.filename;
    qDebug() << "线程数：" << transInfo.threadNum;
    qDebug() << "文件大小：" << transInfo.size;
    qDebug() << "URL：" << transInfo.url;
    
    // 检查必要字段是否有效
    if (transInfo.url.isEmpty()) {
        qDebug() << "错误：URL为空，无法开始下载";
        return;
    }
    
    if (transInfo.threadNum <= 0 || transInfo.threadNum > 16) {
        qDebug() << "错误：线程数无效：" << transInfo.threadNum;
        return;
    }
    
    for(int i = 0; i < transInfo.threadNum; i++)
    {
        // 计算正确的chunkStart位置
        qint64 chunkStart = i * (transInfo.size / transInfo.threadNum);
        
        qDebug() << "块" << i << "：开始位置=" << chunkStart 
                 << "，大小=" << transInfo.chunkSize[i] 
                 << "，已传输=" << transInfo.haveTrans[i];
        
        FileChunk fileChunk{
            i,
            transInfo.filename,
            transInfo.url,
            transInfo.savePath,
            chunkStart,  // 使用计算出的chunkStart
            transInfo.chunkSize[i],
            transInfo.size,
            transInfo.haveTrans[i]
        };

        auto task = new DownloadTask(fileChunk, transInfo.savePath);
        QThreadPool::globalInstance()->start(task);
        
        qDebug() << "已启动下载任务块" << i;

        //提交进度
        connect(task, &DownloadTask::haveWriteSig, this, [this]
                (const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite){
                    this->downloadWidget->updateProgress(filename, chunkId, chunkSize, haveWrite);
                });
    }
}



//暂停传输
void mainwindow::on_stopBtn_clicked()
{
    ThreadTask::isStop = true;
    DownloadTask::isStop = true;
}

//继续传输
void mainwindow::on_continueBtn_clicked()
{
     ThreadTask::isStop = false;
    DownloadTask::isStop = false;
}


void mainwindow::on_uploadFile_clicked()
{
    qDebug() << "上传文件";

    QString filePath = QFileDialog::getOpenFileName(this,"选择文件","/home","全部(*.*)))");
    if( filePath.isEmpty() || filePath == "" )
    {
        return;
    }

    qDebug() << filePath;

    // 为每个文件创建独立的FileTrans对象
    FileTrans* currentFileTrans = new FileTrans(THREAD_NUM);
    currentFileTrans->curFilePath = filetrans->curFilePath; // 复制当前路径
    currentFileTrans->reInit(filePath);
    uploadList[currentFileTrans->url] = 0;
    //qDebug() << "url: "<< currentFileTrans->url;

    // 添加到活跃传输列表
    activeTransfers.append(currentFileTrans);



    //分片完成放入线程池
    connect(currentFileTrans,&FileTrans::splitFinish,this,[this, currentFileTrans](){
        auto threadPool = QThreadPool::globalInstance();

        transferWidget->addTranserFile(currentFileTrans);
        // 捕获当前FileTrans对象，确保每个文件使用独立的对象
        QString currentUrl = currentFileTrans->url;
        QString currentFilename = currentFileTrans->filename;

        for(auto & chunk : currentFileTrans->fileChunkList)
        {
            auto task = new ThreadTask(chunk);
            threadPool->start(task);
            connect(task,&ThreadTask::chunkFinish,this,[this, currentUrl, currentFileTrans]
                    (QString filename,QString url,int chunkId){
                        uploadList[url]++;
                        if(uploadList[url] == THREAD_NUM)
                        {
                            qDebug() << "url : " << url << "发送完成";
                            QUrl u(url);
                            QNetworkRequest req(u);
                            req.setRawHeader("Result","END");
                            this->manager->post(req,nullptr);

                            // 传输完成，清理FileTrans对象
                            activeTransfers.removeOne(currentFileTrans);
                            currentFileTrans->deleteLater();
                            qDebug() << "FileTrans对象已标记删除: " << filename;
                        }
                    });

            connect(task,&ThreadTask::chunkSend,this,[this]
                    (QString filename,int chunkId, qint64 chunkSize,qint64 haveSend){
                        // 使用独立的文件名和块ID更新进度
                        transferWidget->updateProgress(filename, chunkId, chunkSize, haveSend);
                    });
        }
    });

}


void mainwindow::on_uploadDir_clicked()
{
    qDebug() << "上传文件夹";

    QString folderPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if (folderPath.isEmpty()) {
        return;
    }

    qDebug() << "选择的文件夹:" << folderPath;
    uploadFolder(folderPath);
}


void mainwindow::on_createDir_clicked()
{
    qDebug() << "创建文件夹";
    listWidget->CreateDir();
}


void mainwindow::on_cloudAdd_clicked()
{
    CloudAddDialog cloudAddDlg;

    if(cloudAddDlg.exec() == QDialog::Accepted)
    {
        QString shareLink = cloudAddDlg.getAddr();
        QString extractCode = cloudAddDlg.getExtractCode();
        //发送服务器复制
        QString curInDir = listWidget->getCurInDir(listWidget->curFilePath);
        QString url = "http://192.168.111.200:8888/shared/"  +curInDir;

        json j;
        j["shareLink"] = shareLink.toStdString();
        j["extractCode"] = extractCode.toStdString();

        QUrl u(url);
        QNetworkRequest req(u);
        auto reply = manager->post(req,j.dump().c_str());

        //结束后提示
        connect(reply,&QNetworkReply::finished,this,[this](){
            auto reply = qobject_cast<QNetworkReply * >(sender());

            if (reply->error() == QNetworkReply::NoError)
            {
                int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                //文件不存在
                if(statusCode == 500)
                {
                    return;
                }

                QMessageBox::information(
                    nullptr,          // 父窗口
                    "信息",           // 标题
                    "添加成功！"      // 内容
                    );
            }
        });
    }
}

