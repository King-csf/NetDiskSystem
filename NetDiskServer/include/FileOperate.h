#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <mutex>
#include <fcntl.h>

namespace  fs = std::filesystem;

class FileChunk
{
public:
    FileChunk(int chunkId, uint64_t chunkSize,uint64_t haveUpload = 0) : 
    chunkId(chunkId), haveUpload(haveUpload), chunkSize(chunkSize), isError(false), isEnd(false) 
    {
        chunkStart = chunkId * chunkSize;
    }
    
    // 移动构造函数
    FileChunk(FileChunk&& other) noexcept : 
    chunkId(other.chunkId), haveUpload(other.haveUpload), chunkStart(other.chunkStart), 
    chunkSize(other.chunkSize), isError(other.isError), isEnd(other.isEnd) {}
    
    // 移动赋值运算符
    FileChunk& operator=(FileChunk&& other) noexcept {
        if (this != &other) {
            chunkId = other.chunkId;
            haveUpload = other.haveUpload;
            chunkStart = other.chunkStart;
            chunkSize = other.chunkSize;
            isError = other.isError;
            isEnd = other.isEnd;
        }
        return *this;
    }
    
    // 删除拷贝构造函数和拷贝赋值运算符
    FileChunk(const FileChunk&) = delete;
    FileChunk& operator=(const FileChunk&) = delete;
    int chunkId;
    uint64_t haveUpload;
    uint64_t chunkStart;
    uint64_t chunkSize;
    bool isError;
    bool isEnd;
    std::mutex mtx;

    void addHaveUploadSize(uint64_t uploadSize);
    void setIsError(bool isError);
    void setIsEnd(bool isEnd);

    bool getIsEnd(){ std::lock_guard<std::mutex> lock{mtx}; return isEnd;}
    bool getIsError(){ std::lock_guard<std::mutex> lock{mtx}; return isError;}
    uint64_t getUpLoadSize(){ std::lock_guard<std::mutex> lock{mtx}; return haveUpload;}
    //std::vector<std::shared_ptr<char []>> data;
};

class UploadFile
{
public:
    int fd;
    uint64_t fileSize;
    uint64_t chunkNum;
    uint64_t chunkSize;
    std::string fileName;
    bool isEnd;
    bool isError;
    std::vector<FileChunk> chunk;
    std::mutex mtx;
    std::mutex IOmtx;

    void setIsEnd(bool isEnd);
    void setIsError(bool isError);
    bool getIsEnd(){ std::lock_guard<std::mutex> lock{mtx}; return isEnd;}
    bool getIsError(){ std::lock_guard<std::mutex> lock{mtx}; return isError;}
};

class FileOp
{
public:
    static std::string getFileLastModTime(fs::file_time_type& ftime);
};