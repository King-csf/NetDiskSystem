#ifndef FILEINFO_H
#define FILEINFO_H
#include <QString>
#include <QVector>

enum {
    FILE_TYPE_DIRECTORY = 0,
    FILE_TYPE_TXT,
    FILE_TYPE_VIDEO,
    FILE_TYPE_PICTURE,
    FILE_TYPE_OTHER
}FileType;

class FileInfo
{
public:
    FileInfo();

    QString fileName;
    QString fileSize;
    QString lastModTime;
    QString fileType;

    QVector<FileInfo> subFileList;
};

#endif // FILEINFO_H
