#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include "filetrans.h"


#define THREAD_NUM 4
#define BUFFER_SIZE 262144
#define SERVER_ADDR "http://192.168.111.200:8888"

//下载列表
extern QMap<QString,TransferInfo> downloadList;
extern QMap<QString, QVector<int>> downloadMap;
extern bool isExit; //是否退出

#endif // CONFIG_H
