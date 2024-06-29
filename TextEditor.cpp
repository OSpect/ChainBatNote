#include "TextEditor.h"
#include <QAction>

// TextEditor构造函数
TextEditor::TextEditor(QWidget *parent) : QTextEdit(parent) {}

// TextEditor析构函数
TextEditor::~TextEditor() {}


void TextEditor::setEditMode() {
    setReadOnly(false);  // 进入编辑模式
    emit modeChanged("Edit"); // 发出模式更改信号
}

void TextEditor::setViewMode() {
    setReadOnly(true);  // 进入阅览模式
    emit modeChanged("View"); // 发出模式更改信号
}

void TextEditor::keyPressEvent(QKeyEvent *event) {
    // 按键事件处理
    QTextEdit::keyPressEvent(event);
}
