#include "NameMappingView.hpp"

#include "uptime/MethodGate.hpp"

#include "NameMappingWidget.hpp"
#include "pose/PoseMapping.hpp"

#include <QPen>
#include <QPainter>
#include <QPushButton>
#include <QList>
#include <QRegularExpression>
#include <QPainterPath>
#include <QDebug>


NameMappingView::NameMappingView(QWidget *parent)
	: QWidget(parent)
	, mWidget(nullptr)
{
	OC_METHODGATE();
}



void NameMappingView::configure(NameMappingWidget &widget)
{
	OC_METHODGATE();
	mWidget=&widget;
	update();
}

void NameMappingView::paintEvent(QPaintEvent *)
{
	OC_METHODGATE();
	if(nullptr!=mWidget) {
		auto mapping = mWidget->mapping();
		if(!mapping.isNull()) {
			// Get buttons so we can use their coordinates
			QList<QPushButton *> fromButtons = findChildren<QPushButton *>(QRegularExpression("pushButtonFrom.*"));
			QList<QPushButton *> toButtons = findChildren<QPushButton *>(QRegularExpression("pushButtonTo.*"));
			/*
					for(QPushButton *but:fromButtons) {
						qDebug()<<"FROM BUTTON: "<<but->objectName()<<but->geometry();
					}
			//
					for(QPushButton *but:toButtons) {
						qDebug()<<"TO BUTTON: "<<but->objectName()<<but->geometry();
					}
			//		*/
			const qreal radius=6.0;
			QPainter painter(this);
			QPen pen;
			pen.setCapStyle(Qt::RoundCap);
			pen.setColor(Qt::red);
			pen.setWidthF(radius);
			painter.setBrush(Qt::NoBrush);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
			//const quint32 w=static_cast<quint32>(size().width());
			//const quint32 h=static_cast<quint32>(size().height());
			const quint32 sz=static_cast<quint32>(mapping->size());

			const qreal step=(1.0)/((qreal)sz);
			qreal hue=step;
			QColor col;
			QStringList fromList=mWidget->fromList();
			QStringList toList=mWidget->toList();
			quint32 keyIndex = 0;
			for(auto const &key:fromList) {
				QString val = (*mapping)[key];
				quint32 valIndex=toList.indexOf(val);
				//const quint32 m=mapping->map(i);
				const bool dif=(keyIndex!=valIndex);
				qDebug()<< "hue"<< hue << "dif"<<dif;
				col.setHslF(hue,0.5,0.25,dif?1.0:0.3);
				hue+=step;
				pen.setColor(col);
				painter.setPen(pen);

				//qDebug()<<"GOT MAP FROM "<<i<<" TO " <<m;
				QPushButton *fromButton=(keyIndex<fromButtons.size())?fromButtons.at(keyIndex):nullptr;
				QPushButton *toButton=(valIndex<toButtons.size())?toButtons.at(valIndex):nullptr;
				if((nullptr!=fromButton) &&(nullptr!=toButton) ) {
					QRect fromRect=fromButton->geometry();
					QRect toRect=toButton->geometry();
					QPointF fromPoint(fromRect.x()+fromRect.width()+radius*1.5,fromRect.y()+fromRect.height()/2);
					QPointF toPoint(toRect.x()-radius*1.5,toRect.y()+toRect.height()/2);
					QPointF midPoint=(fromPoint+toPoint)/2.0;
					//qDebug()<<"STEP "<<i<<" of "<<sz<<" has buttons: "<<fromButton<<toButton<<" and points "<<fromPoint<<midPoint<<toPoint;
					QPainterPath path;
					path.moveTo(fromPoint);
					path.cubicTo(QPointF(midPoint.x(),fromPoint.y()), QPointF(midPoint.x(),toPoint.y()), toPoint);
					painter.drawPath(path);
				}
				keyIndex++;

			}
		}
	}
}
