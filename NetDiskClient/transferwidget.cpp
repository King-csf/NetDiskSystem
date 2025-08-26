#include "transferwidget.h"
#include "ui_transferwidget.h"

TransferWidget::TransferWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransferWidget)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"文件名","已传输", "总大小", "进度"});

    ui->treeView->setModel(model);  // 用 QTreeView
    ui->treeView->setRootIsDecorated(false); // 去掉树形展开箭头
    ui->treeView->setItemsExpandable(false);
    ui->treeView->setAllColumnsShowFocus(true);
    // 设置列宽模式
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeView->setFont(QFont("Microsoft YaHei", 14)); // 字体名与字号
    ui->treeView->setStyleSheet(
        "QTreeView::item {"
        "   height: 100px;"   // 每行高度
        "}"
        );
    //无法编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 点击后文字不会被省略/消失
    ui->treeView->setTextElideMode(Qt::ElideNone);
    ui->treeView->setSelectionMode(QAbstractItemView::NoSelection);

    // 隐藏网格线
    ui->treeView->setStyleSheet(
        "QTreeView::item { border: none; }"
        "QTreeView::branch { border: none; }"
        );

    ui->treeView->setItemDelegateForColumn(3, new ProgressBarDelegate(ui->treeView));

}

TransferWidget::~TransferWidget()
{
    delete ui;
}

void TransferWidget::initTransList()
{
    for(auto it = transferList.begin(); it != transferList.end(); it++)
    {
        QList<QStandardItem*> row;
        row << new QStandardItem(it->filename)
            << new QStandardItem(0)
            << new QStandardItem(it->size)
            << new QStandardItem("0%");
            ;
        model->appendRow(row);
    }
}

void TransferWidget::addTranserFile(FileTrans *fileTrans)
{
    qDebug() << "[DEBUG]添加下载列表";
    TransferInfo transInfo{
        .filename = fileTrans->filename,
        .filePath = fileTrans->filePath,
        .url = fileTrans->url,
        .threadNum = fileTrans->threadNum,
        .size = fileTrans->size
    };

    // 初始化haveTrans数组，避免数组越界
    transInfo.haveTrans = QVector<qint64>(transInfo.threadNum, 0);
    transInfo.chunkSize = QVector<qint64>(transInfo.threadNum, 0);

    //设置块大小
    for(auto & chunk : fileTrans->fileChunkList)
    {
        transInfo.chunkSize[chunk.chunkId] = chunk.chunkSize;
    }
    // for(int i = 0; i< fileTrans->threadNum;i++)
    // {

    //     transInfo.chunkSize[i] = fileTrans->fileChunkList[i].chunkSize;
    // }

    transferList[transInfo.filename] = transInfo;


    QList<QStandardItem*> row;
    row << new QStandardItem(transInfo.filename)
        << new QStandardItem(0)
        << new QStandardItem(QString::number(transInfo.size))
        << new QStandardItem("0%");
    ;
    model->appendRow(row);

    // 初始化四线程进度为0
    QVector<int> progresses(transInfo.threadNum, 0);
     progressMap[transInfo.filename] = progresses;

     // 将进度写入第3列的模型数据，以便委托绘制
     int lastRow = model->rowCount() - 1;
     QModelIndex progressIndex = model->index(lastRow, 3);
     model->setData(progressIndex, QVariant::fromValue(progresses));


 }
 
 void TransferWidget::updateProgress(const QString& filename, int chunkId, qint64 chunkSize, qint64 haveSent)
{
    //qDebug() << "[DEBUG]transferList"<< transferList.size();

    if(!transferList.contains(filename))
        return;
    // 计算百分比
    //qDebug() << "[DEBUG] 更新进度条";
    int percent = 0;
    if (chunkSize > 0) {
        percent = static_cast<int>(haveSent * 100 / chunkSize);
        if (percent > 100) percent = 100;
        if (percent < 0) percent = 0;
    }

    // 更新本文件的四段进度
    int threadCount = transferList[filename].threadNum > 0 ? transferList[filename].threadNum : 4;
    auto progresses = progressMap.value(filename, QVector<int>(threadCount, 0));
    if (chunkId >= 0 && chunkId < progresses.size())
    {
        progresses[chunkId] = percent;
        progressMap[filename] = progresses;
        //更新已下载的字节数 - 添加边界检查
        if (chunkId < transferList[filename].haveTrans.size()) {
            transferList[filename].haveTrans[chunkId] = haveSent;
        } else {
            qDebug() << "[WARNING] chunkId" << chunkId << "超出haveTrans数组范围，大小为" << transferList[filename].haveTrans.size();
        }
    }

    // 计算总的传输百分比
    int totalProgress = 0;
    for (int progress : progresses) {
        totalProgress += progress;
    }
    int averageProgress = progresses.size() > 0 ? totalProgress / progresses.size() : 0;

    // 找到该文件所在行
    int rows = model->rowCount();
    for (int r = 0; r < rows; ++r) {
        QModelIndex nameIdx = model->index(r, 0);
        if (nameIdx.data().toString() == filename) {
            // 更新已传输列为百分比（第2列）
            QModelIndex sentIndex = model->index(r, 1);
            model->setData(sentIndex, QString("%1%").arg(averageProgress));
            
            // 更新进度列数据（第4列）
            QModelIndex progressIndex = model->index(r, 3);
            model->setData(progressIndex, QVariant::fromValue(progresses));
            break;
        }
    }

    // 如果该 chunk 已完成，做一次完整性检查
    if (chunkSize > 0 && haveSent >= chunkSize) {
        markChunkComplete(filename, chunkId);
        checkFileComplete(filename);
    }
}
 
 void TransferWidget::markChunkComplete(const QString& filename, int chunkId)
 {
     if (!transferList.contains(filename)) return;
     int threadCount = transferList[filename].threadNum > 0 ? transferList[filename].threadNum : 4;
     auto progresses = progressMap.value(filename, QVector<int>(threadCount, 0));
     if (chunkId >= 0 && chunkId < progresses.size()) {
         progresses[chunkId] = 100;
         progressMap[filename] = progresses;
     }
     // 同步到模型
     int rows = model->rowCount();
     for (int r = 0; r < rows; ++r) {
         QModelIndex nameIdx = model->index(r, 0);
         if (nameIdx.data().toString() == filename) {
             QModelIndex progressIndex = model->index(r, 3);
             model->setData(progressIndex, QVariant::fromValue(progresses));
             break;
         }
     }
 }
 
 void TransferWidget::checkFileComplete(const QString& filename)
{
    if (!transferList.contains(filename))
        return;
    int threadCount = transferList[filename].threadNum > 0 ? transferList[filename].threadNum : 4;
    auto progresses = progressMap.value(filename, QVector<int>(threadCount, 0));
    bool allDone = true;
    for (int p : progresses) {
        if (p < 100) { allDone = false; break; }
    }
    if (!allDone) return;

    // 全部完成：设置已传输为文件大小，并标记为完成状态
    qint64 fileSize = transferList[filename].size;
    int rows = model->rowCount();
    for (int r = 0; r < rows; ++r)
    {
        QModelIndex nameIdx = model->index(r, 0);
        if (nameIdx.data().toString() == filename)
        {
            // 更新已传输列为100%
             model->setData(model->index(r, 1), QString("100%"));
            
            // 创建一个特殊的进度数据表示完成状态
            QVector<int> completedProgresses(threadCount, 100);
            QModelIndex progressIndex = model->index(r, 3);
            model->setData(progressIndex, QVariant::fromValue(completedProgresses));
            
            // 可以在这里添加完成状态的视觉提示
            // 例如改变行的背景色或添加完成图标
            QModelIndex statusIndex = model->index(r, 0); // 文件名列
            model->setData(statusIndex, QColor(144, 238, 144), Qt::BackgroundRole); // 浅绿色背景
            
            qDebug() << "文件传输完成: " << filename;
            break;
        }
    }
    //从列表在删除
    if(transferList.contains(filename))
        transferList.remove(filename);
}

void TransferWidget::addDowloadFile(QString fileName)
{

    auto transInfo = downloadList[fileName];

    transferList[fileName] = downloadList[fileName];
    transferList[fileName].haveTrans = QVector<qint64>(THREAD_NUM,0);

    QList<QStandardItem*> row;
    row << new QStandardItem(transInfo.filename)
        << new QStandardItem(0)
        << new QStandardItem(QString::number(transInfo.size))
        << new QStandardItem("0%");
    ;
    model->appendRow(row);

    // 初始化四线程进度为0
    QVector<int> progresses(transInfo.threadNum, 0);
    progressMap[transInfo.filename] = progresses;

    // 将进度写入第3列的模型数据，以便委托绘制
    int lastRow = model->rowCount() - 1;
    QModelIndex progressIndex = model->index(lastRow, 3);
    model->setData(progressIndex, QVariant::fromValue(progresses));
}

// void TransferWidget::updateDownloadProgress(const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite)
// {

//     if(!downloadList.contains(filename))
//     {
//          qDebug() << "[DEBUG]下载filename："<< filename<< "  " << downloadList.begin().key();
//         return;
//     }
//     // 计算百分比
//     int percent = 0;
//     if (chunkSize > 0)
//     {
//         percent = static_cast<int>(haveWrite * 100 / chunkSize);
//         if (percent > 100) percent = 100;
//         if (percent < 0) percent = 0;
//     }

//     // 更新本文件的四段进度
//     int threadCount = downloadList[filename].threadNum > 0 ? downloadList[filename].threadNum : 4;
//     auto progresses = downloadMap.value(filename, QVector<int>(threadCount, 0));
//     if (chunkId >= 0 && chunkId < progresses.size())
//     {
//         progresses[chunkId] = percent;
//         downloadMap[filename] = progresses;
//     }

//     // 计算总的传输百分比
//     int totalProgress = 0;
//     for (int progress : progresses) {
//         totalProgress += progress;
//     }
//     int averageProgress = progresses.size() > 0 ? totalProgress / progresses.size() : 0;

//     // 找到该文件所在行
//     int rows = model->rowCount();
//     for (int r = 0; r < rows; ++r) {
//         QModelIndex nameIdx = model->index(r, 0);
//         if (nameIdx.data().toString() == filename) {
//             // 更新已传输列为百分比（第2列）
//             QModelIndex sentIndex = model->index(r, 1);
//             model->setData(sentIndex, QString("%1%").arg(averageProgress));

//             // 更新进度列数据（第4列）
//             QModelIndex progressIndex = model->index(r, 3);
//             model->setData(progressIndex, QVariant::fromValue(progresses));
//             break;
//         }
//     }

//     // 如果该 chunk 已完成，做一次完整性检查
//     // if (chunkSize > 0 && haveWrite >= chunkSize) {
//     //     markChunkComplete(filename, chunkId);
//     //     checkFileComplete(filename);
//     // }
// }
