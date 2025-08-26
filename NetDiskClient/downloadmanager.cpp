#include "downloadmanager.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QThreadPool>
#include <QApplication>
#include "config.h"

const QString DownloadManager::TEMP_DIR_NAME = "temp";

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
    , m_saveTimer(new QTimer(this))
{
    // 设置定时保存
    m_saveTimer->setInterval(SAVE_INTERVAL);
    connect(m_saveTimer, &QTimer::timeout, this, &DownloadManager::saveProgressPeriodically);
}

DownloadManager::~DownloadManager()
{
    // 保存所有进度
    for (auto it = m_downloads.begin(); it != m_downloads.end(); ++it) {
        if (it->status == DownloadStatus::Downloading || it->status == DownloadStatus::Paused) {
            saveResumeInfo(it.key());
        }
    }
    
    // 清理活跃任务
    for (auto &tasks : m_activeTasks) {
        for (auto task : tasks) {
            if (task) {
                task->deleteLater();
            }
        }
    }
}

void DownloadManager::initialize()
{
    createTempDirectory();
    loadResumeInfo();
    m_saveTimer->start();
    
    qDebug() << "DownloadManager initialized. Found" << m_downloads.size() << "pending downloads.";
}

void DownloadManager::createTempDirectory()
{
    QString appDir = QApplication::applicationDirPath();
    m_tempDir = appDir + "/" + TEMP_DIR_NAME;
    
    QDir dir;
    if (!dir.exists(m_tempDir)) {
        if (dir.mkpath(m_tempDir)) {
            qDebug() << "Created temp directory:" << m_tempDir;
        } else {
            qDebug() << "Failed to create temp directory:" << m_tempDir;
        }
    }
}

QString DownloadManager::getTempFilePath(const QString &filename) const
{
    return m_tempDir + "/" + filename + ".temp";
}

void DownloadManager::addDownloadTask(const QString &filename, const QString &url,
                                    const QString &savePath, qint64 totalSize, int threadNum)
{
    QMutexLocker locker(&m_mutex);
    
    ResumeInfo info;
    info.filename = filename;
    info.url = url;
    info.savePath = savePath;
    info.totalSize = totalSize;
    info.threadNum = threadNum;
    info.status = DownloadStatus::Pending;
    info.lastModified = QDateTime::currentDateTime();
    info.tempFilePath = getTempFilePath(filename);
    
    // 初始化分片信息
    qint64 chunkSize = totalSize / threadNum;
    for (int i = 0; i < threadNum; ++i) {
        if (i == threadNum - 1) {
            info.chunkSizes.append(totalSize - i * chunkSize);
        } else {
            info.chunkSizes.append(chunkSize);
        }
        info.chunkProgress.append(0);
    }
    
    m_downloads[filename] = info;
    saveResumeInfo(filename);
    
    // 立即开始下载
    startDownloadTask(filename);
}

void DownloadManager::startDownloadTask(const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    ResumeInfo &info = m_downloads[filename];
    info.status = DownloadStatus::Downloading;
    info.lastModified = QDateTime::currentDateTime();
    
    QVector<DownloadTask*> tasks;
    
    for (int i = 0; i < info.threadNum; ++i) {
        // 检查该分片是否已完成
        if (info.chunkProgress[i] >= info.chunkSizes[i]) {
            continue;
        }
        
        FileChunk chunk;
        chunk.chunkId = i;
        chunk.filename = filename;
        chunk.url = info.url;
        chunk.filePath = info.savePath;
        chunk.chunkStart = (info.totalSize / info.threadNum) * i + info.chunkProgress[i];
        chunk.chunkSize = info.chunkSizes[i] - info.chunkProgress[i];
        chunk.size = info.totalSize;
        
        auto task = new DownloadTask(chunk, info.savePath);
        
        // 连接信号
        connect(task, &DownloadTask::haveWriteSig, this, &DownloadManager::updateProgress);
        connect(task, &DownloadTask::downloadError, this, &DownloadManager::onDownloadError);
        connect(task, &DownloadTask::downloadPaused, this, [this, filename](const QString &, int chunkId) {
            qDebug() << "Chunk" << chunkId << "paused for" << filename;
            // 立即保存进度
            saveResumeInfo(filename);
        });
        connect(task, &DownloadTask::downloadCancelled, this, [this, filename](const QString &, int chunkId) {
            qDebug() << "Chunk" << chunkId << "cancelled for" << filename;
            // 取消整个下载任务
            cancelDownload(filename);
        });
        
        tasks.append(task);
        QThreadPool::globalInstance()->start(task);
    }
    
    m_activeTasks[filename] = tasks;
    
    // 立即保存初始状态
    saveResumeInfo(filename);
    
    qDebug() << "Started download task for:" << filename << "with" << tasks.size() << "active chunks";
}

void DownloadManager::pauseDownload(const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    m_downloads[filename].status = DownloadStatus::Paused;
    m_downloads[filename].lastModified = QDateTime::currentDateTime();
    
    // 停止活跃任务（注意：QRunnable任务一旦启动很难中途停止，这里只是标记状态）
    if (m_activeTasks.contains(filename)) {
        m_activeTasks.remove(filename);
    }
    
    saveResumeInfo(filename);
    emit downloadPaused(filename);
    
    qDebug() << "Paused download:" << filename;
}

void DownloadManager::resumeDownload(const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    if (m_downloads[filename].status == DownloadStatus::Paused) {
        startDownloadTask(filename);
        qDebug() << "Resumed download:" << filename;
    }
}

void DownloadManager::cancelDownload(const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    // 删除临时文件
    QString tempFile = getTempFilePath(filename);
    QFile::remove(tempFile);
    
    // 停止活跃任务
    if (m_activeTasks.contains(filename)) {
        m_activeTasks.remove(filename);
    }
    
    // 删除下载记录
    m_downloads.remove(filename);
    
    qDebug() << "Cancelled download:" << filename;
}

int DownloadManager::getDownloadProgress(const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return 0;
    }
    
    const ResumeInfo &info = m_downloads[filename];
    qint64 totalProgress = 0;
    qint64 totalSize = 0;
    
    for (int i = 0; i < info.chunkProgress.size(); ++i) {
        totalProgress += info.chunkProgress[i];
        totalSize += info.chunkSizes[i];
    }
    
    return totalSize > 0 ? (int)(totalProgress * 100 / totalSize) : 0;
}

void DownloadManager::updateProgress(const QString &filename, int chunkId, qint64 chunkSize, qint64 haveWrite)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    ResumeInfo &info = m_downloads[filename];
    
    if (chunkId >= 0 && chunkId < info.chunkProgress.size()) {
        qint64 oldProgress = info.chunkProgress[chunkId];
        info.chunkProgress[chunkId] = haveWrite;
        info.lastModified = QDateTime::currentDateTime();
        
        // 检查该分片是否完成
        if (haveWrite >= chunkSize) {
            onDownloadComplete(filename, chunkId);
        }
        
        // 每当进度有显著变化时保存（每1MB或每10%）
        qint64 progressDiff = haveWrite - oldProgress;
        if (progressDiff >= 1024 * 1024 || // 每1MB
            (chunkSize > 0 && (haveWrite * 100 / chunkSize) % 10 == 0 && (oldProgress * 100 / chunkSize) % 10 != 0)) { // 每10%
            saveResumeInfo(filename);
        }
        
        // 发送进度更新信号
        int progress = getDownloadProgress(filename);
        emit progressUpdated(filename, progress);
        
        // 检测异常情况：如果进度长时间没有更新，可能需要暂停
        static QMap<QString, QDateTime> lastProgressTime;
        lastProgressTime[filename] = QDateTime::currentDateTime();
    }
}

void DownloadManager::onDownloadComplete(const QString &filename, int chunkId)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    qDebug() << "Chunk" << chunkId << "completed for" << filename;
    
    // 检查所有分片是否都完成
    checkDownloadComplete(filename);
}

void DownloadManager::checkDownloadComplete(const QString &filename)
{
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    const ResumeInfo &info = m_downloads[filename];
    bool allComplete = true;
    
    for (int i = 0; i < info.chunkProgress.size(); ++i) {
        if (info.chunkProgress[i] < info.chunkSizes[i]) {
            allComplete = false;
            break;
        }
    }
    
    if (allComplete) {
        // 进行文件完整性校验
        if (verifyFileIntegrity(filename)) {
            m_downloads[filename].status = DownloadStatus::Completed;
            m_downloads[filename].lastModified = QDateTime::currentDateTime();
            
            // 删除临时文件
            QString tempFile = getTempFilePath(filename);
            QFile::remove(tempFile);
            
            // 清理活跃任务
            if (m_activeTasks.contains(filename)) {
                m_activeTasks.remove(filename);
            }
            
            emit downloadCompleted(filename);
            qDebug() << "Download completed and verified:" << filename;
        } else {
            // 校验失败，标记为失败状态
            m_downloads[filename].status = DownloadStatus::Failed;
            m_downloads[filename].lastModified = QDateTime::currentDateTime();
            saveResumeInfo(filename);
            
            emit downloadFailed(filename, "File integrity verification failed");
            qDebug() << "Download completed but verification failed:" << filename;
        }
    }
}

void DownloadManager::onDownloadError(const QString &filename, int chunkId, const QString &error)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    qDebug() << "Download error for" << filename << "chunk" << chunkId << ":" << error;
    
    // 暂停下载并保存状态
    m_downloads[filename].status = DownloadStatus::Failed;
    m_downloads[filename].lastModified = QDateTime::currentDateTime();
    saveResumeInfo(filename);
    
    emit downloadFailed(filename, error);
}

void DownloadManager::saveResumeInfo(const QString &filename)
{
    if (!m_downloads.contains(filename)) {
        return;
    }
    
    const ResumeInfo &info = m_downloads[filename];
    QString tempFile = getTempFilePath(filename);
    
    QJsonObject json;
    json["filename"] = info.filename;
    json["url"] = info.url;
    json["savePath"] = info.savePath;
    json["totalSize"] = info.totalSize;
    json["threadNum"] = info.threadNum;
    json["status"] = static_cast<int>(info.status);
    json["lastModified"] = info.lastModified.toString(Qt::ISODate);
    
    QJsonArray chunkProgressArray;
    for (qint64 progress : info.chunkProgress) {
        chunkProgressArray.append(progress);
    }
    json["chunkProgress"] = chunkProgressArray;
    
    QJsonArray chunkSizesArray;
    for (qint64 size : info.chunkSizes) {
        chunkSizesArray.append(size);
    }
    json["chunkSizes"] = chunkSizesArray;
    
    QJsonDocument doc(json);
    
    QFile file(tempFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Failed to save resume info for:" << filename;
    }
}

void DownloadManager::loadResumeInfo()
{
    QDir tempDir(m_tempDir);
    QStringList tempFiles = tempDir.entryList(QStringList() << "*.temp", QDir::Files);
    
    for (const QString &tempFileName : tempFiles) {
        QString fullPath = tempDir.absoluteFilePath(tempFileName);
        QFile file(fullPath);
        
        if (!file.open(QIODevice::ReadOnly)) {
            continue;
        }
        
        QByteArray data = file.readAll();
        file.close();
        
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "Failed to parse temp file:" << tempFileName;
            continue;
        }
        
        QJsonObject json = doc.object();
        
        ResumeInfo info;
        info.filename = json["filename"].toString();
        info.url = json["url"].toString();
        info.savePath = json["savePath"].toString();
        info.totalSize = json["totalSize"].toVariant().toLongLong();
        info.threadNum = json["threadNum"].toInt();
        info.status = static_cast<DownloadStatus>(json["status"].toInt());
        info.lastModified = QDateTime::fromString(json["lastModified"].toString(), Qt::ISODate);
        info.tempFilePath = fullPath;
        
        QJsonArray chunkProgressArray = json["chunkProgress"].toArray();
        for (const QJsonValue &value : chunkProgressArray) {
            info.chunkProgress.append(value.toVariant().toLongLong());
        }
        
        QJsonArray chunkSizesArray = json["chunkSizes"].toArray();
        for (const QJsonValue &value : chunkSizesArray) {
            info.chunkSizes.append(value.toVariant().toLongLong());
        }
        
        // 只加载未完成的下载
        if (info.status != DownloadStatus::Completed) {
            m_downloads[info.filename] = info;
            qDebug() << "Loaded resume info for:" << info.filename;
        } else {
            // 删除已完成的临时文件
            QFile::remove(fullPath);
        }
    }
}

void DownloadManager::saveProgressPeriodically()
{
    QMutexLocker locker(&m_mutex);
    
    for (auto it = m_downloads.begin(); it != m_downloads.end(); ++it) {
        if (it->status == DownloadStatus::Downloading) {
            saveResumeInfo(it.key());
        }
    }
}

bool DownloadManager::verifyFileIntegrity(const QString &filename)
{
    if (!m_downloads.contains(filename)) {
        qDebug() << "File not found in downloads:" << filename;
        return false;
    }
    
    const ResumeInfo &info = m_downloads[filename];
    QFileInfo fileInfo(info.savePath);
    
    // 检查文件是否存在
    if (!fileInfo.exists()) {
        qDebug() << "Downloaded file does not exist:" << info.savePath;
        return false;
    }
    
    // 检查文件大小是否正确
    qint64 actualSize = fileInfo.size();
    if (actualSize != info.totalSize) {
        qDebug() << "File size mismatch for" << filename 
                 << "Expected:" << info.totalSize 
                 << "Actual:" << actualSize;
        return false;
    }
    
    // 检查文件是否可读
    QFile file(info.savePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for verification:" << info.savePath;
        return false;
    }
    
    // 验证文件内容的连续性（检查是否有空洞）
    const qint64 bufferSize = 64 * 1024; // 64KB buffer
    QByteArray buffer(bufferSize, 0);
    qint64 totalRead = 0;
    
    while (totalRead < info.totalSize) {
        qint64 toRead = qMin(bufferSize, info.totalSize - totalRead);
        qint64 bytesRead = file.read(buffer.data(), toRead);
        
        if (bytesRead != toRead) {
            qDebug() << "File read error during verification at position" << totalRead;
            file.close();
            return false;
        }
        
        // 检查是否有大量的零字节（可能表示文件有空洞）
        int zeroCount = 0;
        for (int i = 0; i < bytesRead; ++i) {
            if (buffer[i] == 0) {
                zeroCount++;
            }
        }
        
        // 如果超过90%是零字节，可能有问题（除非文件本身就是这样）
        if (zeroCount > bytesRead * 0.9 && bytesRead > 1024) {
            qDebug() << "Warning: Large amount of zero bytes detected at position" << totalRead;
            // 注意：这里只是警告，不一定是错误，因为文件可能本身包含大量零字节
        }
        
        totalRead += bytesRead;
    }
    
    file.close();
    
    // 可以在这里添加MD5或SHA256校验
    // QString fileMD5 = calculateFileMD5(info.savePath);
    // if (!fileMD5.isEmpty() && info.expectedMD5 != fileMD5) {
    //     qDebug() << "MD5 checksum mismatch for" << filename;
    //     return false;
    // }
    
    qDebug() << "File integrity verification passed for:" << filename;
    return true;
}

QString DownloadManager::calculateFileMD5(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(&file);
    return hash.result().toHex();
}

void DownloadManager::cleanupCompletedTasks()
{
    QMutexLocker locker(&m_mutex);
    
    auto it = m_downloads.begin();
    while (it != m_downloads.end()) {
        if (it->status == DownloadStatus::Completed) {
            QString tempFile = getTempFilePath(it.key());
            QFile::remove(tempFile);
            it = m_downloads.erase(it);
        } else {
            ++it;
        }
    }
}

QMap<QString, ResumeInfo> DownloadManager::getAllDownloads() const
{
    QMutexLocker locker(&m_mutex);
    return m_downloads;
}