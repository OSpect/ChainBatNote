#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QMap>
#include <QSet>
#include <QString>

class MainWindow;

class WindowManager {
public:
    static WindowManager& instance();
    void createWindow(const QString &fileName = "");

    bool hasWindowRecord(const QString &fileName) const;
    void addWindowRecord(const QString &fileName, MainWindow *window);
    void removeWindowRecord(const QString &fileNam, MainWindow *window);

    QString getLastSaveDir() const;
    void setLastSaveDir(const QString &newDirName);

    QStringList getRecentFiles() const;
    void addRecentFile(const QString &fileName);

    bool isFirstLaunch = true;

private:
    WindowManager();
    ~WindowManager();

    QMap<QString, MainWindow*> windows; // 有名文件的窗口列表; 键: 文件名; 值: 窗口指针
    QSet<MainWindow*> anonymousFileWindows; // 新建无名文件的窗口列表

    QString lastSaveDir = "";  // 上次保存的目录

    QStringList recentFiles; // 最近打开的文件列表
};

#endif // WINDOWMANAGER_H
