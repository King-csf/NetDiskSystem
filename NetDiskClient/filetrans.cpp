#include "filetrans.h"

FileTrans::FileTrans()
{
    manager = new QNetworkAccessManager(this);

}

FileTrans::FileTrans(int threadNum)
{
    this->threadNum = threadNum;
    manager = new QNetworkAccessManager(this);

}

FileTrans::FileTrans(const QString &filename, const QString &filePath, 
                     const QString &url, qint64 size, int threadNum,
                     const QVector<qint64> &chunkProgress)
{
    this->filename = filename;
    this->filePath = filePath;
    this->url = url;
    this->size = size;
    this->threadNum = threadNum;
    
    manager = new QNetworkAccessManager(this);
}



void FileTrans::splitFile()
{
    //获取上次传输进度
    QUrl u(url);
    QNetworkRequest req(u);
    req.setRawHeader("FileName",QByteArray::fromStdString(filename.toStdString()));

    req.setRawHeader("File-Size",QByteArray::number(size));
    req.setRawHeader("Thread-Num",QByteArray::number(threadNum));
    qDebug() << req.headers();
    QNetworkReply *reply = manager->get(req);

    connect(reply, &QNetworkReply::finished, this, [this]()
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "获取上传进度失败:" << reply->errorString();
            qDebug() << "URL:" << reply->url().toString();
            // 即使网络请求失败，也继续进行文件分片
        }
        
        auto respones =  reply->readAll();
        qDebug() << "服务器响应:" << respones;
        //未传输过
        if(respones.toStdString() == "NO Record")
        {

                // 正常分片
                for(int i = 0; i < this->threadNum;i++)
                {
                    FileChunk filechunk
                        {
                            i,
                            this->filename,
                            this->url,
                        this->filePath,
                        this->size/this->threadNum * i,
                        this->size/this->threadNum,
                        this->size,
                        0
                    };

                    if(i == this->threadNum - 1)
                    {
                        filechunk.chunkSize = this->size - this->size/this->threadNum * i;
                    }


                    fileChunkList.push_back(filechunk);
                }
                qDebug() << "[DEBUG]fileChunkList: " << fileChunkList.size();

        }


        emit splitFinish();
    });
}

void FileTrans::setUrl()
{
    url = "http://192.168.111.200:8888/upload/";
    for(QString& str : curFilePath)
    {
        url += str + "/";
    }
    url += filename;
}

void FileTrans::reInit(QString filePath)
{
    this->filePath = filePath;
    getFilenameFromPath();
    setUrl();
    splitFile();
}

void FileTrans::getFilenameFromPath()
{
    filename = QFileInfo(filePath).fileName();
    size = QFileInfo(filePath).size();
}

