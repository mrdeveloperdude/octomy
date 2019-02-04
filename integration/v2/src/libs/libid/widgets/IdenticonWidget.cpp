#include "IdenticonWidget.hpp"

#include "uptime/MethodGate.hpp"
#include "uptime/New.hpp"

#include "security/PortableID.hpp"

#include <QPainter>
#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include <QSvgRenderer>

///////////////////////////////////////////////////////////////////////////

IdenticonWidget::IdenticonWidget(QWidget *parent)
	: SvgWidget(parent)
{
	OC_METHODGATE();
	PortableID pid;
	setPortableID(pid);
}

void IdenticonWidget::regenerateIdenticon()
{
	OC_METHODGATE();
	//TODO: This is a gaping hole if someone tried to set the SVG and did not expect us to delete it (not recommended way to use this class but still)
	if(nullptr!=mSVG) {
		delete mSVG;
		mSVG=nullptr;
	}
	QDomDocument doc=mIdenticon.domDocument();
	mSVG=OC_NEW QSvgRenderer (doc.toByteArray());
	mLastURL="";
	mDirty=true;
	update();
}

void IdenticonWidget::setPortableID(PortableID &id)
{
	OC_METHODGATE();
	mIdenticon.setPortableID(id);
	regenerateIdenticon();
}

Identicon IdenticonWidget::identicon()
{
	OC_METHODGATE();
	return mIdenticon;
}

void IdenticonWidget::mouseDoubleClickEvent(QMouseEvent *)
{
	OC_METHODGATE();
	emit doubleClicked();
}


QDomDocument IdenticonWidget::svgDOM()
{
	OC_METHODGATE();
	return mIdenticon.domDocument();
}

QByteArray IdenticonWidget::svgXML()
{
	OC_METHODGATE();
	return mIdenticon.domDocument().toByteArray();
}
