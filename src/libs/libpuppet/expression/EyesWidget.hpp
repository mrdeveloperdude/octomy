#ifndef EYESWIDGET_HPP
#define EYESWIDGET_HPP

#include "expression/IrisRendrer.hpp"
#include <QWidget>
#include <QTimer>
#include <QVector2D>
#include <QSharedPointer>
#include <QImage>


class Eye
{
private:
	QVector2D center;
	bool dirty;
	QPolygonF basicEyeShapeBg;
	QPolygonF scaleraPolygon;
	QPolygonF irisPolygon;
	QPolygonF pupilPolygon;
	QPolygonF upperMask;
	QPolygonF lowerMask;
	QVector2D eyeSteer;
	QVector2D upperLidSteer;
	QVector2D lowerLidSteer;
	QVector2D squintSteer;
	qreal blink;
	qreal slant;

	QVector2D  specPos1;
	QVector2D  specPos2;
	QBrush lidBrush;
	QBrush scaleraBrush;
	QBrush irisBrush;
	QBrush pupilBrush;
	QBrush specularBrush;

	QSharedPointer<QImage> mIrisImage;

public:

	explicit Eye(QVector2D center, qreal slant, QColor irisColor=QColor("#2d8ac9"));
	virtual ~Eye();
	void update();
	void setBlink(qreal);
	void setExpression(QVector2D, QVector2D, QVector2D);
	void setColor(QColor irisColor=QColor("#2d8ac9"));
	void setIrisImage(QSharedPointer<QImage>);
	void setSteer(QVector2D);
	void paint(QPainter &painter);
};

class Personality;
class PortableID;
class Identicon;

class EyesWidget : public QWidget
{
	Q_OBJECT
private:
	QTimer timer;
	quint64 startTime;
	quint64 lastTime;
	qreal blink;
	qreal cycle;
	Eye leftEye;
	Eye rightEye;
	QBrush bgBrush;

	QVector2D upperLidSteer;
	QVector2D lowerLidSteer;
	QVector2D eyeSteer;
	QVector2D eyeSteerSmooth;
	QVector2D squintSteer;

	QVector2D lastPress;

	IrisRendrer mIrisRendrer;

public:
	explicit EyesWidget(QWidget *parent = 0);

public:
	void setPortableID(PortableID &pid);

protected:
	void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
	void hideEvent(QHideEvent *) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

public slots:
	void onUpdateTimer();
};

#endif // EYESWIDGET_HPP
