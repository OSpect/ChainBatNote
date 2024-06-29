#include "ChainBatDialog.h"
#include "MainWindow.h"
#include <QCloseEvent>
#include <QFontDatabase>
#include <QApplication>
#include <QString>
#include <QDebug>

ChainBatDialog::ChainBatDialog(QWidget *parent)
    : QDialog(parent), chainBatButton(nullptr), chaosBatCheck(nullptr), humorBatCheck(nullptr),
      confuseBatCheck(nullptr), abstractBatCheck(nullptr), finalBatCheck(nullptr) {
    setupUI();
    connectSignalsSlots();
}

ChainBatDialog::~ChainBatDialog() {}

void ChainBatDialog::closeEvent(QCloseEvent *event) {
    event->accept();
}

void ChainBatDialog::setupUI() {
    setWindowTitle(tr("Chain Bats"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 描述框
    descriptionText = new QTextEdit(this);
    descriptionText->setReadOnly(true);
    descriptionText->setHtml("<p>"
                             "<span style='color: magenta;'>Chain</span> "
                             "<span style='color: black;'>and</span> "
                             "<span style='color: magenta;'>Bat</span> "
                             "<span style='color: black;'>are 2</span> "
                             "<span style='color: magenta;'>hapless college students</span>"
                             "<span style='color: black;'>, yet</span> "
                             "<span style='color: magenta;'>they</span> "
                             "<span style='color: black;'>believe that the</span> "
                             "<span style='color: magenta;'>essence</span> "
                             "<span style='color: black;'>of</span> "
                             "<span style='color: magenta;'>college life</span> "
                             "<span style='color: black;'>is to</span> "
                             "<span style='color: magenta;'>fool around.</span>"
                             "</p>");

    mainLayout->addWidget(descriptionText);

    // 主按钮
    chainBatButton = new QPushButton(tr("Chain Bats!"), this);
    mainLayout->addWidget(chainBatButton);

    // Bat 选择
    batGroupBox = new QGroupBox(tr("Choose a Bat:"));
    QVBoxLayout *batLayout = new QVBoxLayout;

    chaosBatCheck = new QCheckBox(tr("KuXuanBat"), this);
    humorBatCheck = new QCheckBox(tr("YouMoBat"), this);
    confuseBatCheck = new QCheckBox(tr("ChouXiangBat"), this);
    abstractBatCheck = new QCheckBox(tr("TangShiBat"), this);
    finalBatCheck = new QCheckBox(tr("QiMoBat"), this);

    batLayout->addWidget(chaosBatCheck);
    batLayout->addWidget(humorBatCheck);
    batLayout->addWidget(confuseBatCheck);
    batLayout->addWidget(abstractBatCheck);
    batLayout->addWidget(finalBatCheck);

    batGroupBox->setLayout(batLayout);
    mainLayout->addWidget(batGroupBox);

    setLayout(mainLayout);
}

void ChainBatDialog::connectSignalsSlots() {
    connect(chainBatButton, &QPushButton::clicked, this, &ChainBatDialog::chainBatClicked);
}

void ChainBatDialog::chainBatClicked() {
    QStringList batList;

    if (chaosBatCheck->isChecked())
        batList.append("KuXuanBat");
    if (humorBatCheck->isChecked())
        batList.append("YouMoBat");
    if (confuseBatCheck->isChecked())
        batList.append("ChouXiangBat");
    if (abstractBatCheck->isChecked())
        batList.append("TangShiBat");
    if (finalBatCheck->isChecked())
        batList.append("QiMoBat");

    if (batList.isEmpty()) {
        QMessageBox::warning(this, tr("No Bat Selected"), tr("Please select at least one Bat to Chain."));
        return;
    }

    // 从多选项中抽一个 Bat
    QString selectedBat = batList.at(QRandomGenerator::global()->bounded(batList.size()));
    QMessageBox::information(this, tr("Chain Bat Applied"), tr("You have Chained a %1 to the current document.").arg(selectedBat));

    // 获取MainWindow, 应用抽到的Bat
    MainWindow *mainWindow = qobject_cast<MainWindow *>(parent());
    if (!mainWindow) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to obtain MainWindow instance."));
        return;
    }

    TextEditor *textEditor = mainWindow->textEditor;
    if (!textEditor) {
        QMessageBox::critical(this, tr("Error"), tr("TextEditor instance is null."));
        return;
    }

    if (selectedBat == "KuXuanBat") {
        QTextCursor cursor = textEditor->textCursor();
        cursor.select(QTextCursor::Document);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        QTextCharFormat format;
        QStringList fontFamilies = QFontDatabase().families();
        QString randomFont = fontFamilies.at(QRandomGenerator::global()->bounded(fontFamilies.size()));
        format.setFontFamily(randomFont);
        int randomSize = QRandomGenerator::global()->bounded(8, 30);
        format.setFontPointSize(randomSize);
        QColor randomColor = QColor::fromRgb(QRandomGenerator::global()->bounded(256),
                                             QRandomGenerator::global()->bounded(256),
                                             QRandomGenerator::global()->bounded(256));
        format.setForeground(randomColor);
        format.setFontItalic(QRandomGenerator::global()->bounded(2));
        format.setFontWeight(QRandomGenerator::global()->bounded(2) ? QFont::Bold : QFont::Normal);
        format.setFontUnderline(QRandomGenerator::global()->bounded(2));
        format.setFontStrikeOut(QRandomGenerator::global()->bounded(2));
        cursor.mergeCharFormat(format);
        cursor.clearSelection();
    } else if (selectedBat == "YouMoBat") {
        QString text = textEditor->toPlainText();
        QString result;
        for (int i = 0; i < text.length(); ++i) {
            QChar ch = text.at(i);
            if (QRandomGenerator::global()->bounded(10) < 3) {
                if (ch.isSpace()) {
                    result.append(ch);
                } else if (ch.isPunct()) {
                    result.append(QChar::fromLatin1(",.;!?"[QRandomGenerator::global()->bounded(5)]));
                } else {
                    result.append(QChar::fromLatin1(" \t\n"[QRandomGenerator::global()->bounded(3)]));
                }
            } else {
                result.append(ch);
            }
        }
        textEditor->setPlainText(result);
    } else if (selectedBat == "ChouXiangBat") {
        QByteArray text = textEditor->toPlainText().toUtf8();
        QByteArray encoded = text.toBase64(QRandomGenerator::global()->bounded(2) ? QByteArray::Base64Encoding : QByteArray::Base64UrlEncoding);
        textEditor->setPlainText(QString::fromUtf8(encoded));
    } else if (selectedBat == "TangShiBat") {
        QString text = textEditor->toPlainText();
        QString result;
        const QString dictionary = "😀😃😄😁😆😅🤣😂🙂🙃😉😊😇🥰😍🤩😘😗😚😙😋😛😜🤪😝🤑🤗🤭"
                                   "💨🐵🐒🦍🦧🐶🐕🦮🐕‍🦺🐩🐺🦊🦝🐱🐈🐈‍⬛🦁🐯🐅🍓🥝🍅🥥🥑🍆🥔🥕"
                                   "🌽🌶️🥒🥬🥦🧄🧅🍄🥜🌰🍞🥐🥖🫓🥨🥯🥞🧇🧀🍖🍗🥩🥓🍔🍟🍕🌭🥪🌮"
                                   "🌯🥙🧆🥚🍳🥘🍲🥣🥗🍿🧈🧂🥫🍱🍘🍙🍚🍛🍜🍝🍠🍢🍣🍤🍥🥮🍡🥟🥠"
                                   "🥡🦪🍦🍧🍨🍩🍪🎂🍰🧁🥧🍫🍬🍭🍮🍯🍼🥛🍵🍶🍾🍷🍸🍹🍺🍻🥂🥃🥤"
                                   "🧃🧉🧊🥢";
        for (int i = 0; i < text.length(); ++i) {
            QChar ch = text.at(i);
            if (ch.isLetterOrNumber()) {
                result.append(dictionary.at(QRandomGenerator::global()->bounded(dictionary.size())));
            } else {
                result.append(ch);
            }
        }
        textEditor->setPlainText(result);
    } else if (selectedBat == "QiMoBat") {
        QMessageBox::critical(this, tr("Fatal Error"), tr("Alas, Chain and Bat have perished during the final exam month. The ChainBat Editor is now lost forever. Bye!"));
        qDebug() << "BYE";
        QApplication::exit(-1);
    }
}
