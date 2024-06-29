#ifndef CHAINBATDIALOG_H
#define CHAINBATDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTime>
#include <QCoreApplication>

class ChainBatDialog : public QDialog {
    Q_OBJECT

public:
    explicit ChainBatDialog(QWidget *parent = nullptr);
    ~ChainBatDialog();
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI();
    void connectSignalsSlots();

    // 界面组件
    QGroupBox *chainGroupBox;
    QGroupBox *batGroupBox;
    QTextEdit *descriptionText;

    // 功能按钮
    QPushButton *chainBatButton;

    // Bat 功能选择
    QCheckBox *chaosBatCheck;
    QCheckBox *humorBatCheck;
    QCheckBox *confuseBatCheck;
    QCheckBox *abstractBatCheck;
    QCheckBox *finalBatCheck;

private slots:
    void chainBatClicked();
};

#endif // CHAINBATDIALOG_H
