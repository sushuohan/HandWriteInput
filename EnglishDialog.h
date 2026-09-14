#ifndef ENGLISHDIALOG_H
#define ENGLISHDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>

class EnglishDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnglishDialog(QWidget *parent = nullptr);

signals:
    void textSelected(const QString &text);  // 发送按钮文本到主窗口

private:
    void createButtons(QGridLayout* layout);
};

#endif // ENGLISHDIALOG_H