#include "CommsSession.hpp"

#include "basic/LogDestination.hpp"
#include "CommsChannel.hpp"
#include "security/Key.hpp"
#include "utility/Standard.hpp"





#include <QDateTime>



///////////////////////////////////////////////////////////////////////////


CommsSession::CommsSession(Key &key)
	: mKey(key)
	, mLocalSessionID(0) // 0 means no valid session id selected yet
	, mLastSendTime(0)
	, mLastReceiveTime(mLastSendTime)
	, mDisconnectTimeoutAccumulator(0.0f)
	, mDisconnectTimeout(3.0)
	, mExpireTimeoutAccumulator(0)
	, mExpireTimeout(5*60*1000) // 5 minutes of inactivity means the session may be pruned (by default, value may change)
	, mDeltaTime(0.0f)
	, mIdleAccumulator(0.0)
	, mIdlePacketsSent(0)
	, mConnected(false)
	, mLastConnected(false)
	, mExpired(false)
	, mInitialized(false)
{

	if(nullptr == &key){
		qWarning()<<"ERROR: key was nullptr";
	}
	mInitialized=true;
}


CommsSession::~CommsSession()
{

}


ReliabilitySystem &CommsSession::reliabilitySystem()
{
	return mReliabilitySystem;
}

FlowControl &CommsSession::flowControl()
{
	return mFlowControl;
}

qint64 CommsSession::lastSendTime() const
{
	return mLastSendTime;
}

qint64 CommsSession::lastRecieveTime() const
{
	return mLastReceiveTime;
}

float CommsSession::timeoutAccumulator() const
{
	return mDisconnectTimeoutAccumulator;
}

float CommsSession::deltaTime() const
{
	return mDeltaTime;
}

float CommsSession::idleAccumulator() const
{
	return mIdleAccumulator;
}

quint32 CommsSession::idlePacksSent() const
{
	return mIdlePacketsSent;
}

Key &CommsSession::key() const
{
	return mKey;
}

bool CommsSession::connected() const
{
	return mConnected;
}

bool CommsSession::lastConnected() const
{
	return mLastConnected;
}

bool CommsSession::handshakeStarted() const
{
	return !mHandshakeState.isVirgin();
}


bool CommsSession::established() const
{
	return mHandshakeState.isDone();
}

HandshakeStep CommsSession::nextStep() const{
	return mHandshakeState.nextStep();
}

bool CommsSession::expired() const
{
	return mExpired;
}


quint16 CommsSession::localSessionID() const
{
	return mLocalSessionID;
}


quint16 CommsSession::remoteSessionID() const
{
	return mRemoteSessionID;
}


QString CommsSession::fullID() const
{
	if(!mInitialized){
		qWarning()<<"ERROR: !mInitialized in fullID()";
	}
	if(nullptr==&mKey){
		qWarning()<<"ERROR: mKey was nullptr in fullID()";
	}
	QString id=mKey.id();
	return id;
}

NetworkAddress CommsSession::address() const
{
	return mAddress;
}


void CommsSession::setLocalSessionID(SESSION_ID_TYPE s)
{
	mLocalSessionID=s;
}

void CommsSession::setRemoteSessionID(SESSION_ID_TYPE s)
{
	mRemoteSessionID=s;
}


void CommsSession::setAddress(const NetworkAddress &address)
{
	mAddress=address;
}

void CommsSession::setExpired()
{
	mExpired=true;
}



void CommsSession::countSend(qint64 written)
{
	const qint64 now=QDateTime::currentMSecsSinceEpoch();
	if(mLastSendTime<=0) {
		mLastSendTime=now-1;
	}
	const qint64 delta=now-mLastSendTime;
	mLastSendTime=now;
	mDeltaTime=delta/1000.0f;
	mDisconnectTimeoutAccumulator += mDeltaTime;
	mExpireTimeoutAccumulator+=(qint64)qMax((qint64)0, delta);
	if (mConnected && ( mDisconnectTimeoutAccumulator > mDisconnectTimeout )) {
		qDebug()<<"SESSION "<< signatureToString() <<" disconnected";
		mConnected=false;
	}
	if (!mExpired && ( mExpireTimeoutAccumulator > mExpireTimeout )) {
		qDebug()<<"SESSION "<< signatureToString() <<" expired";
		mExpired=true;
	}
	if(mConnected) {
		mReliabilitySystem.update(mDeltaTime);
		mFlowControl.update( mDeltaTime, mReliabilitySystem.roundTripTime() * 1000.0f );
	}
	if(mConnected!=mLastConnected) {
		mLastConnected=mConnected;
		mFlowControl.reset();
		qDebug()<<"SESSION: New flow state: " <<(mConnected?"CONNECTED":"DISCONNECTED")<< " for "<<signatureToString();
	}
	mReliabilitySystem.packetSent(written);
}

void CommsSession::receive()
{
	mLastReceiveTime=QDateTime::currentMSecsSinceEpoch();
	mConnected=true;
	mDisconnectTimeoutAccumulator = 0.0f;
	mExpireTimeoutAccumulator=0;
}

bool CommsSession::idle()
{
	const float sendRate = mFlowControl.sendRate();
	if( mDeltaTime > (1.0f / sendRate) ) {
		qDebug()<<"SENDING IDLE PACKET "<<mIdlePacketsSent;
		mIdleAccumulator=0.0f;
		mIdlePacketsSent++;
		return mConnected;
	}
	return false;
}




const QString CommsSession::signatureToString() const
{
	return QString::number(mLocalSessionID,16)+"-"+mAddress.toString()+"("+fullID()+")";
}

QString CommsSession::summary(QString sep) const
{
	QString out;
	QTextStream ts(&out);
	ts << "ID: "<< signatureToString();
	ts << ", NET: "<<mAddress.toString();
	ts << ", DELTA: "<<mDeltaTime<<sep;
	ts << ", TOA: "<<mDisconnectTimeoutAccumulator<<sep;
	ts << mFlowControl.toString(sep);
	return out;
}

QString CommsSession::toString() const
{
	QString out;
	QTextStream ts(&out);
	ts << "sig="<< signatureToString()<<", delta: "<<mDeltaTime<<", TOA: "<<mDisconnectTimeoutAccumulator;
	return out;
}


const QString CommsSession::listText() const
{
	QString name=summary(" - ");
	return name;
}

quint64 CommsSession::lastActiveTime() const
{
	// Sending does not count, because we may be sending to deaf ears
	//return qMax(lastSendTime, lastReceiveTime);
	return mLastReceiveTime;
}

////////////////////////////



QVariantMap CommsSession::toVariantMap()
{
	OC_METHODGATE();
	QVariantMap map;
	map["lastSendTime"]=mLastSendTime;
	map["lastReceiveTime"]=mLastReceiveTime;
	return map;
}



void CommsSession::fromVariantMap(const QVariantMap map)
{
	OC_METHODGATE();
	mLastSendTime=( map["lastSendTime"].toInt());
	mLastReceiveTime=( map["lastReceiveTime"].toInt());
}
