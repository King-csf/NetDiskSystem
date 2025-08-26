#ifndef TRANSFERWIDGET_H
#define TRANSFERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "filetrans.h"
#include <QMap>
#include "config.h"
#include "progressbardelegate.h"

namespace Ui {
class TransferWidget;
}

class TransferWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransferWidget(QWidget *parent = nullptr);
    ~TransferWidget();

    void initTransList();
    void addTranserFile(FileTrans* fileTrans);
    void updateProgress(const QString& filename, int chunkId, qint64 chunkSize, qint64 haveSent);
    void markChunkComplete(const QString& filename, int chunkId);
    void checkFileComplete(const QString& filename);

    void addDowloadFile(QString fileName);
    void updateDownloadProgress(const QString& filename, int chunkId, qint64 chunkSize, qint64 haveWrite);

    Ui::TransferWidget *ui;
    QStandardItemModel* model;
    QMap<QString,TransferInfo> transferList;
    // 存储每个文件的四个线程进度 [filename][chunkId] = progress
    QMap<QString, QVector<int>> progressMap;


};

#endif // TRANSFERWIDGET_H
