#include "NumberDialog.h"

NumberDialog::NumberDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Digital");
    setFixedSize(600, 400); // 设置固定大小

    QGridLayout *gridLayout = new QGridLayout(this); // 创建网格布局
    setLayout(gridLayout); // 应用布局到对话框

    createButtons(gridLayout); // 创建按钮并添加到布局中
}

void NumberDialog::createButtons(QGridLayout *layout)
{
    QStringList digits;
    int row = 0, col = 0;
    // 添加数字 0-9
    for (int i = 0; i <= 9; ++i)
		digits.append(QString::number(i));

	for (const QString& text : digits) {
		QPushButton* button = new QPushButton(text, this);
		button->setFixedSize(50, 50); // 按钮固定大小
		button->setStyleSheet(
			"QPushButton {"
			"  border-radius: 10px;" // 圆角矩形
			"  background-color: #808080;" // 按钮背景色
			"  color: white;" // 按钮文字颜色
			"  font-size: 16px;"
			"}"
			"QPushButton:hover {"
			"  background-color: #4682B4;" // 鼠标悬停颜色
			"}"
		);

		// 按钮点击时发送数字
		connect(button, &QPushButton::clicked, [this, text]() {
			emit numberSelected(text); // 发送文本到主窗口
			});

		layout->addWidget(button, row, col); // 将按钮添加到布局
		col++;
		if (col >= 7) { // 每行最多放 7 个按钮
			col = 0;
			row++;
		}
	}
}
