#include "SymbolDialog.h"

SymbolDialog::SymbolDialog(QWidget* parent)
	: QDialog(parent), layout(new QGridLayout(this)) {
	// 符号列表
	QStringList symbols = {
		",", ".", "/", ";", "'", "[", "]", "=","+", "-",
		"<",">", "?", ":", "{", "}",  "_", "(", ")",
		"*", "&", "^", "%", "$", "#", "@", "!", "`", "~"
	};

	int row = 0;
	int col = 0;

	for (const QString& symbol : symbols) {
		QPushButton* button = new QPushButton(symbol, this);
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

		// 按钮点击时发送符号
		connect(button, &QPushButton::clicked, [this, symbol]() {
			emit symbolSelected(symbol); // 发送符号到主窗口
			});

		layout->addWidget(button, row, col); // 将按钮添加到布局
		col++;
		if (col >= 7) { // 每行最多放 7 个按钮
			col = 0;
			row++;
		}
	}
}
