#ifndef NUMBERDIALOG_H
#define NUMBERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>

class NumberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NumberDialog(QWidget *parent = nullptr);
    ~NumberDialog() = default;

signals:
    void numberSelected(const QString &number);

private:
    void createButtons(QGridLayout *layout);
};

#endif // NUMBERDIALOG_H
