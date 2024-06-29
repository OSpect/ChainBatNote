#include "FileManager.h"
#include <QFile>
#include <QTextStream>

// FileManager构造函数
FileManager::FileManager() {}

// FileManager析构函数
FileManager::~FileManager() {}

// 创建文件
bool FileManager::createFile(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
        currentFile = fileName;
        return true;
    }
    return false;
}

// 打开文件
bool FileManager::openFile(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        fileContent = in.readAll();
        file.close();
        currentFile = fileName;
        return true;
    }
    return false;
}

// 保存文件
bool FileManager::saveFile(const QString &fileName, const QString &content) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << content;
        file.close();
        currentFile = fileName;
        return true;
    }
    return false;
}

// 删除文件
bool FileManager::deleteFile(const QString &fileName) {
    QFile file(fileName);
    return file.remove();
}

// 获取文件内容
QString FileManager::getFileContent() const {
    return fileContent;
}

// 获取当前文件名
QString FileManager::getCurrentFile() const {
    return currentFile;
}
