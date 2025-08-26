#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMutex>
#include <QTimer>
#include "filetrans.h"
#include "downloadtask.h"

// 下载任务状态枚举
enum class DownloadStatus {
    Pending,    // 等待中
    Downloading, // 下载中
    Paused,     // 已暂停
    Completed,  // 已完成
    Failed      // 失败
};

// 断点续传信息结构
struct ResumeInfo {
    QString filename;           // 文件名
    QString url;               // 下载URL
    QString savePath;          // 保存路径
    qint64 totalSize;          // 文件总大小
    QVector<qint64> chunkProgress; // 每个分片的下载进度
    QVector<qint64> chunkSizes;    // 每个分片的大小
    QDateTime lastModified;    // 最后修改时间
    DownloadStatus status;     // 下载状态
    int threadNum;             // 线程数
    QString tempFilePath;      // 临时文件路径
};

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(QObject *parent = nullptr);
    ~DownloadManager();

    // 初始化管理器
    void initialize();
    
    // 添加下载任务
    void addDownloadTask(const QString &filename, const QString &url, 
                        const QString &savePath, qint64 totalSize, int threadNum = 4);
    
    // 暂停下载任务
    void pauseDownload(const QString &filename);
    
    // 恢复下载任务
    void resumeDownload(const QString &filename);
    
    // 取消下载任务
    void cancelDownload(const QString &filename);
    
    // 获取下载进度
    int getDownloadProgress(const QString &filename);
    
    // 检查文件完整性
    bool verifyFileIntegrity(const QString &filename);
    
    // 清理已完成的任务
    void cleanupCompletedTasks();
    
    // 获取所有下载任务
    QMap<QString, ResumeInfo> getAllDownloads() const;

public slots:
    // 更新下载进度
    void updateProgress(const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite);
    
    // 处理下载完成
    void onDownloadComplete(const QString &filename, int chunkId);
    
    // 处理下载错误
    void onDownloadError(const QString &filename, int chunkId, const QString &error);

signals:
    // 下载进度更新信号
    void progressUpdated(const QString &filename, int progress);
    
    // 下载完成信号
    void downloadCompleted(const QString &filename);
    
    // 下载失败信号
    void downloadFailed(const QString &filename, const QString &error);
    
    // 下载暂停信号
    void downloadPaused(const QString &filename);

private slots:
    // 定时保存进度
    void saveProgressPeriodically();

private:
    // 保存下载信息到.temp文件
    void saveResumeInfo(const QString &filename);
    
    // 从.temp文件加载下载信息
    void loadResumeInfo();
    
    // 创建temp目录
    void createTempDirectory();
    
    // 获取.temp文件路径
    QString getTempFilePath(const QString &filename) const;
    
    // 启动下载任务
    void startDownloadTask(const QString &filename);
    
    // 检查下载是否完成
    void checkDownloadComplete(const QString &filename);
    
    // 计算文件MD5
    QString calculateFileMD5(const QString &filePath);

private:
    QMap<QString, ResumeInfo> m_downloads;     // 下载任务映射
    QMap<QString, QVector<DownloadTask*>> m_activeTasks; // 活跃的下载任务
    QString m_tempDir;                         // 临时目录路径
    mutable QMutex m_mutex;                   // 线程安全锁
    QTimer *m_saveTimer;                      // 定时保存计时器
    
    static const int SAVE_INTERVAL = 5000;   // 保存间隔(毫秒)
    static const QString TEMP_DIR_NAME;      // 临时目录名称
};

#endif // DOWNLOADMANAGER_H