#ifndef SYMBOLDIALOG_H
#define SYMBOLDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QGridLayout>
#include <QPushButton>

class SymbolDialog : public QDialog {
	Q_OBJECT

public:
	explicit SymbolDialog(QWidget* parent = nullptr);

signals:
	void symbolSelected(const QString& symbol);

private:
	QGridLayout* layout;
};

#endif // SYMBOLDIALOG_H
