#include "SettingsDialog.h"
#include <QColor>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent), defaultColor(Qt::black), defaultThickness(2) {
    setWindowTitle("设置");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 笔迹颜色
    QLabel* colorLabel = new QLabel("选择颜色:");
    colorComboBox = new QComboBox(this);
    colorComboBox->addItem("黑色", QColor(Qt::black));
    colorComboBox->addItem("红色", QColor(Qt::red));
    colorComboBox->addItem("蓝色", QColor(Qt::blue));
    colorComboBox->addItem("绿色", QColor(Qt::green));
    colorComboBox->addItem("黄色", QColor(Qt::yellow));

    // 笔迹粗细
    QLabel* thicknessLabel = new QLabel("选择粗细:");
    thicknessComboBox = new QComboBox(this);
    thicknessComboBox->addItem("细 (1px)", 1);
    thicknessComboBox->addItem("中 (2px)", 2);
    thicknessComboBox->addItem("粗 (3px)", 3);

    // 恢复默认设置按钮
    QPushButton* resetButton = new QPushButton("恢复默认", this);
    QPushButton* applyButton = new QPushButton("应用", this);

    // 布局
    mainLayout->addWidget(colorLabel);
    mainLayout->addWidget(colorComboBox);
    mainLayout->addWidget(thicknessLabel);
    mainLayout->addWidget(thicknessComboBox);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(resetButton);
    mainLayout->addLayout(buttonLayout);

    // 信号与槽
    connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);
    connect(resetButton, &QPushButton::clicked, this, &SettingsDialog::onResetClicked);
}

QColor SettingsDialog::getSelectedColor() const {
    return colorComboBox->currentData().value<QColor>();
}

int SettingsDialog::getSelectedThickness() const {
    return thicknessComboBox->currentData().toInt();
}

void SettingsDialog::onApplyClicked() {
    emit settingsChanged(getSelectedColor(), getSelectedThickness());
    accept();
}

void SettingsDialog::onResetClicked() {
    colorComboBox->setCurrentIndex(0);
    thicknessComboBox->setCurrentIndex(1);
}
