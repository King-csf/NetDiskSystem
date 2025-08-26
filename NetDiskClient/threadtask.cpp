#include "threadtask.h"

bool ThreadTask::isStop = false;

ThreadTask::ThreadTask() {}

ThreadTask::ThreadTask(FileChunk fileChunk)
{
    this->fileChunk = fileChunk;

    //任务结束自动 delete
    setAutoDelete(true);
}

ThreadTask::~ThreadTask()
{

}

void ThreadTask::run()
{
    QNetworkAccessManager  manager;
    QUrl u(fileChunk.url);
    QEventLoop loop;               // 事件循环，等待异步完成

    QFile f(fileChunk.filePath);
    if (!f.open(QFile::ReadOnly)) {
        qDebug() << "无法打开文件:" << fileChunk.filePath;
        return;
    }
    
    // 计算实际的起始位置（考虑已传输的部分）
    qint64 actualStart = fileChunk.chunkStart + fileChunk.haveTrans;
    f.seek(actualStart);

    qint64 haveSend = fileChunk.haveTrans;
    
    qDebug() << QString("分片 %1 开始上传: 起始位置=%2, 已传输=%3, 剩余=%4")
                .arg(fileChunk.chunkId).arg(actualStart).arg(haveSend)
                .arg(fileChunk.chunkSize - haveSend);

    while (haveSend < fileChunk.chunkSize)
    {
        if(isExit)
            return;

        while(isStop)
        {
            if(isExit)
                return;
        }

        char buf[BUFFER_SIZE];
        int maxReadSize = fileChunk.chunkSize-haveSend > BUFFER_SIZE
                              ? BUFFER_SIZE :fileChunk.chunkSize-haveSend;
        qint64 readSize = f.read(buf,maxReadSize);

        if (readSize < 0 )
        {
            //读错误
            qDebug() << "文件读取错误, chunkId:" << fileChunk.chunkId;
            
            //发送错误
            QNetworkRequest req(u);
            req.setRawHeader("Error","ReadError");
            req.setRawHeader("Chunk-Id",QByteArray::number(fileChunk.chunkId));
            QNetworkReply *errorReply = manager.post(req,nullptr);
            
            // 等待错误请求完成
            QObject::connect(errorReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            
            if (errorReply->error() != QNetworkReply::NoError) {
                qDebug() << "发送错误信息失败:" << errorReply->errorString();
            }
            errorReply->deleteLater();
            break;
        }
        if(readSize == 0)
        {
            continue;
        }
        
        QNetworkRequest req(u);
        req.setRawHeader("Content-Length",QByteArray::number(readSize));
        req.setRawHeader("Chunk-Id",QByteArray::number(fileChunk.chunkId));
        req.setRawHeader("HaveRead",QByteArray::number(haveSend + readSize));
        req.setRawHeader("Chunk-Start",QByteArray::number(fileChunk.chunkStart));
        req.setRawHeader("Chunk-Size",QByteArray::number(fileChunk.chunkSize));
        req.setRawHeader("File-Offset",QByteArray::number(actualStart + (haveSend - fileChunk.haveTrans)));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

        QByteArray data(buf, readSize);  // 深拷贝数据
        QNetworkReply *reply = manager.post(req, data);
        
        // 等待请求完成
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        
        // 检查请求结果
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "发送数据块失败, chunkId:" << fileChunk.chunkId << ", error:" << reply->errorString();
            
            // 检查是否是网络错误，如果是则重试
            if (reply->error() == QNetworkReply::TimeoutError || 
                reply->error() == QNetworkReply::TemporaryNetworkFailureError ||
                reply->error() == QNetworkReply::NetworkSessionFailedError) {
                qDebug() << "网络错误，将重试分片" << fileChunk.chunkId;
                // 这里可以添加重试逻辑
            }
        } else {
            // 检查服务器响应
            QByteArray responseData = reply->readAll();
            if (!responseData.isEmpty()) {
                qDebug() << "服务器响应:" << responseData;
            }
            
            haveSend += readSize;
            emit chunkSend(fileChunk.filename, fileChunk.chunkId, fileChunk.chunkSize, haveSend);
            
        }
        
        reply->deleteLater();

    }
    
    f.close();
    emit chunkFinish(fileChunk.filename,fileChunk.url,fileChunk.chunkId);
}


// void ThreadTask::run()
// {
//     qDebug() << "run : " << fileChunk.chunkId;
//     manager = new QNetworkAccessManager();
//     QUrl u(fileChunk.url);

//     f = new QFile(fileChunk.filePath);
//     // QFile f(fileChunk.filePath);
//     if (!f->open(QFile::ReadOnly))
//     {
//         qDebug() << "无法打开文件:" << fileChunk.filePath;
//         return;
//     }
//     haveSend = 0;

//     sendNextMsg(u);
//     qDebug() << "end : " << fileChunk.chunkId;
// }

// void ThreadTask::sendNextMsg(QUrl u)
// {
//     qDebug() << "sendNextMsg : " << fileChunk.chunkId;
//     if(haveSend >= fileChunk.chunkSize)
//     {
//         emit chunkFinish(fileChunk.url,fileChunk.chunkId);
//         return;
//     }

//     char buf[4096];
//     int maxReadSize = fileChunk.chunkSize-haveSend > 4096
//                                   ? 4096 :fileChunk.chunkSize-haveSend;
//     f->seek(fileChunk.chunkStart + haveSend);
//     qint64 readSize = f->read(buf,maxReadSize);

//     QNetworkRequest req(u);
//     req.setRawHeader("Content-Length",QByteArray::number(readSize));
//     req.setRawHeader("Chunk-Id",QByteArray::number(fileChunk.chunkId));
//     req.setRawHeader("HaveRead",QByteArray::number(haveSend + readSize));
//             //req.setRawHeader("begin")
//     req.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

//     QByteArray data(buf, readSize);  // 深拷贝数据
//     QNetworkReply *reply = manager->post(req, data);

//     connect(reply, &QNetworkReply::finished, this , [this,u,readSize](){
//         auto reply = qobject_cast<QNetworkReply*>(sender());
//         if(reply->readAll().toStdString() == "done")
//         {
//             this->haveSend += readSize;
//             sendNextMsg(u);
//         }
//         else
//         {
//             sendNextMsg(u);
//         }
//     });
// }
