#ifndef FILETRANS_H
#define FILETRANS_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include "json.hpp"

using json = nlohmann::json;

typedef struct TransferInfo{
    QString filename;
    QString filePath;
    QString savePath;
    QString url;
    int threadNum;
    QVector<qint64> chunkSize;
    QVector<qint64> haveTrans;
    qint64 size;
}TransferInfo;

typedef struct FileChunk{
    int chunkId;
    QString filename;
    QString url;
    QString filePath;
    qint64 chunkStart;
    qint64 chunkSize;
    qint64 size;
    qint64 haveTrans;
}FileChunk;

class FileTrans : public QObject
{
    Q_OBJECT
public:
    FileTrans();
    FileTrans(int threadNum);
    FileTrans(FileTrans& fileTrans);
    FileTrans(const QString &filename, const QString &filePath, 
              const QString &url, qint64 size, int threadNum,
              const QVector<qint64> &chunkProgress);

    QVector<QString> curFilePath;
    QString filename;
    QString filePath;
    QString url;
    int threadNum;
    qint64 size;
    QNetworkAccessManager * manager;

    QVector<FileChunk> fileChunkList;
    
    // 断点续传相关成员
    bool isResumeMode;
    QVector<qint64> resumeProgress;
    
    void getFilenameFromPath();
    void setUrl();
    void reInit(QString filePath);
    //文件分片
    void splitFile();

signals:
    void splitFinish();
};

#endif // FILETRANS_H
