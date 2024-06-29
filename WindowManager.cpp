#include "WindowManager.h"
#include "MainWindow.h"
#include <QDebug>
#include <QFile>

WindowManager::WindowManager() {}

WindowManager::~WindowManager() {
    qDeleteAll(windows);
    windows.clear();
}

WindowManager& WindowManager::instance() {
    static WindowManager instance;
    return instance;
}

void WindowManager::createWindow(const QString &fileName) {
    MainWindow *newWindow = new MainWindow(nullptr, fileName);
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            file.close();

            if (fileName.endsWith(".html", Qt::CaseInsensitive)) {
                newWindow->textEditor->setHtml(fileContent);
            } else {
                newWindow->textEditor->setPlainText(fileContent);
                newWindow->updateFormatStatus("No");
            }
        }
    }
    addWindowRecord(fileName, newWindow);
    newWindow->isModified = false;
    newWindow->updateWindowTitle();
    newWindow->show();
}


bool WindowManager::hasWindowRecord(const QString &fileName) const {
    return windows.contains(fileName);
}

void WindowManager::addWindowRecord(const QString &fileName, MainWindow *window) {
    if (fileName.isEmpty()) {
        anonymousFileWindows.insert(window);
        qDebug() << "WindowManager::addWindowRecord Added anonymous window";
    } else {
        windows.insert(fileName, window);
        qDebug() << "WindowManager::addWindowRecord Added window record for: " << fileName;
    }
}

void WindowManager::removeWindowRecord(const QString &fileName, MainWindow *window) {
    if (fileName.isEmpty()) {
        anonymousFileWindows.remove(window);
        qDebug() << "WindowManager::removeWindowRecord Removed anonymous window";
    } else if (windows.value(fileName) == window) {
        windows.remove(fileName);
        qDebug() << "WindowManager::removeWindowRecord Removed window record for: " << fileName;
    }
}

QString WindowManager::getLastSaveDir() const {
    return lastSaveDir;
}

void WindowManager::setLastSaveDir(const QString &newDirName) {
    lastSaveDir = newDirName;
}


QStringList WindowManager::getRecentFiles() const {
    return recentFiles;
}

void WindowManager::addRecentFile(const QString &fileName) {
    recentFiles.removeAll(fileName); // 确保不重复添加
    recentFiles.prepend(fileName); // 将最近打开的文件名添加到列表开头
    while (recentFiles.size() > 15) {
        recentFiles.removeLast(); // 保持最多15个最近打开文件
    }

    // 同步更新所有有名窗口的最近文件菜单
    QList<MainWindow *> allWindows = windows.values();
    for (MainWindow *win : allWindows) {
        win->updateRecentFilesMenu();
    }
    // 更新无名文件窗口列表的最近文件菜单
    for (MainWindow *anonWin : anonymousFileWindows) {
        anonWin->updateRecentFilesMenu();
    }
}
