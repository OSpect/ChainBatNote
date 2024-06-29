#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>

// FileManager类，负责文件管理的基本结构和功能声明
class FileManager {
public:
    FileManager();    // 构造函数
    ~FileManager();   // 析构函数

    bool createFile(const QString &fileName);  // 创建文件
    bool openFile(const QString &fileName);    // 打开文件
    bool saveFile(const QString &fileName, const QString &content);  // 保存文件
    bool deleteFile(const QString &fileName);  // 删除文件

    QString getFileContent() const;  // 获取文件内容
    QString getCurrentFile() const;  // 获取当前文件名

private:
    QString currentFile;   // 当前文件名
    QString fileContent;   // 文件内容
};

#endif // FILEMANAGER_H
