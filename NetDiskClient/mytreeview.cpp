#include "mytreeview.h"


QMap<QString,TransferInfo> downloadList;
QMap<QString, QVector<int>> downloadMap;

MyTreeView::MyTreeView() {}

MyTreeView::MyTreeView(QWidget *parent)
    : QTreeView(parent)
{

}
void MyTreeView::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::RightButton) {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            QMenu menu;
            //下载文件
            menu.addAction(QIcon(":/new/prefix1/image/下载.png"),"下载",[=,this](){

                QModelIndex fileIndex = this->model()->index(index.row(), 0);
                auto fileName = fileIndex.data();
                QString downFilePath = getRightBtnFilePath(fileName.toString());
                
                // 先检查是文件还是文件夹
                QUrl u("http://192.168.111.200:8888/getDownloadInfo/" + downFilePath);
                QNetworkRequest req(u);
                auto reply = manager.get(req);
                
                connect(reply, &QNetworkReply::finished, this, [this, fileName, downFilePath]() {
                    auto reply = qobject_cast<QNetworkReply*>(sender());
                    
                    if (reply->error() != QNetworkReply::NoError) {
                        qDebug() << "获取文件信息失败:" << reply->errorString();
                        reply->deleteLater();
                        return;
                    }
                    
                    QString content = QString::fromStdString(reply->readAll().toStdString());
                    reply->deleteLater();
                    
                    try {
                        json j = json::parse(content.toStdString());
                        QString savePath;
                        
                        if (QString::fromStdString(j["isDirectory"].get<std::string>()) == "false")
                        {
                            // 文件下载
                            QString defaultFile = QDir::homePath() + "/" + fileName.toString();
                            savePath = QFileDialog::getSaveFileName(
                                this,
                                tr("保存文件"),
                                defaultFile,
                                tr("所有文件 (*)"));
                        } else {
                            // 文件夹下载
                            savePath = QFileDialog::getExistingDirectory(
                                this,
                                tr("选择保存目录"),
                                QDir::homePath());
                            
                            if (!savePath.isEmpty())
                            {
                                // 在选择的目录下创建同名文件夹
                                savePath = savePath + "/" + fileName.toString();
                            }
                        }
                        
                        if (!savePath.isEmpty())
                        {
                            getDownloadFileInfo(fileName.toString(), downFilePath, savePath);
                        }
                        
                    } catch (const std::exception& e) {
                        qDebug() << "解析文件信息JSON失败：" << e.what();
                    }
                });
            });

             //删除文件
            menu.addAction(QIcon(":/new/prefix1/image/删除.png"),"删除",[=,this](){

                QModelIndex fileIndex = this->model()->index(index.row(), 0);
                auto fileName = fileIndex.data();
                QString deleFilePath = getRightBtnFilePath(fileName.toString());

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(nullptr, "确认", "是否确认删除？",
                                              QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    deleteFile(deleFilePath);
                }
            });

            //分享文件
            menu.addAction(QIcon(":/new/prefix1/image/分享.png"),"分享",[=,this](){
                QModelIndex fileIndex = this->model()->index(index.row(), 0);
                auto fileName = fileIndex.data();
                QString shareFilePath = getRightBtnFilePath(fileName.toString());

                generateShareLink(shareFilePath);
            });
            menu.exec(viewport()->mapToGlobal(event->pos()));
        }
    }
    QTreeView::mousePressEvent(event);
}

QString MyTreeView::getRightBtnFilePath(const QString& fileName)
{
    QString res;
    for(auto & dir : this->curDirPath)
    {
        qDebug() << "[DEBUG]dir:" << dir;
        res += dir + "/";
    }
    res += fileName;
    return res;
}

QString MyTreeView::getDownloadFileInfo(QString fileName,const QString &filePath,const QString & savePath )
{
    //获取要下载的文件大小信息，分块
    QUrl u("http://192.168.111.200:8888/getDownloadInfo/" + filePath);
    QNetworkRequest req(u);
    //qDebug() << "[DEBUG]fileName: " << fileName;
    auto reply = manager.get(req);

    connect(reply, &QNetworkReply::finished,this,[this,filePath,savePath,fileName](){
        //解析回复
        auto reply = qobject_cast<QNetworkReply * >(sender());
        
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "获取下载信息失败:" << reply->errorString();
            reply->deleteLater();
            return;
        }
        
        QString content = QString::fromStdString(reply->readAll().toStdString());
        reply->deleteLater();
        //qDebug() << "[DEBUG] json内容：" << content.toStdString();
        json j = json::parse(content.toStdString());

        //判断是否为文件夹
        if(QString::fromStdString(j["isDirectory"].get<std::string>()) == "false")
        {
            // 处理单个文件下载
            qDebug() << "[DEBUG] 下载文件：" << content.toStdString();
            // 安全地获取size字段，处理可能的字符串类型
            qint64 size;
            if (j["size"].is_string()) {
                std::string sizeStr = j["size"].get<std::string>();
                size = std::stoll(sizeStr);
            } else {
                size = j["size"].get<qint64>();
            }
            splitFile(fileName,filePath,savePath,size);
        }
        else
        {
            // 处理文件夹下载
            qDebug() << "[DEBUG] 下载文件夹：" << fileName;
            downloadFolder(fileName, filePath, savePath);
        }
    });

    return "";

}

void MyTreeView::splitFile(QString fileName,QString filePath, QString savePath,qint64 size)
{
    // 确保父目录存在
    QFileInfo fileInfo(savePath);
    QDir parentDir = fileInfo.absoluteDir();
    if (!parentDir.exists())
    {
        if (!parentDir.mkpath("."))
        {
            qDebug() << "创建父目录失败：" << parentDir.absolutePath();
            return;
        }
    }
    
    // 预先创建指定大小的文件，避免多线程冲突
    QFile preFile(savePath);
    if(preFile.open(QFile::WriteOnly)) {
        preFile.resize(size);  // 预分配文件大小
        preFile.close();
        qDebug() << "文件预分配成功，大小:" << size;
    } else {
        qDebug() << "文件预分配失败:" << savePath;
        return;
    }
    
    qint64 chunkSize = size / THREAD_NUM;
    for(int i = 0; i < THREAD_NUM;i++)
    {
        FileChunk fileChunk{
                i,
                fileName,
                "http://192.168.111.200:8888/download/" + filePath,
                savePath,
                i*chunkSize,
                chunkSize,
                size,
                0
        };
        if(i == THREAD_NUM-1)
            fileChunk.chunkSize = size - fileChunk.chunkStart;
        auto task = new DownloadTask(fileChunk,savePath);
        QThreadPool::globalInstance()->start(task);

        //提交进度
        connect(task,&DownloadTask::haveWriteSig,this,[this]
                (const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite){
            emit haveWriteSig(filename, chunkId, chunkSize, haveWrite);
        });
    }
    addDownloadList(fileName,filePath,savePath,size);
}

void MyTreeView::addDownloadList(QString fileName,QString filePath,QString savePath,qint64 size)
{
    //添加到下载列表
    TransferInfo transferInfo;
    transferInfo.filename = fileName;
    transferInfo.filePath  = filePath;
    transferInfo.savePath = savePath;
    transferInfo.url = "http://192.168.111.200:8888/download/" + filePath;
    transferInfo.threadNum = THREAD_NUM;
    transferInfo.size = size;
    for(int i = 0;i < THREAD_NUM;i++)
    {
        transferInfo.chunkSize.push_back(size/THREAD_NUM);
        if(i == THREAD_NUM-1)
        {
            transferInfo.chunkSize[i] = size - i*(size/THREAD_NUM);
        }
    }
    downloadList[fileName] = transferInfo;
    emit haveAddDownloadFile(fileName);
}

void MyTreeView::deleteFile(QString deleFilePath)
{
    QUrl u("http://192.168.111.200:8888/delete/" + deleFilePath);
    QNetworkRequest req(u);
    auto reply = manager.get(req);

    connect(reply, &QNetworkReply::finished,this,[this,deleFilePath](){
        //解析回复
        auto reply = qobject_cast<QNetworkReply * >(sender());

        if (reply->error() == QNetworkReply::NoError)
        {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            //文件不存在
            if(statusCode == 406)
            {
                return;
            }
            auto pos = deleFilePath.toStdString().find_last_of("/");
            auto fileName = QString::fromStdString(deleFilePath.toStdString().substr(pos+1));
            emit haveDelFile(fileName);
        }
    });
}

// void MyTreeView::onDownloadCompleted(int chunkId, bool success, qint64 downloadedSize)
// {
//     // Handle download completion for a specific chunk
//     qDebug() << "Chunk" << chunkId << "completed. Success:" << success << "Size:" << downloadedSize;
//     // TODO: Update download progress and check if all chunks are completed
// }

// void MyTreeView::onDownloadProgress(int chunkId, qint64 downloaded, qint64 total)
// {
//     // Handle download progress for a specific chunk
//     qDebug() << "Chunk" << chunkId << "progress:" << downloaded << "/" << total;
//     // TODO: Update UI progress bar
// }

void MyTreeView::downloadFolder(const QString &folderName, const QString &folderPath, const QString &savePath)
{
    // 创建文件夹保存目录
    QDir saveDir(savePath);
    if (!saveDir.exists())
    {
        if (!saveDir.mkpath(savePath))
        {
            qDebug() << "创建保存目录失败：" << savePath;
            return;
        }
    }
    
    // 开始递归下载文件夹
    downloadFolderRecursive(folderPath, savePath);
}

void MyTreeView::downloadFolderRecursive(const QString &folderPath, const QString &savePath)
{
    // 获取文件夹内容
    QUrl u("http://192.168.111.200:8888/getDownloadInfo/" + folderPath);
    QNetworkRequest req(u);
    qDebug() << "[DEBUG] 获取文件夹内容：" << folderPath;
    
    auto reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, this, [this, folderPath, savePath]() {
        auto reply = qobject_cast<QNetworkReply*>(sender());
        
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "获取文件夹信息失败:" << reply->errorString();
            reply->deleteLater();
            return;
        }
        
        QString content = QString::fromStdString(reply->readAll().toStdString());
        reply->deleteLater();
        
        try {
            json j = json::parse(content.toStdString());
            
            // 确认是文件夹
            if (QString::fromStdString(j["isDirectory"].get<std::string>()) == "true")
            {
                json files = j["files"];
                
                // 遍历文件夹中的每个文件/子文件夹
                for (const auto& file : files)
                {
                    QString fileName = QString::fromStdString(file["filename"].get<std::string>());
                    QString currentFilePath = folderPath + "/" + fileName;
                    QString currentSavePath = savePath + "/" + fileName;
                    
                    // 检查是否有size字段来判断是文件还是文件夹
                    if (file.contains("size") && file["size"].get<std::string>() != "-")
                    {
                        // 文件，直接下载
                        // 将字符串类型的size转换为qint64
                        std::string sizeStr = file["size"].get<std::string>();
                        qint64 fileSize = std::stoll(sizeStr);
                        splitFile(fileName,currentFilePath,currentSavePath,fileSize);
                        //qDebug() << "[DEBUG] 下载文件：" << fileName << "路径：" << currentFilePath;
                        //getDownloadFileInfo(fileName, currentFilePath, currentSavePath);
                    } else
                    {
                        // 文件夹，递归下载
                        //qDebug() << "[DEBUG] 进入子文件夹：" << fileName;
                        //确保文件夹路径存在
                        QDir saveDir;
                        if (!saveDir.exists(currentSavePath))
                        {
                            if (!saveDir.mkpath(currentSavePath))
                            {
                                qDebug() << "创建文件夹失败：" << currentSavePath;
                                continue;
                            }
                        }
                        downloadFolderRecursive(currentFilePath, currentSavePath);
                    }
                }
            }
        } catch (const std::exception& e) {
            qDebug() << "解析文件夹内容JSON失败：" << e.what();
        }
    });
}

void MyTreeView::generateShareLink(QString filePath)
{
    QString url = "http://192.168.111.200:8888/shared/" + filePath;
    QUrl u(url);
    QNetworkRequest req(u);
    auto reply = manager.get(req);

    connect(reply,&QNetworkReply::finished,this,[this,url](){
        auto reply = qobject_cast<QNetworkReply * >(sender());

        if (reply->error() == QNetworkReply::NoError)
        {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            //文件不存在
            if(statusCode == 406)
            {
                return;
            }
            auto getCode = QString::fromStdString(reply->readAll().toStdString());
            GenerateShareLinkDlg dlg;
            dlg.setGetCode(getCode);
            dlg.setLink(url);
            dlg.exec();
        }
    });
}

// 下载完成回调
void MyTreeView::onDownloadCompleted(int chunkId, bool success, qint64 downloadedSize)
{
    // 处理下载完成逻辑
    qDebug() << "Download chunk" << chunkId << "completed. Success:" << success << "Size:" << downloadedSize;
}

// 下载进度回调
void MyTreeView::onDownloadProgress(int chunkId, qint64 downloaded, qint64 total)
{
    // 处理下载进度逻辑
    qDebug() << "Download chunk" << chunkId << "progress:" << downloaded << "/" << total;
}
