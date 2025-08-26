#include "../include/NetDiskServer.h"
#include <cstddef>
#include <cstdint>
#include <fcntl.h>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <unordered_set>
#include <workflow/WFTask.h>
#include <workflow/WFTaskFactory.h>
#include <cstring>
#include <errno.h>

uint64_t count = 0;

NetDiskServer::NetDiskServer()
{

  WFHttpServer server([this](WFHttpTask *task)
                      { dealUrl(task); });

  if (server.start(8888) == 0)
  {                 // start server on port 8888
    std::getchar(); // press "Enter" to end.
    server.stop();
  }
}

void NetDiskServer::dealUrl(WFHttpTask *task)
{

  std::string url = std::string(task->get_req()->get_request_uri());
  //std::cout << "url:" << url << std::endl;
  // 登录
  if ( url == "/login")
  {
    dealLogin(task);
  }
  else if (url.substr(0,12) == "/getFileList") 
  {
    dealSendFileList(task);
  }
  //新建文件夹
  else if(url == "/createDir")
  {
    dealCreateDir(task);
  }
  //上传文件
  else if(url.substr(0,7) == "/upload")
  {
    dealUploadFile(task);
  }
  //下载文件
  else if(url.substr(0,9) == "/download")
  {
    dealDownloadFile(task);
  }
  //获取下载文件信息
  else if( url.substr(0,16) == "/getDownloadInfo")
  {
    sendDownloadFileInfo(task);
  }
  //删除文件
  else if(url.substr(0,7) == "/delete")
  {
    dealDelFile(task);
  }
  else if(url.substr(0,7) == "/shared")
  {
    dealShareFile(task);
  }
  //结束上传，记录进度
  else if(url.substr(0,13) == "/resumeUpload")
  {
    dealResumeUpload(task);
  }
  else if(url.substr(0,8) == "/preview")
  {
    dealPreviewFile(task);
  }
}

void NetDiskServer::dealPreviewFile(WFHttpTask *task)
{
  auto req = task->get_req();
  auto resp = task->get_resp();
  
  std::string url = std::string(req->get_request_uri());
  // 从URL中提取文件名，格式: /preview/filename
  std::string filename = url.substr(9); // 去掉 "/preview/"
  
  // 构建完整的文件路径
  std::string filePath = "../storage/" + filename;
  
  // 检查文件是否存在
  if (!fs::exists(filePath)) {
    resp->set_status_code("404");
    resp->append_output_body("File not found");
    return;
  }
  
  // 获取文件大小
  auto fileSize = fs::file_size(filePath);
  
  // 打开文件
  int fd = open(filePath.c_str(), O_RDONLY);
  if (fd == -1) {
    resp->set_status_code("500");
    resp->append_output_body("Failed to open file");
    return;
  }
  
  // 设置响应头
  resp->set_status_code("200");
  resp->add_header_pair("Content-Type", "video/mp4");
  resp->add_header_pair("Content-Length", std::to_string(fileSize));
  resp->add_header_pair("Accept-Ranges", "bytes");
  resp->add_header_pair("Cache-Control", "no-cache");
  // 添加CORS头支持跨域访问
  resp->add_header_pair("Access-Control-Allow-Origin", "*");
  resp->add_header_pair("Access-Control-Allow-Methods", "GET, HEAD, OPTIONS");
  resp->add_header_pair("Access-Control-Allow-Headers", "Range");
  // 添加流媒体相关头
  resp->add_header_pair("Connection", "keep-alive");
  resp->add_header_pair("Transfer-Encoding", "identity");
  
  // 检查是否是Range请求（用于视频流播放）
  std::string rangeHeader = getHeader(req, "Range");
  std::cout << "Range : " << rangeHeader << std::endl;
  if (!rangeHeader.empty() && rangeHeader.substr(0, 6) == "bytes=") {
    // 处理Range请求
    std::string rangeValue = rangeHeader.substr(6);
    size_t dashPos = rangeValue.find('-');
    
    uint64_t start = 0;
    uint64_t end = fileSize - 1;
    
    if (dashPos != std::string::npos) {
      if (dashPos > 0) {
        start = std::stoull(rangeValue.substr(0, dashPos));
      }
      if (dashPos + 1 < rangeValue.length()) {
        end = std::stoull(rangeValue.substr(dashPos + 1));
      }
    }
    
    // 确保范围有效
    if (start >= fileSize) {
      close(fd);
      resp->set_status_code("416");
      resp->add_header_pair("Content-Range", "bytes */" + std::to_string(fileSize));
      return;
    }
    
    if (end >= fileSize) {
      end = fileSize - 1;
    }
    
    uint64_t contentLength = end - start + 1;
    
    // 设置206 Partial Content响应
    resp->set_status_code("206");
    resp->add_header_pair("Content-Range", 
      "bytes " + std::to_string(start) + "-" + std::to_string(end) + "/" + std::to_string(fileSize));
    resp->add_header_pair("Content-Length", std::to_string(contentLength));
    
    // 定位到起始位置
    if (lseek(fd, start, SEEK_SET) == -1) {
      close(fd);
      resp->set_status_code("500");
      resp->append_output_body("Failed to seek file");
      return;
    }
    
    // 读取并发送指定范围的数据
    const size_t bufferSize = 8 * 1024; // 8KB buffer for better streaming
    char buffer[bufferSize];
    uint64_t remaining = contentLength;
    
    while (remaining > 0) {
      size_t toRead = std::min(remaining, (uint64_t)bufferSize);
      ssize_t bytesRead = read(fd, buffer, toRead);
      
      if (bytesRead <= 0) {
        break;
      }
      
      resp->append_output_body(buffer, bytesRead);
      remaining -= bytesRead;
    }
  } else {
    // 发送完整文件
    const size_t bufferSize = 8 * 1024; // 8KB buffer for better streaming
    char buffer[bufferSize];
    
    while (true) {
      ssize_t bytesRead = read(fd, buffer, bufferSize);
      if (bytesRead <= 0) {
        break;
      }
      resp->append_output_body(buffer, bytesRead);
    }
  }
  
  close(fd);
}

void NetDiskServer::dealResumeUpload(WFHttpTask *task)
{
  auto req = task->get_req();
  std::string method = req->get_method();
  if(method == "GET")
    startResumeUpload(task);
  else
    endResumeUpload(task);
}

void NetDiskServer::generatJsonByUploadFile(json & j ,std::shared_ptr<UploadFile> uploadFile)
{
  std::cout << "run generatJsonByUploadFile" << std::endl;
  json jtemp;
  std::string filePath = uploadFile->fileName.substr(10);
  jtemp["filePath"] =  filePath; //../storage/
  auto pos = uploadFile->fileName.find_last_of("/");
  jtemp["fileName"] = uploadFile->fileName.substr(pos+1);
  jtemp["size"] = uploadFile->fileSize;
  jtemp["threadNum"] = uploadFile->chunkNum;
  jtemp["url"] = "http://192.168.111.200:8888/upload/"+filePath;

  for(auto & chunk : uploadFile->chunk)
  {
    jtemp["chunk_" + std::to_string(chunk.chunkId)] = chunk.haveUpload;

  }

  j.push_back(jtemp);
}

void NetDiskServer::addResumeUpTask(json j,std::string account)
{

  auto jurl = j["url"].get<std::string>();
  auto pos = jurl.find("/upload/") + 8;
  auto filePath = "../storage/" + jurl.substr(pos);

  auto size = j["size"].get<uint64_t>();
  auto chunkNum = j["threadNum"].get<int>();

  createUploadTask(filePath,std::to_string(size),std::to_string(chunkNum),jurl);

  for(auto& chunk:uploadList[filePath]->chunk)
  {
    if(j.contains("chunk_"+std::to_string(chunk.chunkId)))
    {
      std::cout<< "[DEBUG]chunkId: "  << chunk.chunkId << "  haveUpload: " << chunk.haveUpload << std::endl;
      chunk.haveUpload = j["chunk_"+std::to_string(chunk.chunkId)].get<uint64_t>();
      std::cout << "chunkid: " << chunk.chunkId 
              << "   chunkStart: " << chunk.chunkStart 
              << "   haveUpload: " << chunk.haveUpload
              <<"    chunkSize: " << chunk.chunkSize << std::endl;
    }
    else {
      chunk.haveUpload = 0;
    }
  }
}

void NetDiskServer::startResumeUpload(WFHttpTask *task)
{
  std::cout << "开始断点续传" << std::endl;
  auto req = task->get_req();

  std::string url = req->get_request_uri();
  std::string account = url.substr(14);

  json j = json::array();
  //上传列表仍有任务
  if(uploadUser.count(account) > 0)
  {
    for(auto & filePath : uploadUser[account])
    {
      if(uploadList.count(filePath) > 0)
      {
        generatJsonByUploadFile(j, uploadList[filePath]);
      }
    }
  }

  std::string query  = "select JsonInfo from fileResume where account='"+ account +"';";
  std::cout << "[DEBUG]" << query << std::endl;

  auto mysqlTask = WFTaskFactory::create_mysql_task(
    "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8", 5,
    [&j,task, account, this](WFMySQLTask *sqlTask) {
      auto resp = task->get_resp();

      if (sqlTask->get_state() == WFT_STATE_SUCCESS) 
      {
        auto sqlRes = sqlTask->get_resp();
        
        //请求:失败
        if(sqlRes->get_packet_type() == MYSQL_PACKET_ERROR)
        {
          auto httpRes = task->get_resp();
          return;
        }
        
        protocol::MySQLResultCursor cursor(sqlRes);
        //未查找到
        if(cursor.get_rows_count() == 0)
        {
          resp->set_status_code("500");
          return;
        }

        std::vector<protocol::MySQLCell> row;
        while (cursor.fetch_row(row)) 
        {
          std::string value = row[0].as_string();
          std::cout << "[DEBUG]" << value << std::endl;
          if(json::accept(value))
          {
            json temp = json::parse(value);
            addResumeUpTask(temp,account);
            j.push_back(temp);
          }
          
        }

        std::string query = "delete from fileResume where account = '" + account + "';";
        //删除记录
          auto delMysqlTask = WFTaskFactory::create_mysql_task(
              "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8",
              5, [](WFMySQLTask *sqlTask) {
                
              });
        delMysqlTask->get_req()->set_query(query);
        auto *series = series_of(task);    
        series->push_back(delMysqlTask);

        std::cout << "[DEBUG]json: " << j.dump() << std::endl;
        resp->append_output_body(j.dump());

      }
    });
          
  mysqlTask->get_req()->set_query(query);
  
  auto *series = series_of(task);    
  series->push_back(mysqlTask);

}

void NetDiskServer::endResumeUpload(WFHttpTask *task)
{
  auto req = task->get_req();
  std::string method = req->get_method();
  if(method == "GET")
    return;
  
  std::string url = req->get_request_uri();
  std::string account = url.substr(14); 
  
  // 检查account是否为空或无效
  if(account.empty() || account == "/") 
  {
    return;
  }

  const void *body = nullptr;
  size_t size = 0;
  if (req->get_parsed_body(&body, &size) && body && size > 0)
  {
    std::string content = static_cast<const char *>(body);
    json j = json::parse(content);
    std::string filePath = j["filePath"].get<std::string>();

    std::string query  = "insert into fileResume(account,path,JsonInfo) values('" 
                        + account +"','"+ filePath+"','" + j.dump() + "');";
                        std::cout << query << std::endl;
    {
      //清除列表上传任务
      std::string jurl = j["url"].get<std::string>();
      auto pos = j["url"].get<std::string>().find("/upload");
      std::string filePath = "../storage/" + jurl.substr(pos+8);
      //std::cout << "filePath : " << filePath << std::endl;
      std::lock_guard<std::mutex> lock{upmtx};
      if(uploadList.count(filePath) >0)
      { 
        std::cout << "[DEBUG]断点，关闭文件：" << filePath << std::endl;
        close(uploadList[filePath]->fd);
        uploadList.erase(filePath);
      }
    }
    //从用户列表删除
    {
      std::lock_guard<std::mutex> lock{upUsermtx};
      if(uploadUser[account].count(filePath) > 0)
        uploadUser[account].erase(filePath);
      if(uploadUser[account].size() == 0)
        uploadUser.erase(account);
    }
    //插入数据库
    auto mysqlTask = WFTaskFactory::create_mysql_task(
        "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8", 5,
        [task, account, this](WFMySQLTask *sqlTask) {
        std::cout << "插入数据库" << std::endl;
           //任务是否执行成功
        if (sqlTask->get_state() != WFT_STATE_SUCCESS)
        {
          std::cout << "MySQL连接失败" << std::endl;
          return;
        }
        
        auto sqlRes = sqlTask->get_resp();
        //检查SQL执行结果
        if(sqlRes->get_packet_type() == MYSQL_PACKET_ERROR)
        {
          std::cout << "SQL执行失败: " << sqlRes->get_error_msg() << std::endl;
          return;
        }
        
        std::cout << "数据插入成功" << std::endl;
        });

    mysqlTask->get_req()->set_query(query);

    auto *series = series_of(task);    
    series->push_back(mysqlTask);
    
  }

}

void NetDiskServer::createShareLink(WFHttpTask *task)
{
  auto req = task->get_req();
  auto resp = task->get_resp();
  //分享链接
  std::string shareLink = "http://192.168.111.200" + std::string(req->get_request_uri());
  //提取码
  std::string extractCode = generateExtractCode();

  std::string query = "insert into shareInfo(shareLink,extractCode) values('" + shareLink + "','" + extractCode + "');";

  auto mysqlTask = WFTaskFactory::create_mysql_task(
      "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8",
      5, [task,this,extractCode](WFMySQLTask *sqlTask)
      {
        if (sqlTask->get_state() != WFT_STATE_SUCCESS)
        {
          auto httpRes = task->get_resp();
         httpRes->set_status_code("500");
          return;
        }


        auto sqlRes = sqlTask->get_resp();
        
        //请求:失败
        if(sqlRes->get_packet_type() == MYSQL_PACKET_ERROR)
        {
          //std::cout << "qwe" << std::endl;
          auto httpRes = task->get_resp();
          httpRes->set_status_code("500");
          return;
        }
        
        auto httpRes = task->get_resp();
        httpRes->append_output_body(extractCode);
        
      });


  mysqlTask->get_req()->set_query(query);
  
  auto *series = series_of(task);    
  series->push_back(mysqlTask);
}

void NetDiskServer::copyShareFile(WFHttpTask *task)
{
  std::cout << "run copyShareFile" << std::endl;
  auto req = task->get_req();
  auto resp = task->get_resp();
  //保存路径
  std::string url = req->get_request_uri();
  std::string saveFilePath = "../storage" + url.substr(7);
  //源路径
  const void *body = nullptr;
  size_t size = 0;
  if (req->get_parsed_body(&body, &size) && body && size > 0)
  {
    
    std::string content((char *)body);
    std::cout << "content:  " << content << std::endl;
    json j = json::parse(content);
    std::string shareLink = j["shareLink"].get<std::string>();
    std::string extractCode = j["extractCode"].get<std::string>();
    auto pos = shareLink.find("/shared") + 8;
    std::string srcFilePath = "../storage/" + shareLink.substr(pos);
  
    std::cout << "源路径：" << srcFilePath << "   " << "目的路径：" << saveFilePath << std::endl;

    std::string query = "select * from shareInfo where shareLink = '" 
                          + shareLink + "' and extractCode =  '" + extractCode + "';";

    auto mysqlTask = WFTaskFactory::create_mysql_task(
      "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8",
      5, [task,this,srcFilePath,saveFilePath](WFMySQLTask *sqlTask)
      {
        if (sqlTask->get_state() != WFT_STATE_SUCCESS)
        {
          auto httpRes = task->get_resp();
         httpRes->set_status_code("500");
          return;
        }

        auto sqlRes = sqlTask->get_resp();
        
        //请求:失败
        if(sqlRes->get_packet_type() == MYSQL_PACKET_ERROR)
        {
          //std::cout << "qwe" << std::endl;
          auto httpRes = task->get_resp();
          httpRes->set_status_code("500");
          return;
        }
        
        protocol::MySQLResultCursor cursor(sqlRes);
        //查询到
        if(cursor.get_rows_count() != 0)
        {
         fs::copy(srcFilePath,saveFilePath);
        }
        else {
          auto httpRes = task->get_resp();
          httpRes->set_status_code("500");
        }
        

      });


  mysqlTask->get_req()->set_query(query);
  
  auto *series = series_of(task);    
  series->push_back(mysqlTask);
  }
}

void NetDiskServer::dealShareFile(WFHttpTask *task)
{
  auto req = task->get_req();
  std::string method = req->get_method();
  if(method == "GET")
    createShareLink(task);
  else
    copyShareFile(task);
}

void NetDiskServer::dealDelFile(WFHttpTask *task)
{
  auto req = task->get_req();
  auto resp = task->get_resp();
  //获取文件
  std::string url = req->get_request_uri();
  std::string filePath = "../storage" + url.substr(7);

  if(fs::exists(filePath))
  {
    //文件夹：递归删除
    if(fs::is_directory(filePath))
    {
      fs::remove_all(filePath);
    }
    else {
      fs::remove(filePath);
    }
  }
  else 
  {
    resp->set_status_code("404");
  }

}

void NetDiskServer::dealDownloadFile(WFHttpTask *task)
{
  auto req = task->get_req();
  auto resp = task->get_resp();
  std::string method = req->get_method();
  
  //获取文件
  std::string url = req->get_request_uri();
  std::string filePath = "../storage" + url.substr(9);
  
  // 检查文件是否存在
  if (!fs::exists(filePath) || fs::is_directory(filePath)) {
    resp->set_status_code("404");
    resp->append_output_body("File not found");
    return;
  }
  
  // 获取文件大小
  uint64_t fileSize = fs::file_size(filePath);
  
  // 检查是否有Range头
  std::string rangeHeader = getHeader(req, "range");
  std::cout << "range:" << rangeHeader << std::endl;
  uint64_t start = 0;
  uint64_t end = fileSize - 1;
  bool isRangeRequest = false;
  
  if (!rangeHeader.empty()) {
    isRangeRequest = true;
    // 解析Range头，格式: bytes=start-end
    if (rangeHeader.substr(0, 6) == "bytes=") {
      std::string range = rangeHeader.substr(6);
      size_t dashPos = range.find('-');
      
      if (dashPos != std::string::npos) {
        std::string startStr = range.substr(0, dashPos);
        std::string endStr = range.substr(dashPos + 1);
        
        if (!startStr.empty()) {
          start = std::stoull(startStr);
        }
        if (!endStr.empty()) {
          end = std::stoull(endStr);
        }
        
        // 验证范围
        if (start >= fileSize) {
          resp->set_status_code("416"); // Range Not Satisfiable
          resp->add_header_pair("Content-Range", "bytes */" + std::to_string(fileSize));
          return;
        }
        
        if (end >= fileSize) {
          end = fileSize - 1;
        }
      }
    }
  }
  
  // 计算内容长度
  uint64_t contentLength = end - start + 1;
  
  // 检查内容长度是否合理（限制最大为100MB）
  const uint64_t MAX_CHUNK_SIZE = 100 * 1024 * 1024; // 100MB
  if (contentLength > MAX_CHUNK_SIZE) {
    contentLength = MAX_CHUNK_SIZE;
    end = start + contentLength - 1;
  }
  
  // 检查内容长度是否为0
  if (contentLength == 0) {
    resp->set_status_code("416"); // Range Not Satisfiable
    resp->add_header_pair("Content-Range", "bytes */" + std::to_string(fileSize));
    return;
  }
  
  // 打开文件
  int fd = open(filePath.c_str(), O_RDONLY);
  if (fd < 0) {
    resp->set_status_code("500");
    resp->append_output_body("Internal Server Error");
    return;
  }
  
  // 设置响应头 有Range头
  if (isRangeRequest) 
  {
    resp->set_status_code("206"); // Partial Content
    resp->add_header_pair("Content-Range", 
      "bytes " + std::to_string(start) + "-" + std::to_string(end) + "/" + std::to_string(fileSize));
  } else 
  {
    resp->set_status_code("200");
  }
  
  resp->add_header_pair("Content-Length", std::to_string(contentLength));
  resp->add_header_pair("Accept-Ranges", "bytes");
  resp->add_header_pair("Content-Type", "application/octet-stream");
  
  // // 获取文件名用于下载
  // std::string fileName = fs::path(filePath).filename().string();
  // resp->add_header_pair("Content-Disposition", "attachment; filename=\"" + fileName + "\"");
  
  char *buf = new(std::nothrow) char[contentLength];
  if (!buf) {
    ::close(fd);
    resp->set_status_code("500");
    resp->append_output_body("Memory allocation failed");
    return;
  }
  auto readTask = WFTaskFactory::create_pread_task(fd,buf,contentLength,start,
    [fd, buf](WFFileIOTask *readTask ){
      int ret = readTask->get_retval();
            auto *httpTask = static_cast<WFHttpTask *>(series_of(readTask)->get_context());
            auto *resp = httpTask->get_resp();

            if (ret < 0) {
                /* 读文件失败 */
                resp->set_status_code("500");
                resp->append_output_body("Read file error");
                delete[] buf;  // 释放内存
            } else {
                /* 把读到的内容写进 HTTP body */
                resp->append_output_body_nocopy(buf, ret);
                // 注意：append_output_body_nocopy会接管buf的所有权，不需要手动delete
            }

            ::close(fd);
  });
  
  // 设置context，这样回调函数才能获取到httpTask
  series_of(task)->set_context(task);
  series_of(task)->push_back(readTask);
}

void NetDiskServer::sendDownloadFileInfo(WFHttpTask *task)
{
  auto req = task->get_req();
  auto resp = task->get_resp();
  //获取文件
  std::string url = req->get_request_uri();
  std::string filePath = "../storage" + url.substr(16);

  json j;
  //j["isDirectory"] = fs::is_directory(filePath) ? "true" : "false";
  //非文件夹
  if(!fs::is_directory(filePath))
  {
    j["isDirectory"] = "false";
    j["size"] = fs::file_size(filePath);
    resp->append_output_body(j.dump());
  }
  //文件夹
  else 
  {
    j["isDirectory"] = "true";
    j["files"] = json::parse(getUserFileName(filePath));
    resp->append_output_body(j.dump());
  }

}

void NetDiskServer::dealUploadFile(WFHttpTask * task)
{
  auto req = task->get_req();
  std::string method = std::string(req->get_method());
  //获取文件传输信息
  if(method == "GET")
  {
    getUploadFileInfo(task);
  }
  //接收文件
  else if(method == "POST")
  {
    recvUploadFile(task);
  }
}


void NetDiskServer::getUploadFileInfo(WFHttpTask * task)
{
  auto req = task->get_req();
  std::string url = req->get_request_uri();
  std::string filePath = "../storage" + url.substr(7);
  auto fileSize = getHeader(req,"file-size");
  auto chunkNum = getHeader(req,"thread-num");
  // std::cout << "fileSize :" << fileSize << std::endl
  //           << "chunkNum :" << chunkNum << std::endl;
  
  std::string query = "select JsonInfo from fileResume where path = '" + filePath + "';";
  //std::cout << query << std::endl;

  auto mysqlTask = WFTaskFactory::create_mysql_task(
      "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8",
      5, [task,this,filePath,fileSize,chunkNum,url](WFMySQLTask *sqlTask)
      {

        //任务是否执行成功
        if (sqlTask->get_state() != WFT_STATE_SUCCESS)
        {
          std::cout << "连接失败" << std::endl;
          auto httpRes = task->get_resp();
          httpRes->append_output_body("NO Record");
          return;
        }

        

        auto sqlRes = sqlTask->get_resp();
        
        //请求:失败
        if(sqlRes->get_packet_type() == MYSQL_PACKET_ERROR)
        {
          //std::cout << "qwe" << std::endl;
          auto httpRes = task->get_resp();
          httpRes->append_output_body("NO Record");
          return;
        }

        createUploadTask(filePath,fileSize,chunkNum,url);
        
        protocol::MySQLResultCursor cursor(sqlRes);
        //未查询到
        if(cursor.get_rows_count() == 0)
        {
          auto httpRes = task->get_resp();
          httpRes->append_output_body("NO Record");
          return;
        }
        else
        {
          std::vector<protocol::MySQLCell> row;
          cursor.fetch_row(row);
          std::string content = row[0].as_string();
          auto httpRes = task->get_resp();
          httpRes->append_output_body(content);
          return;
        }
      });


  mysqlTask->get_req()->set_query(query);
  
  auto *series = series_of(task);    
  series->push_back(mysqlTask);


}

void NetDiskServer::recvUploadFile(WFHttpTask * task)
{
  //std::cout << "run recvUploadFile" << std::endl;
  
  auto req = task->get_req();
  auto resp = task->get_resp();

  std::string url = req->get_request_uri();
  std::string filePath = "../storage" + url.substr(7);
  int chunkId = 0;

  std::shared_ptr<UploadFile> file;
  {
    std::lock_guard<std::mutex> lock{upmtx};
    if(uploadList.find(filePath) == uploadList.end()){
      //if(count % 500 == 0)
        std::cout   << "chunk : " << std::stoi(getHeader(req,"chunk-id"))
                 << "     uploadList不含" << filePath << std::endl;
      return;
    }
      
    file = uploadList[filePath];
  }

  if(!getHeader(req, "result").empty())
  {
    std::lock_guard<std::mutex> lock{upmtx};
    //close(file->fd);
    uploadList.erase(file->fileName);
    std::cout << "传输结束"  << std::endl;

    return;
  }

  try{
    chunkId = std::stoi(getHeader(req,"chunk-id"));
  }catch (const std::invalid_argument& e)
  {
    std::cout << "出错" << std::endl;
  }

  
  //写入文件
  const void *body = nullptr;
  size_t size = 0;
  if (req->get_parsed_body(&body, &size) && body && size > 0)
  {
    int fd = open(filePath.c_str(), O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        task->get_resp()->set_status_code("500");
        return;
    }
    off_t offset = file->chunk[chunkId].chunkStart + file->chunk[chunkId].haveUpload;
    file->chunk[chunkId].addHaveUploadSize(size);

    auto writeTask = WFTaskFactory::create_pwrite_task(fd, body, size, offset,
        [task, chunkId,fd](WFFileIOTask *t) 
        {
            close(fd);
            
        });
    series_of(task)->push_back(writeTask);
    
  }

  
}

void NetDiskServer::dealSendFileList(WFHttpTask *task)
{
  auto req = task->get_req();
  auto resp = task->get_resp();
  std::string url = std::string(task->get_req()->get_request_uri());
  std::string dirPath = "../storage" + url.substr(12);
  std::cout << "url：" << url << std::endl;
  std::cout << "文件夹：" << dirPath << std::endl;
  //把文件内容解析为JSON发送
  std::string sendContent = getUserFileName(dirPath);

  resp->append_output_body(sendContent);
}

void NetDiskServer::dealCreateDir(WFHttpTask * task)
{
  //获取请求体
  auto httpReq = task->get_req();
  const void *body = nullptr;
  size_t size = 0;
  if (!(httpReq->get_parsed_body(&body, &size) && body && size > 0))
  {
    auto httpRes = task->get_resp();
    httpRes->append_output_body("createDir failed");
    return;
  }
  //解析创建文件夹信息
  json j = json::parse(std::string((char *)(body)));
  std::string dirName = j["dirName"].get<std::string>();
  //std::cout << "dirName : " << dirName << std::endl;
  std::string dirPath = "../storage/" + dirName;
  
  // 使用create_directories支持创建嵌套目录
  std::error_code ec;
  bool res = fs::create_directories(dirPath, ec);
  
  // 检查目录是否已存在或创建成功
  if(!res && !fs::exists(dirPath))
  {
    auto httpRes = task->get_resp();
    httpRes->append_output_body("createDir failed: " + ec.message());
    return;
  }
  
  auto ftime = fs::last_write_time(dirPath);
  auto httpRes = task->get_resp();
  httpRes->append_output_body(FileOp::getFileLastModTime(ftime));

}


void NetDiskServer::dealLogin(WFHttpTask *task)
{
  std::cout << "run dealLogin" << std::endl;
  //获取请求体
  protocol::HttpRequest *httpReq = task->get_req();
  const void *body = nullptr;
  size_t size = 0;
  if (!(httpReq->get_parsed_body(&body, &size) && body && size > 0))
  {
    auto httpRes = task->get_resp();
    httpRes->append_output_body("login failed");
    return;
  }

  //解析登录信息
  json j = json::parse(std::string((char *)(body)));
  std::string account = j["account"].get<std::string>();
  std::string query = "select * from user where account = '" + account + "' and password = '" + j["password"].get<std::string>() + "';";
  std::cout << query << std::endl;


  auto mysqlTask = WFTaskFactory::create_mysql_task(
      "mysql://root:123456@127.0.0.1:3306/NetDisk?character_set=utf8",
      5, [task,account,this](WFMySQLTask *sqlTask)
      {

        //任务是否执行成功
        if (sqlTask->get_state() != WFT_STATE_SUCCESS)
        {
          std::cout << "连接失败" << std::endl;
          auto httpRes = task->get_resp();
          httpRes->append_output_body("login failed");
          return;
        }


        auto sqlRes = sqlTask->get_resp();
        
        //请求:失败
        if(sqlRes->get_packet_type() == MYSQL_PACKET_ERROR)
        {
          //std::cout << "qwe" << std::endl;
          auto httpRes = task->get_resp();
          httpRes->append_output_body("login failed");
          return;
        }
        
        protocol::MySQLResultCursor cursor(sqlRes);
        //未查询到
        if(cursor.get_rows_count() == 0)
        {
          auto httpRes = task->get_resp();
          httpRes->append_output_body("login failed");
          return;
        }
        else
        {
          //获取首页文件
          std::string userFile = getUserFileName("../storage/" + account);
          auto httpRes = task->get_resp();
          httpRes->append_output_body(userFile);
          return;
        }
      });


  mysqlTask->get_req()->set_query(query);
  
  auto *series = series_of(task);    
  series->push_back(mysqlTask);

}

std::string NetDiskServer::getUserFileName(std::string path)
{
  if(!fs::is_directory(path))
  {
    std::string ret = "NOT DIR";
    return ret;
  }
  //获取用户存储的文件夹下的文件
  //std::cout << "filePath: " << filePath << std::endl;
   
  json res = json::array();

    for (auto &entry : fs::directory_iterator(path))
    {
      json j;

      auto fileName = entry.path().filename();
      j["filename"] = fileName;

      if(!entry.is_directory())
      {
        auto size = entry.file_size();
        j["size"] = std::to_string(size);
      }
      else
      {
        j["size"] = "-";
      }

      //上次修改时间
      fs::file_time_type ftime = fs::last_write_time(entry);
      std::string timeStr = FileOp::getFileLastModTime(ftime);
      j["time"] = timeStr;

      res.push_back(j);
    }


    return res.dump();
  
}

bool NetDiskServer::loginCheck(WFHttpTask */*task*/)
{
  // mysql://用户名:密码@主机:端口/数据库名称?character_set=charset

  return true;
}

NetDiskServer::~NetDiskServer() {}

std::string NetDiskServer::getHeader(protocol::HttpRequest *httpReq,std::string header)
{
    auto parser = httpReq->get_parser();

    // 初始化头部迭代器
    http_header_cursor_t cursor;
    http_header_cursor_init(&cursor, parser); // 初始化迭代器指向头部链表

    // 存储头部字段的指针和长度
    const void *header_name = nullptr;
    size_t name_len = 0;
    const void *header_value = nullptr;
    size_t value_len = 0;

    std::string target_value;

    // 遍历所有头部字段
    while (http_header_cursor_next(&header_name, &name_len,
                                   &header_value, &value_len, &cursor) == 0)
    {
        std::string current_name(static_cast<const char *>(header_name), name_len);
        //std::cout << "header : " << current_name << "  " << std::string((char *)header_value);
        if (current_name == header)
        {
            //std::cout << "header : " << current_name << "  " << std::string((char *)header_value);
            target_value = std::string(static_cast<const char *>(header_value), value_len);
            break; 
        }
    }

    return target_value;

}

void NetDiskServer::createUploadTask(std::string filePath,std::string fileSize,std::string chunkNum,std::string url)
{
  std::cout << "run createUploadTask" << std::endl;
 // std::cout << fileSize << " " << chunkNum << std::endl;
 //添加用户列表
  auto pos =url.find("/");
  std::string account = url.substr(0,pos+1);
  {
    std::lock_guard<std::mutex> lock{upUsermtx};
    if(uploadUser.count(account) == 0)
      uploadUser[account] = std::unordered_set<std::string>();
  }

  // 检查上传列表是否已存在该文件
  {
    std::lock_guard<std::mutex> lock{upmtx};
    if (uploadList.find(filePath) != uploadList.end())
    {
      std::cout << "上传任务已存在: " << filePath << std::endl;
      return;
    }
  }
  
  std::cout << "创建上传任务：" << filePath << std::endl;
  auto uploadFile = std::make_shared<UploadFile>();
  
  try
  {
    uploadFile->chunkNum = std::stoi(chunkNum);
    uploadFile->fileSize = std::stoi(fileSize);
  }
  catch(std::invalid_argument& e)
  {
    std::cout << "参数解析错误"<< '\n';
    return;
  }
  
  uploadFile->fileName = filePath;
  uploadFile->chunkSize = uploadFile->fileSize / uploadFile->chunkNum;
  uploadFile->isEnd = false;
  uploadFile->isError = false;
  
  // 创建文件所在的目录结构
  std::filesystem::path filePathObj(filePath);
  std::filesystem::path parentDir = filePathObj.parent_path();
  if (!parentDir.empty() && !std::filesystem::exists(parentDir)) 
  {
    std::filesystem::create_directories(parentDir);
  }
  
  // bool isExist = false;
  // if(fs::exists(filePath))
  //   isExist = true;

  // //打开文件（在锁外进行文件操作）
  // std::cout << "开始打开文件: " << filePath << std::endl;
  // int flags = O_RDWR | O_CREAT;
  // if(!isExist){
  //   flags |= O_TRUNC;  // 只在新文件时截断
  // }
  // int fd = open(filePath.c_str(),flags , 0644);
  // if (fd == -1) 
  // {
  //     std::cout << "文件打开失败: " << filePath << ", 错误: " << strerror(errno) << std::endl;
  //     return;
  // }
  // uploadFile->fd = fd;
  
  // if (!isExist) 
  // {
  //   std::cout << "文件打开成功，开始截断文件，大小: " << uploadFile->fileSize << std::endl;
  //   if (ftruncate(fd, uploadFile->fileSize) == -1) 
  //   {
  //     std::cout << "文件截断失败: " << filePath << ", 错误: " << strerror(errno)
  //               << std::endl;
  //     close(fd);
  //     return;
  //   }
  // }

  //std::cout << "文件创建成功: " << filePath << ", fd: " << fd << ", 大小: " << uploadFile->fileSize << std::endl;

  // 初始化chunk信息
  for(uint64_t i = 0; i < uploadFile->chunkNum;i++)
  {
    FileChunk fileChunk(i,uploadFile->chunkSize);
    if(i == uploadFile->chunkNum - 1)
      fileChunk.chunkSize = uploadFile->fileSize - i*uploadFile->chunkSize;
    std::cout << "chunkid: " << i 
              << "   chunkStart: " << fileChunk.chunkStart 
              <<"    chunkSize: " << fileChunk.chunkSize << std::endl;
    uploadFile->chunk.push_back(std::move(fileChunk));
  }

  // 文件操作完成后，再加锁添加到上传列表
  {
    std::lock_guard<std::mutex> lock{upmtx};
    // 再次检查，防止并发情况下重复创建
    if (uploadList.find(filePath) == uploadList.end())
    {
      this->uploadList[filePath] = uploadFile;
      uploadUser[account].insert(filePath);
      std::cout << "上传任务添加到列表: " << filePath << std::endl;
    }
    else
    {
      std::cout << "上传任务已存在，关闭文件: " << filePath << std::endl;
      //close(fd);
    }
  }
}


std::string NetDiskServer::generateExtractCode(int len)
{
  static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_int_distribution<std::size_t> dist(0, sizeof(charset) - 2);

    std::string code;
    code.reserve(len);
    for (std::size_t i = 0; i < len; ++i)
        code += charset[dist(rng)];

    return code;
}