#pragma once
#include <iostream>
#include "workflow/WFHttpServer.h"
#include "workflow/WFTaskFactory.h"
#include "workflow/MySQLResult.h"
#include "workflow/MySQLUtil.h"
#include "workflow/HttpUtil.h"
#include "workflow/http_parser.h"
#include "workflow/WFFacilities.h"
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
//#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>
#include "FileOperate.h"
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include  <mutex>
#include <unistd.h>
#include <random>

using json = nlohmann::json;
namespace  fs = std::filesystem;

#define SERVER_ADD "http://192.168.111.200"


class NetDiskServer
{
public:
    NetDiskServer();
    ~NetDiskServer();

    std::mutex upmtx; //上传列表锁
    std::unordered_map<std::string, std::shared_ptr<UploadFile>> uploadList;
    std::mutex upUsermtx; 
    std::unordered_map<std::string, std::unordered_set<std::string>> uploadUser;

    //根据URL处理请求
    void dealUrl(WFHttpTask *task);
    void dealLogin(WFHttpTask *task);
    void dealCreateDir(WFHttpTask *task);
    void dealUploadFile(WFHttpTask *task);
    void dealSendFileList(WFHttpTask *task);
    void dealDelFile(WFHttpTask *task);
    void dealShareFile(WFHttpTask *task);
    void dealPreviewFile(WFHttpTask *task);

    void dealResumeUpload(WFHttpTask *task);
    void endResumeUpload(WFHttpTask *task); //客户端退出，记录传输进度
    void startResumeUpload(WFHttpTask *task); //客户端启动，继续上次传输
    void generatJsonByUploadFile(json & j ,std::shared_ptr<UploadFile> uploadFile); //根据UploadFile生成json
    void addResumeUpTask(json j,std::string account); //添加上传断点续传任务

    void createShareLink(WFHttpTask *task);
    void copyShareFile(WFHttpTask *task);

    void getUploadFileInfo(WFHttpTask *task);
    void recvUploadFile(WFHttpTask *task);
    void dealDownloadFile(WFHttpTask *task);
    void sendDownloadFileInfo(WFHttpTask *task);
    //检查登录信息
    bool loginCheck(WFHttpTask *task);
    static void mysqlCallback(WFMySQLTask *task);
    std::string getUserFileName(std::string path);
    //生成4位随机提取码
    std::string generateExtractCode(int len = 4);
    std::string getHeader(protocol::HttpRequest *httpReq,std::string header);
    void createUploadTask(std::string filePath,std::string fileSize,std::string chunkNum,std::string url);
};



