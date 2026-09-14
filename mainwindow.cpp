#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
	,englishDialog(new EnglishDialog(this))
	,numberDialog(new NumberDialog(this))
	,symbolDialog(new SymbolDialog(this))
	,settingsDialog(new SettingsDialog(this))
	,m_timer (new QTimer(this))
{
    ui->setupUi(this);


	setWindowTitle("HandWriteInput");
    setFixedSize(size());

	//批量绑定按钮事件
	initializeWordButtons();

	// 绑定对话框按钮和信号
	initializeDialogs();
	
	// 初始化
	WId hShow = ui->label->winId();
	m_inputs.initializeInput((HWND)hShow);  // 使用 InkRecognition

	initializeTimer();

	ui->textEdit->setPlaceholderText("name");
	ui->textEdit->document()->setMaximumBlockCount(1);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeWordButtons()
{
	for (int i = 0; i < 9; ++i) {
		QString buttonName = QString("Btn_word%1").arg(i);
		QPushButton* button = findChild<QPushButton*>(buttonName);
		if (button) {
			connect(button, &QPushButton::clicked, this, &MainWindow::on_Btn_word_clicked);
		}
	}
}

void MainWindow::initializeDialogs()
{
	// 绑定 Btn_English
	connect(ui->Btn_English, &QPushButton::clicked, this, &MainWindow::on_Btn_English_openDialog);
	connect(englishDialog, &EnglishDialog::textSelected, this, &MainWindow::on_Btn_clicked);

	// 绑定 Btn_number
	connect(ui->Btn_number, &QPushButton::clicked, this, &MainWindow::on_Btn_number_openDialog);
	connect(numberDialog, &NumberDialog::numberSelected, this, &MainWindow::on_Btn_clicked);

	// 绑定 Btn_symbol
	connect(ui->Btn_symbol, &QPushButton::clicked, this, &MainWindow::on_Btn_symbol_openDialog);
	connect(symbolDialog, &SymbolDialog::symbolSelected, this, &MainWindow::on_Btn_clicked);

	// 绑定 Btn_set
	connect(ui->Btn_set, &QPushButton::clicked, this, &MainWindow::on_Btn_set_openDialog);

}

void MainWindow::initializeTimer()
{
	connect(m_timer, &QTimer::timeout, this, &MainWindow::SLOT_timeout);
	m_timer->setSingleShot(true); // 设置单次触发
	m_autoinput = false;          // 初始化自动输入标志
}

void MainWindow::input_word(QString new_word)
{  
	ui->textEdit->setFocus();  // 获取焦点
	if (ui->Btn_word1->text() == "")
		return;

	if (m_autoinput == true)  // 如果已经自动输入，则删掉自动输入的，然后输入点击选择的
		on_Btn_delete_clicked();

	QString str = ui->textEdit->toPlainText();
	int pos = ui->textEdit->textCursor().position();
	QString str_input = str.left(pos);
	str_input = str_input + new_word + str.right(str.length() - pos);

	ui->textEdit->clear();
	ui->textEdit->setFontUnderline(false);
	ui->textEdit->setText(str_input);

	QTextCursor cur = ui->textEdit->textCursor();
	cur.setPosition(pos + 1);
	ui->textEdit->setTextCursor(cur);

	on_Btn_clear_clicked();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
	m_autoinput = false;
	m_timer->start(3000);
	//更新识别数据
	m_tempRlt.clear();
	m_inputs.recognizeData(m_tempRlt);  // 使用 InkRecognition

	// 遍历填充按钮文本
	const int buttonCount = 9; //一共9个选择
	for (int i = 0; i < buttonCount; ++i) {
		QPushButton* button = findChild<QPushButton*>(QString("Btn_word%1").arg(i + 1));
		if (button) {
			if (i < m_tempRlt.size()) {
				button->setText(QString::fromUtf8(QByteArray(m_tempRlt[i].rlt)));
				button->setEnabled(true); // 启用按钮
			}
			else {
				button->setEnabled(false); // 禁用按钮
				button->setText(""); // 如果没有足够的结果，则清空文本
			}
		}
	}

}


void MainWindow::on_Btn_delete_clicked()
{
	ui->textEdit->setFocus();  // 获取焦点
	int pos = ui->textEdit->textCursor().position();
	if (pos == 0)
		return;

	QString str_old = ui->textEdit->toPlainText();
	QString str_new = "";
	if (!str_old.isEmpty())
	{
		str_new = str_old.left(pos - 1) + str_old.right(str_old.length() - pos);
		ui->textEdit->setFontUnderline(false);
		ui->textEdit->setText(str_new);
		QTextCursor cur = ui->textEdit->textCursor();
		cur.setPosition(pos - 1);
		ui->textEdit->setTextCursor(cur);
	}
}

void MainWindow::on_Btn_deleteall_clicked()
{
	ui->textEdit->clear();
}

void MainWindow::on_Btn_word_clicked() 
{
	QPushButton* button = qobject_cast<QPushButton*>(sender()); // 获取信号发送者
	if (!button) return; // 如果信号发送者不是按钮类型，直接返回

	QString text = button->text(); // 获取按钮的文本内容
	if (text.isEmpty()) {
		qDebug() << "Button text is empty.";
		return;
	}

	// 插入文本到文本框
	ui->textEdit->insertPlainText(text);

	// 清空输入状态并停止计时器
	m_inputs.clearRecognitionData();
	m_tempRlt.clear();
	m_timer->stop();
	m_autoinput = true;

	// 清空所有候选字按钮
	clearCandidateButtons();

	// 点击按钮后清除手写框内的内容
	ui->label->clear();

	removeUnderlinedText(ui->textEdit);
	
}

void MainWindow::clearCandidateButtons() {
	for (int i = 1; i < 10; ++i) {
		QString buttonName = QString("Btn_word%1").arg(i);
		QPushButton* button = findChild<QPushButton*>(buttonName);
		if (button) {
			button->setText(""); // 清空按钮文本
		}
	}
}

void MainWindow::setCandidateButtons(const QStringList& candidates) {
	for (int i = 1; i < candidates.size() && i < 10; ++i) {
		QString buttonName = QString("Btn_word%1").arg(i);
		QPushButton* button = findChild<QPushButton*>(buttonName);
		if (button) {
			button->setText(candidates[i]); // 设置候选字文本
		}
	}
}


void MainWindow::on_Btn_output_clicked()
{
	
}

void MainWindow::on_Btn_clear_clicked()
{
	ui->textEdit->setFocus(); // 获取焦点
	m_inputs.clearInk(); // 删除笔触
	ui->label->clear();

	// 清除按钮文字
	clearCandidateButtons();
	
	//清除临时结果
	m_tempRlt.clear();

	removeUnderlinedText(ui->textEdit);

}



void MainWindow::SLOT_timeout()
{
	if (ui->Btn_word1->text().isEmpty())
		return;

	QString str = ui->textEdit->toPlainText();
	QString str1 = QString::fromUtf8(QByteArray(m_tempRlt[0].rlt));

	int pos = ui->textEdit->textCursor().position();

	ui->textEdit->clear();
	ui->textEdit->setFontUnderline(false);
	ui->textEdit->setText(str.left(pos));

	ui->textEdit->moveCursor(QTextCursor::End);
	ui->textEdit->setFontUnderline(true);
	ui->textEdit->insertPlainText(str1);
	ui->textEdit->setFontUnderline(false);

	ui->textEdit->insertPlainText(str.right(str.length() - pos));

	QTextCursor cur = ui->textEdit->textCursor();
	cur.setPosition(pos + 1);
	ui->textEdit->setTextCursor(cur);

	m_inputs.clearInk(); // 删除笔触
	ui->label->clear();

	m_autoinput = true;
}

void MainWindow::on_Btn_English_openDialog()
{
	if (!englishDialog) {
		qWarning() << "englishDialog is not initialized!";
		return;
	}
	englishDialog->exec();  // 打开对话框
}

void MainWindow::on_Btn_clicked(const QString& text)
{
	ui->textEdit->insertPlainText(text + " ");  // 将文本插入到 textEdit 中
}

void MainWindow::on_Btn_number_openDialog()
{
	if (!numberDialog) {
		qWarning() << "englishDialog is not initialized!";
		return;
	}
	numberDialog->exec();  // 打开对话框
}

void MainWindow::on_Btn_symbol_openDialog()
{
	if (!symbolDialog) {
		qWarning() << "symbolDialog is not initialized!";
		return;
	}
	symbolDialog->exec();  // 打开对话框
}

void MainWindow::on_Btn_set_openDialog()
{
	if (!settingsDialog) {
		qWarning() << "symbolDialog is not initialized!";
		return;
	}
	settingsDialog->exec();  // 打开对话框
}

void MainWindow::removeUnderlinedText(QTextEdit* textEdit) {
	if (!textEdit) return;

	// 保存当前光标的位置
	QTextCursor originalCursor = textEdit->textCursor();
	int originalPosition = originalCursor.position(); // 获取光标位置

	// 获取当前文本内容
	QString str = textEdit->toPlainText();
	QTextCursor cursor = textEdit->textCursor();
	cursor.movePosition(QTextCursor::Start); // 将光标移动到文本开始

	// 存储处理后的文本内容
	QString newText;
	int newCursorPosition = 0; // 记录新光标位置

	// 遍历原始文本，删除带下划线的字符
	for (int i = 0; i < str.length(); ++i) {
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

		// 检查字符格式
		QTextCharFormat format = cursor.charFormat();
		if (!format.fontUnderline()) {
			// 如果字符没有下划线，保留到 newText
			newText.append(str[i]);

			// 如果当前字符在原光标之前，更新新光标位置
			if (i < originalPosition) {
				newCursorPosition++;
			}
		}

		cursor.clearSelection(); // 清除当前字符的选区
	}

	// 更新文本框内容
	textEdit->clear();
	textEdit->setFontUnderline(false);
	textEdit->setText(newText);

	// 恢复光标到新位置
	QTextCursor newCursor = textEdit->textCursor();
	newCursor.setPosition(newCursorPosition);
	textEdit->setTextCursor(newCursor);
}
