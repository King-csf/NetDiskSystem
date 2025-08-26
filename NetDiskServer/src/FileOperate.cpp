#include "../include/FileOperate.h"
#include <mutex>


std::string FileOp::getFileLastModTime(fs::file_time_type& ftime)
{
    // 转换为 system_clock 的时间点
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
    );

    // 转换为 time_t
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

    // 格式化为字符串
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tt), "%Y-%m-%d");
    return ss.str();
}

void FileChunk::addHaveUploadSize(uint64_t uploadSize)
{
    std::lock_guard<std::mutex> lock{mtx};
    this->haveUpload += uploadSize;
}
void FileChunk::setIsError(bool isError)
{
    std::lock_guard<std::mutex> lock{mtx};
    this->isError = isError;
}
void FileChunk::setIsEnd(bool isEnd)
{
    std::lock_guard<std::mutex> lock{mtx};
    this->isEnd = isEnd;
}

void UploadFile::setIsEnd(bool isEnd)
{
    std::lock_guard<std::mutex> lock{mtx};
    this->isEnd = isEnd;
}
void UploadFile::setIsError(bool isError) 
{
    std::lock_guard<std::mutex> lock{mtx};
    this->isError = isError;
}