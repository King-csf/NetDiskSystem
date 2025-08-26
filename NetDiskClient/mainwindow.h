#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
 #include <QStandardItemModel>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include "json.hpp"
#include "fileinfo.h"
#include <QVector>
#include "createdirdialog.h"
#include <QtHttpServer/QHttpServer>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QMessageBox>
#include <QFileDialog>
#include <QThreadPool>
#include "threadtask.h"
#include <QMap>
#include "transferwidget.h"
#include "listwidget.h"
#include "config.h"
#include "mytreeview.h"
#include "cloudadddialog.h"

class FileTrans;

using json = nlohmann::json;

namespace Ui {
class mainwindow;
}

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
    explicit mainwindow(QString userRootFilePath,QString account,QWidget *parent = nullptr);

    //发送请求
    void sendCreateDirReq(QString dirName);

    //获取当前所在文件夹的字符串
    QString getCurInDir(QVector<QString> curFilePath);
    
    // 文件夹上传相关方法
    void uploadFolder(const QString& folderPath);
    void uploadFilesInFolder(const QString& folderPath, const QString& relativePath = "");
    void createRemoteDirectory(const QString& dirPath);
    
    // 文件夹上传状态管理
    QMap<QString, int> folderUploadStatus; // 跟踪每个文件夹的上传进度
    void checkFolderUploadComplete(const QString& folderName);

    void recordUnfinish();

    void getLastUnfinishFile();
    void getLastDownloadFile();
    void getLastTransFile();

    void addLastDownloadTask(TransferInfo transInfo);


    static QString getUserRootFilePath();
    static QVector<QString> getCurFilePath();

private slots:


    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_stopBtn_clicked();

    void on_continueBtn_clicked();

    void on_uploadFile_clicked();

    void on_uploadDir_clicked();

    void on_createDir_clicked();

    void on_cloudAdd_clicked();

private :


    QNetworkAccessManager *manager;

    QVector<FileInfo> curFileList;

    static QString userRootFilePath;//用户根目录
    QString curPath;
    static QString account; // 账号

    FileTrans * filetrans;
    QList<FileTrans*> activeTransfers; // 管理活跃的传输对象

    QMap<QString,int> uploadList;

    TransferWidget * transferWidget;//上传
    TransferWidget * downloadWidget;//下载
    ListWidget * listWidget;

private:
    Ui::mainwindow *ui;
};


#endif // MAINWINDOW_H
