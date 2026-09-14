#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QRect>
#include <QTimer>
#include <QTextCursor>
#include <QDebug>
#include <QTextBlock>
#include <QTextEdit>
#include "InkRecognition.h"
#include "EnglishDialog.h"
#include "NumberDialog.h"
#include "SymbolDialog.h"
#include "SettingsDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	// 成员变量
	InkRecognition m_inputs;  
	InPutRltS m_tempRlt;  
	QTimer* m_timer;          // 自动输入定时器
	bool m_autoinput;         // 是否自动输入
	void input_word(QString new_word);  // 输入文字



protected:
	void mouseReleaseEvent(QMouseEvent* e);  // 重写鼠标释放事件
	void initializeDialogs();
	void initializeWordButtons();
	void initializeTimer();
	void removeUnderlinedText(QTextEdit* textEdit);
	void clearCandidateButtons();
	void setCandidateButtons(const QStringList& candidates);
	void SLOT_timeout();  // 自动输入超时槽函数
	void on_Btn_set_openDialog();

private slots:

	void on_Btn_delete_clicked();   // 删除一个字
	void on_Btn_deleteall_clicked();// 全部删除
	void on_Btn_word_clicked();
	void on_Btn_clear_clicked();   // 清除按钮
	void on_Btn_output_clicked();  // 输出按钮槽函数
	void on_Btn_clicked(const QString& text);  // 接收从对话框传来的文本
	void on_Btn_English_openDialog();  // 打开英文字母对话框的槽函数
	void on_Btn_number_openDialog();  // 打开数字对话框的槽函数
	void on_Btn_symbol_openDialog();  // 打开符号对话框的槽函数

private:
    Ui::MainWindow *ui;
	EnglishDialog* englishDialog;
	NumberDialog* numberDialog;
	SymbolDialog* symbolDialog;
	SettingsDialog* settingsDialog;

};
#endif // MAINWINDOW_H
