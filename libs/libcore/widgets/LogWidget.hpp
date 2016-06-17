#ifndef LOGWIDGET_HPP
#define LOGWIDGET_HPP

#include <QWidget>
#include <QPlainTextEdit>


namespace Ui {
	class LogWidget;
}

class LogWidget : public QPlainTextEdit
{
		Q_OBJECT
	private:
		Ui::LogWidget *ui;

	public:
		explicit LogWidget(QWidget *parent = 0);
		~LogWidget();

	public:
		void appendLog(const QString& text);
};

#endif // LOGWIDGET_HPP
