#ifndef PENWIDTHSELECTWIDGET_HPP
#define PENWIDTHSELECTWIDGET_HPP

#include <QWidget>

class QComboBox;

class PenWidthSelectWidget: public QWidget
{
	Q_OBJECT
private:
	QComboBox *combo;
public:
	explicit PenWidthSelectWidget(QWidget *parent = nullptr);
	virtual ~PenWidthSelectWidget();

private:

	void setupUi();

	void populateCombo();

public:

	qreal penWidth();
	void setPenWidth(qreal width);

	// From QWidget
protected:
	virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


	signals:

	void penWidthChanged(qreal penWidth);

};

#endif // PENWIDTHSELECTWIDGET_HPP
