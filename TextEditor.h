#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QImage>

// TextEditor类，负责文本编辑器的基本结构和功能声明
class TextEditor : public QTextEdit {
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);  // 构造函数
    ~TextEditor();  // 析构函数

public slots:
    void setEditMode();  // 设置编辑模式
    void setViewMode();  // 设置阅览模式

//    QString toHtml() const;
//    void setHtml(const QString &html);

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void modeChanged(const QString &mode); // 模式更改信号

private:
    QString currentMode; // 当前模式
};

#endif // TEXTEDITOR_H
