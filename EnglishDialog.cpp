#include "EnglishDialog.h"

EnglishDialog::EnglishDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("English");
	setFixedSize(600, 400); // 设置固定大小

	QGridLayout* gridlayout = new QGridLayout(this); // 创建网格布局
	setLayout(gridlayout); // 设置为对话框的主布局
	createButtons(gridlayout); // 创建按钮并添加到布局
}

void EnglishDialog::createButtons(QGridLayout *layout)
{
	QStringList buttonTextsA;

	// 添加大写 A-Z
	for (char ch = 'A'; ch <= 'Z'; ++ch)
		buttonTextsA.append(QString(ch));


	int row = 0, col = 0;
	for (const QString& text : buttonTextsA) {
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

		// 按钮点击时发送文本
		connect(button, &QPushButton::clicked, [this, text]() {
			emit textSelected(text); // 发送文本到主窗口
			});

		layout->addWidget(button, row, col); // 将按钮添加到布局
		col++;
		if (col >= 9) { // 每行最多放 9 个按钮
			col = 0;
			row++;
		}
	}

	QStringList buttonTextsa;
	col=0;
	row++;
	// 添加小写 a-z
	for (char ch = 'a'; ch <= 'z'; ++ch)
		buttonTextsa.append(QString(ch));

	for (const QString& text : buttonTextsa) {
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

		// 按钮点击时发送文本
		connect(button, &QPushButton::clicked, [this, text]() {
			emit textSelected(text); // 发送文本到主窗口
			});

		layout->addWidget(button, row, col); // 将按钮添加到布局
		col++;
		if (col >= 9) { // 每行最多放 9 个按钮
			col = 0;
			row++;
		}
	}
}
