#include "CommsCarrier.hpp"

#include "uptime/MethodGate.hpp"
#include "uptime/ConnectionType.hpp"
#include "utility/time/HumanTime.hpp"

#include "address/AddressList.hpp"

#include "CommsCarrier.hpp"

#include <QHostAddress>
#include <QDateTime>

Q_DECLARE_METATYPE(QHostAddress)
Q_DECLARE_METATYPE(QHostAddress *)

quint64 CommsCarrier::sTotalRxCount = 0;
quint64 CommsCarrier::sTotalTxCount = 0;

CommsCarrier::CommsCarrier(QObject *parent)
	: QObject(parent)
	, mConnected(false)
	, mConfigureHelper("CommsCarrier", true, true, false, true, false)
	, mRXRate("RX RATE", 10000)
	, mTXRate("TX RATE", 10000)
	, mTXOpportunityRate("OP RATE")
{
	OC_METHODGATE();
}


CommsCarrier::~CommsCarrier()
{
	OC_METHODGATE();
}

void CommsCarrier::configure()
{
	OC_METHODGATE();
	if(mConfigureHelper.configure()) {
		mSendingTimer.setSingleShot(false);
		mSendingTimer.setTimerType(Qt::PreciseTimer);
		if(!connect(&mSendingTimer, SIGNAL(timeout()), this, SLOT(onOpportunityTimer()), OC_CONTYPE)) {
			qWarning()<<"Could not connect sending timer";
		}
		configureImp();
	}
}


bool CommsCarrier::activate(bool start)
{
	OC_METHODGATE();
	bool success=true;
	if(mConfigureHelper.activate(start)) {
		if(start) {
			if(isStarted()) {
				activate(false);
			}
			success=activateImp(true);
			if(success) {
				mSendingTimer.start();
			} else {
				activate(false);
			}
		} else {
			mSendingTimer.stop();
			activateImp(false);
			mConnected=false;
			emit carrierConnectionStatusChanged(false);
		}
	} else {
		success=false;
	}
	return success;

}



void CommsCarrier::setListenAddress(NetworkAddress address)
{
	OC_METHODGATE();
	if(mConfigureHelper.isConfiguredAsExpected()) {
		setAddressImp(address);
	}
}



void CommsCarrier::detectConnectionChanges(const quint64 now)
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		const quint64 timeSinceLastRX = (now - mRXRate.mLast);
		const quint64 timeout = connectionTimeout();
		/*
		qDebug().nospace().noquote()
				<< ("DETECTIN' timeSinceLastRX=")
				<< timeSinceLastRX
				<< (", timeout=")
				<< timeout
				<< (", now=")
				<< now
				<< (", mRXRate=")
				<< mRXRate
				//<< (", mConnected=")
				//<< (mConnected?"YES":"NO");
				;
				*/
		const bool old=mConnected;
		if(mConnected && (timeSinceLastRX > timeout) ) {
			mConnected=false;
			qDebug()<<" #!#!#!# Connection timed out";
		} else if(!mConnected && (timeSinceLastRX <= timeout) ) {
			mConnected=true;
			qDebug()<<" #!#!#!# Connection completed";
		}
		if(old!=mConnected) {
			emit carrierConnectionStatusChanged(mConnected);
		}
	}
}



void CommsCarrier::onOpportunityTimer()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		//qDebug()<<"Opportunity timer";
		const quint64 now=utility::time::currentMsecsSinceEpoch<quint64>();
		mTXOpportunityRate.countPacket(0, now);
		detectConnectionChanges(now);
		emit carrierSendingOpportunity(now);
	}
}


quint64 CommsCarrier::connectionTimeout()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		// 1 sec  more than our minimal packet rate
		return minimalPacketInterval() + 1000;
	}
	return 0;
}


void CommsCarrier::setHookCarrierSignals(QObject &ob, bool hook)
{
	OC_METHODGATE();
	if(mConfigureHelper.isConfiguredAsExpected()) {
		//qRegisterMetaType<CommsSession *>("Client *");
		if(hook) {
			if(!connect(this,SIGNAL(carrierReadyRead()),&ob,SLOT(onCarrierReadyRead()),OC_CONTYPE)) {
				qWarning()<<"ERROR: Could not connect "<<ob.objectName();
			}
			if(!connect(this,SIGNAL(carrierError(QString)),&ob,SLOT(onCarrierError(QString)),OC_CONTYPE)) {
				qWarning()<<"ERROR: Could not connect "<<ob.objectName();
			}
			if(!connect(this,SIGNAL(carrierSendingOpportunity(quint64)),&ob,SLOT(onCarrierSendingOpportunity(quint64)),OC_CONTYPE)) {
				qWarning()<<"ERROR: Could not connect "<<ob.objectName();
			}
			if(!connect(this,SIGNAL(carrierConnectionStatusChanged(bool)),&ob,SLOT(onCarrierConnectionStatusChanged(bool)),OC_CONTYPE)) {
				qWarning()<<"ERROR: Could not connect "<<ob.objectName();
			}
		} else {
			if(!disconnect(this,SIGNAL(carrierReadyRead()),&ob,SLOT(onCarrierReadyRead()))) {
				qWarning()<<"ERROR: Could not disconnect "<<ob.objectName();
			}
			if(!disconnect(this,SIGNAL(carrierError(QString)),&ob,SLOT(onCarrierError(QString)))) {
				qWarning()<<"ERROR: Could not disconnect "<<ob.objectName();
			}
			if(!disconnect(this,SIGNAL(carrierSendingOpportunity(quint64)),&ob,SLOT(onCarrierSendingOpportunity(quint64)))) {
				qWarning()<<"ERROR: Could not disconnect "<<ob.objectName();
			}
			if(!disconnect(this,SIGNAL(carrierConnectionStatusChanged(bool)),&ob,SLOT(onCarrierConnectionStatusChanged(bool)))) {
				qWarning()<<"ERROR: Could not disconnect "<<ob.objectName();
			}
		}
	}
}



// CommsCarrier internal interface methods
///////////////////////////////////////////////////////////




bool CommsCarrier::isStarted() const
{
	OC_METHODGATE();
	const bool isToo=isActiveImp();
	return (mSendingTimer.isActive() && isToo);
}

bool CommsCarrier::isConnected() const
{
	OC_METHODGATE();
	return mConnected;
}


qint64 CommsCarrier::writeData(const QByteArray &datagram, const NetworkAddress &address)
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		const qint64 len=writeDataImp(datagram, address);
		mTXRate.countPacket(static_cast<quint32>(len));
		sTotalRxCount++;
		return len;
	}
	return 0;
}

qint64 CommsCarrier::readData(char *data, qint64 maxlen, QHostAddress *host, quint16 *port)
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		const qint64 len=readDataImp(data,maxlen,host,port);
		mRXRate.countPacket(static_cast<quint32>(len));
		sTotalTxCount++;
		return len;
	}
	return 0;
}

bool CommsCarrier::hasPendingData()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return hasPendingDataImp();
	}
	return false;
}

qint64 CommsCarrier::pendingDataSize()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return pendingDataSizeImp();
	}
	return -1;
}


QString CommsCarrier::errorString()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return errorStringImp();
	}
	return "Not activated";
}


NetworkAddress CommsCarrier::address()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return addressImp();
	}
	return NetworkAddress();
}


quint64 CommsCarrier::minimalPacketInterval()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return minimalPacketIntervalImp();
	}
	return 0;
}


quint64 CommsCarrier::maximalPacketInterval()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return maximalPacketIntervalImp();
	}
	return 0;
}


quint64 CommsCarrier::setDesiredOpportunityInterval(quint64 desiredInterval)
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		const quint64 actualInterval=qBound(maximalPacketInterval(), desiredInterval, minimalPacketInterval());
		//qDebug()<<"NEW SCHEDULE DELAY: "<<actualInterval<<" ("<<desiredInterval<<")";
		mSendingTimer.setInterval(static_cast<int>(actualInterval));
		return actualInterval;
	}
	return 0;
}

quint64 CommsCarrier::opportunityInterval()
{
	OC_METHODGATE();
	if(mConfigureHelper.isActivatedAsExpected()) {
		return static_cast<quint64>(mSendingTimer.interval());
	}
	return 0;
}
