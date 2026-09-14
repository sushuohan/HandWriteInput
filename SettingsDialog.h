#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);

    QColor getSelectedColor() const;
    int getSelectedThickness() const;

signals:
    void settingsChanged(QColor color, int thickness);

private slots:
    void onApplyClicked();
    void onResetClicked();

private:
    QComboBox* colorComboBox;
    QComboBox* thicknessComboBox;

    QColor defaultColor;
    int defaultThickness;
};

#endif // SETTINGSDIALOG_H
