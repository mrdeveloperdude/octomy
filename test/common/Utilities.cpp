#include "Utilities.hpp"

#include "utility/Utility.hpp"

#include <QDateTime>

#include <QCoreApplication>
#include <QTest>
#include <QSignalSpy>

void testSleep(quint64 ms, QString occation)
{
	if(!occation.isEmpty()) {
		qDebug()<<"Sleeping, waiting for "+occation;
	}
	const quint64 start=utility::currentMsecsSinceEpoch<quint64>();
	quint64 now=start;
	while(now<start+ms) {
		now=utility::currentMsecsSinceEpoch<quint64>();
		QCoreApplication::processEvents();
	}
	if(!occation.isEmpty()) {
		qDebug()<<"Sleeping over for "+occation;
	}

}


void testHeading(QString msg, QString ch)
{
	auto sz=msg.size();
	auto full=80-4;
	auto half=(full-sz)/2;
	auto second=full-sz-half;
	//QTest::waitForEvents();
	qDebug()<<"";
	qDebug()<<"";
	qDebug().nospace().noquote()<<QString(ch).repeated(half)<< " # "<<msg << " # "<<QString(ch).repeated(second);
	qDebug()<<"";
}


UICloseFilter::UICloseFilter(QObject &ob)
	: QObject(nullptr)
	, target(ob)
{
	target.installEventFilter(this);
}

UICloseFilter::~UICloseFilter()
{

}

bool UICloseFilter::eventFilter(QObject *object, QEvent *event)
{
	if (object == &target && event->type() == QEvent::Close) {
		emit close();
		deleteLater();
	}
	return false;
}




bool waitForUIEnd(QObject *obj, qint64 timeOutMillis, const char * customSignal)
{
	const qint64 start=QDateTime::currentMSecsSinceEpoch();
	qDebug()<<"Waiting for UI to end with timeout="<<timeOutMillis<<" ("<< (timeOutMillis>0?"ENABLED":"DISABLED")<<")";
	if(nullptr==obj) {
		qDebug()<<"No ui specified";
		return false;
	}
	UICloseFilter *filter=new UICloseFilter(*obj);
	if(nullptr==filter) {
		qDebug()<<"Could not allocate close event filter";
		return false;
	}
	obj->installEventFilter(filter);
	QSignalSpy spyClose(filter, SIGNAL(close()));
	QSignalSpy *spyCustom=nullptr;
	if(nullptr!=customSignal) {
		spyCustom=new QSignalSpy(obj, customSignal);
	}
	qint64 now=start;
	qint64 elapsed=0;
	bool ret=false;
	while(true) {
		now=QDateTime::currentMSecsSinceEpoch();
		elapsed=(now-start);
		if((timeOutMillis>0) && (elapsed > timeOutMillis)) {
			qDebug()<<"Timout!";
			ret=true;
			break;
		}
		if(spyClose.count()>0) {
			qDebug()<<"Close event caught!";
			break;
		}
		if(nullptr!=customSignal && (spyCustom->count()>0) ) {
			qDebug()<<"Custom event caught!";
			break;
		}
		QTest::qWait(10);
	}
	if(nullptr!=customSignal) {
		delete customSignal;
		customSignal=nullptr;
	}
	return ret;
}

