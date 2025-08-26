#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QRunnable>
#include <QObject>
#include "filetrans.h"
#include <QFile>
#include "config.h"
#include <QEventLoop>
#include <QTimer>



class DownloadTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    DownloadTask();
    DownloadTask(FileChunk fileChunk,QString savePath);
    void run() override;

    static bool isStop;

    FileChunk fileChunk;
    QString savePath;
signals:
    void haveWriteSig(const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite);

};

#endif // DOWNLOADTASK_H
