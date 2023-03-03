#ifndef MARKDOWNEDITOR_HPP
#define MARKDOWNEDITOR_HPP

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QDir>


class OctomyParse;

class QSyntaxHighlighter;

namespace Ui
{
class MarkdownEditor;
}

namespace markdown{
Q_NAMESPACE

class MarkdownEditor : public QWidget
{
	Q_OBJECT
	
private:
	Ui::MarkdownEditor *ui;
	QSyntaxHighlighter *highlighter;
	QString plan_fn;
	QTimer saveTimer;
	QDir const topDir;
	QDir lastDir;
	
public:
	explicit MarkdownEditor(QWidget *parent = nullptr);
	virtual ~MarkdownEditor() Q_DECL_OVERRIDE;
	
public:
	void configure(QString plan_fn);
	void setText(QString);
	void toPDF(const QString &filename);
	void appendText(QString);
	
protected:
	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
	
public slots:
	void onTextChanged();
	
private slots:
	void on_pushButtonExportPDF_clicked();
	
};

}

#endif
// MARKDOWNEDITOR_HPP

