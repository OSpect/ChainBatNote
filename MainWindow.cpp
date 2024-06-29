#include "MainWindow.h"
#include "WindowManager.h"
#include "ChainBatDialog.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QCloseEvent>
#include <QFontComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextStream>
#include <QLineEdit>
#include <QInputDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, const QString &fileName)
    : QMainWindow(parent), isModified(false), currentFileName(fileName), fileManager(new FileManager()), textEditor(new TextEditor(this)) {
    createActions(); // 创建动作
    createMenus();   // 创建菜单
    createFormatDockWidgets(); // 创建格式工具栏

    setCentralWidget(textEditor);  // 设置中心部件为文本编辑器
    connect(textEditor, &QTextEdit::textChanged, this, &MainWindow::documentWasModified); // 连接信号
    connect(textEditor, &TextEditor::modeChanged, this, &MainWindow::updateModeLabel); // 连接信号

    statusBar = new QStatusBar(this); // 创建状态栏
    setStatusBar(statusBar);
    modeLabel = new QLabel("Mode: Edit", this); // 初始化模式标签
    modeLabel->setContentsMargins(10, 0, 10, 0); // 设置左右边距
    statusBar->addWidget(modeLabel);
    formatStatusLabel = new QLabel("Text is formatted: Yes", this); // 初始化格式状态标签
    formatStatusLabel->setContentsMargins(10, 0, 10, 0); // 设置左右边距
    statusBar->addWidget(formatStatusLabel);

    if (!fileName.isEmpty()) { // 由其它窗口唤起新有名文件窗口
        if (fileManager->openFile(fileName)) {
            textEditor->setHtml(fileManager->getFileContent());
            WindowManager::instance().addRecentFile(fileName);
        }
    } else { // 初次打开编辑器或新建无名文件窗口
        ;
    }

    if (WindowManager::instance().isFirstLaunch) { // 把第一个窗口加入管理器表
        WindowManager::instance().addWindowRecord(currentFileName, this);
        WindowManager::instance().isFirstLaunch = false;
    }

    updateWindowTitle();
    updateRecentFilesMenu();
}

MainWindow::~MainWindow() {
    delete fileManager;
    delete textEditor;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Unsaved Changes", "The document has unsaved changes. Do you want to save them?",
                                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reply == QMessageBox::Save) {
            saveFile();
        } else if (reply == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
    }
    WindowManager::instance().removeWindowRecord(currentFileName, this);
    event->accept();
}


void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this); // 新建文件动作
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    newAct->setShortcut(QKeySequence("Ctrl+N"));

    openAct = new QAction(tr("&Open..."), this); // 打开文件动作
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    openAct->setShortcut(QKeySequence("Ctrl+O"));

    saveAct = new QAction(tr("&Save"), this); // 保存文件动作
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    saveAct->setShortcut(QKeySequence("Ctrl+S"));

    saveAsAct = new QAction(tr("Save &As..."), this); // 另存为动作
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFileAs);
    saveAsAct->setShortcut(QKeySequence("Ctrl+Shift+S"));

    deleteAct = new QAction(tr("&Delete"), this); // 删除文件动作
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteFile);
    deleteAct->setShortcut(QKeySequence("Ctrl+D"));

    editModeAction = new QAction(tr("Edit Mode"), this); // 设置为编辑模式动作
    connect(editModeAction, &QAction::triggered, textEditor, &TextEditor::setEditMode);
    editModeAction->setShortcut(QKeySequence("Ctrl+W"));

    viewModeAction = new QAction(tr("View Mode"), this); // 设置为阅读模式动作
    connect(viewModeAction, &QAction::triggered, textEditor, &TextEditor::setViewMode);
    viewModeAction->setShortcut(QKeySequence("Ctrl+R"));

    findAction = new QAction(tr("&Find"), this);  // 查找动作
    connect(findAction, &QAction::triggered, this, &MainWindow::findText);
    findAction->setShortcut(QKeySequence("Ctrl+F"));

    replaceAction = new QAction(tr("&Replace"), this);  // 替换动作
    connect(replaceAction, &QAction::triggered, this, &MainWindow::replaceText);
    replaceAction->setShortcut(QKeySequence("Ctrl+Alt+R"));

    toggleFontDockAction = new QAction(tr("Font"), this); // 切换字体Dock显示动作
    connect(toggleFontDockAction, &QAction::triggered, this, &MainWindow::toggleFontDock);
    toggleFontDockAction->setCheckable(true);

    toggleSizeDockAction = new QAction(tr("Size"), this); // 切换大小Dock显示动作
    connect(toggleSizeDockAction, &QAction::triggered, this, &MainWindow::toggleSizeDock);
    toggleSizeDockAction->setCheckable(true);

    toggleColorDockAction = new QAction(tr("Color"), this); // 切换颜色Dock显示动作
    connect(toggleColorDockAction, &QAction::triggered, this, &MainWindow::toggleColorDock);
    toggleColorDockAction->setCheckable(true);

    toggleStyleDockAction = new QAction(tr("Style"), this); // 切换样式Dock显示动作
    connect(toggleStyleDockAction, &QAction::triggered, this, &MainWindow::toggleStyleDock);
    toggleStyleDockAction->setCheckable(true);
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File")); // 文件菜单
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(deleteAct);

    QMenu *modeMenu = menuBar()->addMenu(tr("&Mode")); // 模式菜单
    modeMenu->addAction(editModeAction);
    modeMenu->addAction(viewModeAction);

    formatMenu = menuBar()->addMenu(tr("&Format")); // 格式菜单
    createFormatMenu();

    auxiliaryMenu = menuBar()->addMenu(tr("&Auxiliary")); // 辅助功能菜单
    createAuxiliaryMenu();
}


void MainWindow::createFormatDockWidgets() {
    // 创建字体工具栏
    fontDock = new QDockWidget(tr("Font (!)"), this);
    fontDock->setToolTip(tr("Warning: Set font before adjusting other options to avoid overwriting."));
    QFontComboBox *fontComboBox = new QFontComboBox(fontDock);
    // 更换字体
    // BUG: 字体更改目前会将选中部分全部的大小(Size)及样式(Style)值清空, 暂未发现修复方式
    connect(fontComboBox, &QFontComboBox::currentFontChanged, [=](const QFont &font) {
        textEditor->setCurrentFont(font);
        updateFormatStatus("Yes");
        qDebug() << "Flawed font changing behavior";
    });
    fontDock->setWidget(fontComboBox);
    addDockWidget(Qt::RightDockWidgetArea, fontDock);
    fontDock->setVisible(true); // 默认显示
    toggleFontDockAction->setChecked(true);
    fontDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);


    // 创建大小工具栏
    sizeDock = new QDockWidget(tr("Size"), this);
    QSpinBox *sizeSpinBox = new QSpinBox(sizeDock);
    sizeSpinBox->setRange(8, 72);
    connect(sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int size) {
        textEditor->setFontPointSize(size);
        updateFormatStatus("Yes");
    });
    sizeDock->setWidget(sizeSpinBox);
    addDockWidget(Qt::RightDockWidgetArea, sizeDock);
    sizeDock->setVisible(true); // 默认显示
    toggleSizeDockAction->setChecked(true);
    sizeDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);

    // 创建颜色工具栏
    colorDock = new QDockWidget(tr("Color"), this);
    QPushButton *colorButton = new QPushButton(tr("Select Color"), colorDock);
    connect(colorButton, &QPushButton::clicked, [=]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
        if (color.isValid()) {
            textEditor->setTextColor(color);
            updateFormatStatus("Yes");
        }
    });
    colorDock->setWidget(colorButton);
    addDockWidget(Qt::RightDockWidgetArea, colorDock);
    colorDock->setVisible(true); // 默认显示
    toggleColorDockAction->setChecked(true);
    colorDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);

    // 创建样式工具栏
    styleDock = new QDockWidget(tr("Style"), this);
    QWidget *styleWidget = new QWidget(styleDock);
    QHBoxLayout *styleLayout = new QHBoxLayout(styleWidget);

    // 斜体按钮
    QPushButton *italicButton = new QPushButton(tr("Italic"));
    connect(italicButton, &QPushButton::clicked, [=]() {
        QTextCharFormat format;
        if (!textEditor->textCursor().hasSelection()) {
            return;
        }
        format.setFontItalic(!textEditor->textCursor().charFormat().fontItalic());
        textEditor->textCursor().mergeCharFormat(format);
        updateFormatStatus("Yes");
    });
    styleLayout->addWidget(italicButton);

    // 粗体按钮
    QPushButton *boldButton = new QPushButton(tr("Bold"));
    connect(boldButton, &QPushButton::clicked, [=]() {
        QTextCharFormat format;
        if (!textEditor->textCursor().hasSelection()) {
            return;
        }
        format.setFontWeight(textEditor->textCursor().charFormat().fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
        textEditor->textCursor().mergeCharFormat(format);
        updateFormatStatus("Yes");
    });
    styleLayout->addWidget(boldButton);

    // 下划线按钮
    QPushButton *underlineButton = new QPushButton(tr("Underline"));
    connect(underlineButton, &QPushButton::clicked, [=]() {
        QTextCharFormat format;
        if (!textEditor->textCursor().hasSelection()) {
            return;
        }
        format.setFontUnderline(!textEditor->textCursor().charFormat().fontUnderline());
        textEditor->textCursor().mergeCharFormat(format);
        updateFormatStatus("Yes");
    });
    styleLayout->addWidget(underlineButton);

    // 删除线按钮
    QPushButton *strikethroughButton = new QPushButton(tr("Strikethrough"));
    connect(strikethroughButton, &QPushButton::clicked, [=]() {
        QTextCharFormat format;
        if (!textEditor->textCursor().hasSelection()) {
            return;
        }
        format.setFontStrikeOut(!textEditor->textCursor().charFormat().fontStrikeOut());
        textEditor->textCursor().mergeCharFormat(format);
        updateFormatStatus("Yes");
    });
    styleLayout->addWidget(strikethroughButton);

    styleWidget->setLayout(styleLayout);
    styleDock->setWidget(styleWidget);
    styleDock->setFixedHeight(fontDock->height() * 3 / 2);
    addDockWidget(Qt::RightDockWidgetArea, styleDock);
    styleDock->setVisible(true); // 默认显示
    toggleStyleDockAction->setChecked(true);
    styleDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
}

void MainWindow::createFormatMenu() {
    QAction *toggleFormatActions[] = {
        toggleFontDockAction,
        toggleSizeDockAction,
        toggleColorDockAction,
        toggleStyleDockAction
    };

    for (auto action : toggleFormatActions) {
        formatMenu->addAction(action);
    }

    // 全显示动作
    QAction *showAllDocksAction = new QAction(tr("Show All Docks"), this);
    connect(showAllDocksAction, &QAction::triggered, this, [=]() {
        fontDock->setVisible(true);
        sizeDock->setVisible(true);
        colorDock->setVisible(true);
        styleDock->setVisible(true);
    });
    formatMenu->addAction(showAllDocksAction);

    // 全隐藏动作
    QAction *hideAllDocksAction = new QAction(tr("Hide All Docks"), this);
    connect(hideAllDocksAction, &QAction::triggered, this, [=]() {
        fontDock->setVisible(false);
        sizeDock->setVisible(false);
        colorDock->setVisible(false);
        styleDock->setVisible(false);
    });
    formatMenu->addAction(hideAllDocksAction);

    // 清除格式动作
    QAction *clearFormattingAction = new QAction(tr("Clear Formatting"), this);
    connect(clearFormattingAction, &QAction::triggered, this, [=]() {
        QTextCursor cursor = textEditor->textCursor();
        if (!cursor.hasSelection()) {
            QMessageBox::information(this, tr("Clear Formatting"), tr("Please select the text you want to clear the formatting for."));
            return;
        }

        if (QMessageBox::question(this, tr("Clear Formatting"), tr("Are you sure you want to clear the formatting of the selected text?")) == QMessageBox::Yes) {
            cursor.setCharFormat(QTextCharFormat()); // 清除格式
            textEditor->setTextCursor(cursor);
        }
    });
    clearFormattingAction->setShortcut(QKeySequence("Ctrl+Shift+F"));
    formatMenu->addAction(clearFormattingAction);
}


void MainWindow::createAuxiliaryMenu() {
    // 最近文件
    recentFilesSubMenu = new QMenu(tr("Recent Files")); // 最近文件子菜单
    recentFilesSubMenu->setEnabled(false); // 默认禁用，需要更新后启用

    recentFilesSubMenuAction = new QAction(tr("Recent Files"), this);
    recentFilesSubMenuAction->setMenu(recentFilesSubMenu);
    auxiliaryMenu->addAction(recentFilesSubMenuAction);

    // 查找和替换
    auxiliaryMenu->addAction(findAction);
    auxiliaryMenu->addAction(replaceAction);

    // 玩笑功能 Chain A Bat
    QAction *chainBatAction = auxiliaryMenu->addAction(tr("Chain A Bat..."));
    connect(chainBatAction, &QAction::triggered, this, &MainWindow::openChainBatDialog);
}

void MainWindow::updateRecentFilesMenu() {
    recentFilesSubMenu->clear(); // 清空当前菜单项

    QStringList recentFiles = WindowManager::instance().getRecentFiles();
    for (const QString &fileName : recentFiles) {
        QAction *fileAction = recentFilesSubMenu->addAction(fileName);
        connect(fileAction, &QAction::triggered, this, &MainWindow::openRecentFile);
    }

    recentFilesSubMenu->setEnabled(!recentFiles.isEmpty()); // 如果有最近文件则启用菜单
}

void MainWindow::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString fileName = action->text();
        if (!fileName.isEmpty()) {
            if (!WindowManager::instance().hasWindowRecord(fileName)) {
                WindowManager::instance().createWindow(fileName);
            }
        }
    }
}


void MainWindow::newFile() {
    WindowManager::instance().createWindow(); // 新建窗口
}

void MainWindow::openFile() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "", tr("HTML Files (*.html);;Text Files (*.txt);;All Files (*)"));
    for (const QString &fileName : fileNames) {
        if (!WindowManager::instance().hasWindowRecord(fileName)) {
            WindowManager::instance().createWindow(fileName);

            // 更新最近打开列表
            WindowManager::instance().addRecentFile(fileName);
        }
    }
}

void MainWindow::saveFile() {
    QString fileName = currentFileName.isEmpty() ? fileManager->getCurrentFile() : currentFileName;
    if (fileName.isEmpty()) {
        saveFileAs(); // 如果当前文件名为空，则调用另存为
        updateWindowTitle();
    } else {
        if (fileManager->saveFile(fileName, textEditor->toHtml())) {
            isModified = false;  // 重置标志
            updateWindowTitle();
        }
    }
}

void MainWindow::saveFileAs() {
    QString defaultFileName = "untitled.html";
    QString directory = WindowManager::instance().getLastSaveDir().isEmpty() ? QDir::homePath() : WindowManager::instance().getLastSaveDir();

    QString baseName = QFileInfo(defaultFileName).completeBaseName();
    QString extension = QFileInfo(defaultFileName).completeSuffix();
    QString newFileName = directory + "/" + defaultFileName;
    int counter = 1;

    // 生成最小可用默认文件名
    while (QFile::exists(newFileName)) {
        newFileName = QString("%1/%2(%3).%4").arg(directory).arg(baseName).arg(counter++).arg(extension);
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"), newFileName, tr("HTML Files (*.html);;Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty()) {
        WindowManager::instance().setLastSaveDir(QFileInfo(fileName).absolutePath());

        // 更新目前文件名以及窗口控制器的窗口表
        WindowManager::instance().removeWindowRecord(currentFileName, this);
        currentFileName = fileName;
        WindowManager::instance().addWindowRecord(currentFileName, this);

        // 更新最近打开列表
        WindowManager::instance().addRecentFile(currentFileName);

        // 根据文件扩展名选择保存的内容格式
        QString extension = QFileInfo(fileName).suffix().toLower();
        bool success = false;

        if (extension == "html") {
            success = fileManager->saveFile(fileName, textEditor->toHtml());
            updateFormatStatus("Yes");
        } else if (extension == "txt") {
            QString plainTextContent = textEditor->toPlainText();
            // 清除格式
            textEditor->clear();
            textEditor->setPlainText(plainTextContent);
            success = fileManager->saveFile(fileName, textEditor->toPlainText());
            updateFormatStatus("No");
        } else {
            success = fileManager->saveFile(fileName, textEditor->toHtml()); // 默认保存为HTML
            updateFormatStatus("Yes");
        }

        if (success) {
            isModified = false;
            updateWindowTitle();
        }
    }
}

void MainWindow::deleteFile() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Delete File(s)"), "", tr("HTML Files (*.html);;Text Files (*.txt);;All Files (*)"));
    if (!fileNames.isEmpty()) {
        QStringList failedFiles;
        for (const QString &fileName : fileNames) {
            if (!fileManager->deleteFile(fileName)) {
                failedFiles.append(fileName);
            }
        }

        if (!failedFiles.isEmpty()) {
            QMessageBox::warning(this, tr("Deletion Failed!"), tr("Failed to delete the following files:\n%1").arg(failedFiles.join("\n")));
        }
    }
}


void MainWindow::toggleFontDock() {
    fontDock->setVisible(toggleFontDockAction->isChecked());
}

void MainWindow::toggleSizeDock() {
    sizeDock->setVisible(toggleSizeDockAction->isChecked());
}

void MainWindow::toggleColorDock() {
    colorDock->setVisible(toggleColorDockAction->isChecked());
}

void MainWindow::toggleStyleDock() {
    styleDock->setVisible(toggleStyleDockAction->isChecked());
}


void MainWindow::findText() {
    QString searchString = QInputDialog::getText(this, tr("Find"), tr("Find &what:"), QLineEdit::Normal);
    if (!searchString.isEmpty()) {
        QTextCursor cursor = textEditor->textCursor();
        QTextDocument::FindFlags flags;
        int counter = 0;

        cursor.movePosition(QTextCursor::Start);

        QTextCharFormat plainFormat;
        plainFormat.setUnderlineStyle(QTextCharFormat::UnderlineStyle::NoUnderline);

        // 搜索到的子串加下虚线
        QTextCharFormat highlightFormat;
        highlightFormat.setUnderlineStyle(QTextCharFormat::DashUnderline);
        highlightFormat.setUnderlineColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));

        QTextCursor highlightCursor = textEditor->textCursor();
        highlightCursor.setPosition(0);
        highlightCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        highlightCursor.mergeCharFormat(plainFormat);

        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = textEditor->document()->find(searchString, cursor, flags);

            if (!cursor.isNull()) {
                QTextCharFormat matchFormat = highlightFormat;
                highlightCursor.setPosition(cursor.position() - searchString.length());
                highlightCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, searchString.length());
                highlightCursor.mergeCharFormat(matchFormat);
                counter++;
            }
        }

        QMessageBox::information(this, tr("Information"), tr("Found %n occurrence(s).", "", counter));
    }
}

void MainWindow::replaceText() {
    QString searchString = QInputDialog::getText(this, tr("Replace"), tr("Find &what:"), QLineEdit::Normal);
    if (searchString.isEmpty()) {
        return; // 用户取消或未输入查找文本
    }

    // 查找
    QTextCursor cursor = textEditor->textCursor();
    QTextDocument::FindFlags flags;
    int counter = 0;

    cursor.movePosition(QTextCursor::Start);

    QTextCharFormat plainFormat;
    plainFormat.setUnderlineStyle(QTextCharFormat::UnderlineStyle::NoUnderline);

    QTextCharFormat highlightFormat;
    highlightFormat.setUnderlineStyle(QTextCharFormat::DashUnderline);
    highlightFormat.setUnderlineColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));

    QTextCursor highlightCursor = textEditor->textCursor();
    highlightCursor.setPosition(0);
    highlightCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    highlightCursor.mergeCharFormat(plainFormat);

    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = textEditor->document()->find(searchString, cursor, flags);

        if (!cursor.isNull()) {
            QTextCharFormat matchFormat = highlightFormat;
            highlightCursor.setPosition(cursor.position() - searchString.length());
            highlightCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, searchString.length());
            highlightCursor.mergeCharFormat(matchFormat);
            counter++;
        }
    }

    QMessageBox::information(this, tr("Information"), tr("Found %n occurrence(s).", "", counter));

    // 替换
    if (counter > 0) {
        QString replaceString = QInputDialog::getText(this, tr("Replace"), tr("Replace with:"), QLineEdit::Normal);
        if (!replaceString.isEmpty()) {
            cursor.movePosition(QTextCursor::Start);

            while (!cursor.isNull() && !cursor.atEnd()) {
                cursor = textEditor->document()->find(searchString, cursor, flags);

                if (!cursor.isNull()) {
                    cursor.beginEditBlock();
                    cursor.removeSelectedText();
                    cursor.insertText(replaceString);
                    cursor.endEditBlock();
                }
            }

            QMessageBox::information(this, tr("Information"), tr("Replaced %n occurrence(s).", "", counter));
        }
    }
}


void MainWindow::openChainBatDialog() {
    // 打开Chain Bat对话框
    ChainBatDialog *dialog = new ChainBatDialog(this);
    dialog->exec();
    delete dialog;
}


void MainWindow::documentWasModified() {
    isModified = true;
    updateWindowTitle();
}

void MainWindow::updateWindowTitle() {
    QString editorName = "[ChainBat Note alpha1]";
    QString fileName = currentFileName.isEmpty() ? "untitled new file" : currentFileName;
    if (isModified) {
        setWindowTitle(editorName + " Now editing " + fileName + " *");
    } else {
        setWindowTitle(editorName + " Now editing " + fileName);
    }
}

void MainWindow::updateModeLabel(const QString &mode) {
    modeLabel->setText("Mode: " + mode);
}

void MainWindow::updateFormatStatus(const QString &status) {
    formatStatusLabel->setText("Text is formatted: " + status);
}
