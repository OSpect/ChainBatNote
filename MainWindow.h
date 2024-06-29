#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QFileDialog>
#include <QMimeData>
#include <QClipboard>
#include <QMessageBox>
#include "FileManager.h"
#include "TextEditor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, const QString &fileName = "");
    ~MainWindow();
    void closeEvent(QCloseEvent *event) override;

    friend class WindowManager;
    friend class ChainBatDialog;

private:
    void setupUI();       // 设置UI组件
    void createActions(); // 创建文件动作（新建、打开、保存等）
    void createMenus();   // 创建菜单

    void createAuxiliaryMenu(); // 创建辅助功能菜单内容
    void openRecentFile(); // 在辅助功能最近文件子菜单打开最近文件
    void updateRecentFilesMenu(); // 更新最近文件子菜单

    void createFormatMenu(); // 创建格式菜单内容
    void createFormatDockWidgets(); // 创建格式工具栏

    void updateWindowTitle(); // 更新窗口标题显示
    void updateFormatStatus(const QString &status); // 更新格式状态显示

    // 基本组件指针
    FileManager *fileManager;  // 文件管理器指针
    TextEditor *textEditor;    // 文本编辑器指针
    QStatusBar *statusBar;     // 状态栏指针
    QLabel *modeLabel;         // 状态栏模式标签
    QLabel *formatStatusLabel; // 状态栏格式状态标签

    // 标题显示参数
    QString currentFileName;   // 此文件名
    bool isModified = false;   // 文本已修改标志

    // 辅助功能菜单指针及参数
    QMenu *auxiliaryMenu; // 辅助功能菜单
    QAction *recentFilesSubMenuAction; // 最近文件子菜单的展开按钮
    QMenu *recentFilesSubMenu; // 最近文件子菜单

    // 格式菜单、工具栏和动作
    QMenu *formatMenu; // 格式菜单
    QDockWidget *fontDock;
    QDockWidget *sizeDock;
    QDockWidget *colorDock;
    QDockWidget *styleDock;

    QAction *toggleFontDockAction;
    QAction *toggleSizeDockAction;
    QAction *toggleColorDockAction;
    QAction *toggleStyleDockAction;

    // 文件动作指针
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *deleteAct;

    // 辅助功能动作指针
    QAction *editModeAction;    // 切换编辑模式动作
    QAction *viewModeAction;    // 切换阅读模式动作

    QAction *findAction;        // 查找动作
    QAction *replaceAction;     // 替换动作

private slots:
    void newFile();      // 新建文件槽函数
    void openFile();     // 打开文件槽函数
    void saveFile();     // 保存文件槽函数
    void saveFileAs();   // 另存为文件槽函数
    void deleteFile();   // 删除文件槽函数

    void documentWasModified(); // 文件内容更改槽函数
    void updateModeLabel(const QString &mode); // 更新编辑状态槽函数

    void toggleFontDock();  // 显示/隐藏字体设置槽函数
    void toggleSizeDock();  // 显示/隐藏大小设置槽函数
    void toggleColorDock(); // 显示/隐藏颜色设置槽函数
    void toggleStyleDock(); // 显示/隐藏样式设置槽函数

    void findText();            // 查找文本槽函数
    void replaceText();         // 替换文本槽函数

    void openChainBatDialog();  // 打开Chain Bat小游戏槽函数
};

#endif // MAINWINDOW_H
