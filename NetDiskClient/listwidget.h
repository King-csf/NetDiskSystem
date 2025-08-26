#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include "filetrans.h"
#include "fileinfo.h"
#include <QVector>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include "json.hpp"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMenu>
#include <QTreeView>
#include "createdirdialog.h"
#include "player.h"

using json = nlohmann::json;

namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);
    ~ListWidget();

    FileTrans * filetrans;

    QStandardItemModel* model;
    QNetworkAccessManager *manager;

    QList<FileInfo> curFileList;
    QVector<FileInfo> lastFileList;

     QString userRootFilePath;//用户根目录
     QVector<QString> curFilePath;  //当前所在目录
    QString curPath;
    static QString account; // 账号

    void initTableView();
    //获取用户首页的文件
    void parsePageFileName(QString fileName);
    //发送请求
    void sendCreateDirReq(QString dirName);

    void flushListView(QString fileName,QString lastTime);
    //获取当前所在文件夹的字符串
    QString getCurInDir(QVector<QString> curFilePath);
    void getCurFileList(); //获取文件列表
    void reSetFileList(); //重新显示文件列表

    void CreateDir();
    void addCurFilePath(QString dir);

signals:
    void getCurFileListFinish(QString path);
    void haveAddDownloadFile(QString fileName);
    void haveWriteSig(const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite);
    void haveDelFile(QString fileName);

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_toolButton_clicked();

    // void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::ListWidget *ui;


};

#endif // LISTWIDGET_H
