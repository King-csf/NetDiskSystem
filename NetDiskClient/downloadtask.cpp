#include "downloadtask.h"

bool DownloadTask::isStop = false;
bool isExit = false;

DownloadTask::DownloadTask() {}

DownloadTask::DownloadTask(FileChunk fileChunk,QString savePath)
{
    this->fileChunk = fileChunk;
    this->savePath = savePath;

    //任务结束自动 delete
    setAutoDelete(true);
}



void DownloadTask::run()
{

    QNetworkAccessManager manager;

    QFile f(savePath);
    if (!f.open(QFile::ReadWrite))
    {
        qDebug() << "无法打开文件：" << savePath;
        return;
    }

    qint64 haveWrite = fileChunk.haveTrans;
    QEventLoop loop;

    while (haveWrite < fileChunk.chunkSize)
    {
        if(isExit)
            return;

        while(isStop)
        {
            if(isExit)
                return;
        }

        int retry = 3; //重试次数

        QUrl u(fileChunk.url);
        QNetworkRequest req(u);

        qint64 rangeStart = haveWrite + fileChunk.chunkStart;
        qint64 rangeEnd = qMin(rangeStart + BUFFER_SIZE - 1,
                               fileChunk.chunkStart + fileChunk.chunkSize - 1);

        QByteArray rangeHeader = QByteArray("bytes=") +
                                 QByteArray::number(rangeStart) + "-" +
                                 QByteArray::number(rangeEnd);
        req.setRawHeader("Range", rangeHeader);
        req.setTransferTimeout(5'000);   // 5 秒无数据即超时

        QNetworkReply *reply = manager.get(req);
        QTimer timer;
        timer.setSingleShot(true);

        // 整体 5 秒超时
        QObject::connect(&timer, &QTimer::timeout, [&]{
            if (reply->isRunning()) reply->abort();
        });
        timer.start(5'000);

        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();


        // 读取状态与头，先保存 body
        QByteArray body = reply->readAll();
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QByteArray contentRange = reply->rawHeader("Content-Range");
        QVariant contentLengthVar = reply->header(QNetworkRequest::ContentLengthHeader);
        qint64 contentLength = contentLengthVar.isValid() ? contentLengthVar.toLongLong() : -1;

        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "发送数据块失败, chunkId:" << fileChunk.chunkId
                     << ", error:" << reply->errorString()
                     << ", httpStatus:" << httpStatus;

            reply->deleteLater();
            //重试
            if(retry != 0)
            {
                retry--;
                continue;
            }

            //记录传输位置


            break;
        }

        // 如果服务器正确返回 206 或者有 Content-Range 头：常规处理
        if (httpStatus == 206 || contentRange.startsWith("bytes"))
        {
            qint64 canWrite = qMin<qint64>(body.size(), fileChunk.chunkSize - haveWrite);
            f.seek(fileChunk.chunkStart + haveWrite);
            qint64 written = f.write(body.constData(), canWrite);
            if (written != canWrite)
            {
                qDebug() << "文件写入不完整, chunkId:" << fileChunk.chunkId
                         << ", expect:" << canWrite << ", wrote:" << written;
            }
            haveWrite += canWrite;
            if (body.size() > canWrite)
            {
                qDebug() << "注意：服务器返回的数据大于本次需要的字节数，已截断多余部分.";
            }
        }
        //提交进度
        emit haveWriteSig(fileChunk.filename, fileChunk.chunkId, fileChunk.chunkSize, haveWrite);
        //qDebug() << "chunkId:" << fileChunk.chunkId << "已写入:" << haveWrite << "/" << fileChunk.chunkSize;

        reply->deleteLater();
    }

    f.close();
    qDebug() << "下载任务完成, chunkId:" << fileChunk.chunkId
             << "     写入：" << haveWrite;
}



