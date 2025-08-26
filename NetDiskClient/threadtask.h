#ifndef THREADTASK_H
#define THREADTASK_H

#include <QRunnable>
#include "filetrans.h"
#include  <QFile>
#include <QMessageBox>
#include <QEventLoop>
#include "config.h"


class ThreadTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    ThreadTask();
    ThreadTask(FileChunk fileChunk);
    ~ThreadTask();

    FileChunk fileChunk;
    // QNetworkAccessManager*  manager;
    void run () override;
    void sendNextMsg(QUrl u);
    static bool isStop;
    // qint64 haveSend;
    // QFile * f;
signals:
    void chunkFinish(QString filename,QString url,int chunkId);
    void chunkSend(QString filename,int chunkId,qint64 chunkSzie, qint64 haveSend);
};

#endif // THREADTASK_H
