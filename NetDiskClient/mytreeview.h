#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QMenu>
#include <QMouseEvent>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "json.hpp"
#include "config.h"
#include "filetrans.h"
#include "downloadtask.h"
#include <QThreadPool>
#include <QMessageBox>
#include "generatesharelinkdlg.h"

using json = nlohmann::json;

class MyTreeView : public QTreeView
{
    Q_OBJECT
public:
    MyTreeView();
    explicit MyTreeView(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;

    QVector<QString> curDirPath;
    //QNetworkAccessManager *manager;

    // 下载任务管理
    struct DownloadInfo {
        QString fileName;
        QString savePath;
        qint64 totalSize;
        int completedTasks;
        int totalTasks;
        QVector<bool> taskStatus;
    };
    QMap<QString, DownloadInfo> activeDownloads;

    QString getRightBtnFilePath(const QString& fileName);
    QString getDownloadFileInfo(QString fileName,const QString &filePath,const QString & savePath );
    void splitFile(QString fileName,QString filePath, QString savePath,qint64 size);
    void addDownloadList(QString fileName,QString filePath,QString savePath,qint64 size);

    void deleteFile(QString deleFilePath);
    void downloadFolder(const QString &folderName, const QString &folderPath, const QString &savePath);
    void downloadFolderRecursive(const QString &folderPath, const QString &savePath);

    void generateShareLink(QString filePath);

private slots:
    void onDownloadCompleted(int chunkId, bool success, qint64 downloadedSize);
    void onDownloadProgress(int chunkId, qint64 downloaded, qint64 total);

signals:
    void haveAddDownloadFile(QString fileName);
    void haveWriteSig(const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite);
    void haveDelFile(QString fileName);

private:
    void verifyFileIntegrity(const QString& fileName);

    QNetworkAccessManager  manager;
};

#endif // MYTREEVIEW_H
