#include "MockCommsCarrier.hpp"

#include <QHostAddress>

#include <QtGlobal>

MockCommsCarrier::MockCommsCarrier(QObject *parent)
	: CommsCarrier(parent)
	, mHasPendingData(false)
	, mWriteBatchSize(-1)
	, mIsStarted(false)
	, mStartFail(false)
	, mMinimalPacketInterval(100) // Shortest pause between sending opportunities is 100 ms (Throttling)
	, mMaximalPacketInterval(5000) // Longest pause between sending opportunities is 5000 ms (keepalive)
	, mOverrideStartStop(false)
{

}


MockCommsCarrier::~MockCommsCarrier()
{
	qDebug()<<"Mock-dtor";
}

////////////////////////// Mock interface

void MockCommsCarrier::mockWriteMock(QByteArray data, const NetworkAddress &address, bool sendReadyReadSignal)
{
	QString key=address.toString();
	mMockReadData.append(QPair<QString, QByteArray>(key, data));
	qDebug()<<"Mock-writing "<< data.size() << "bytes of data to be read from "<<key;
	if(sendReadyReadSignal) {
		mHasPendingData=true;
		emit carrierReadyRead();
	}
}

QByteArray MockCommsCarrier::mockReadMock(const NetworkAddress &address)
{
	const QString key=address.toString();
	if(mMockWriteData.contains(key) && ! mMockWriteData[key].isEmpty()) {
		QByteArray out = mMockWriteData[key].takeFirst();
		qDebug()<<"Mock-reading "<< out.size() << "bytes of data to be written to "<<key;
		return out;
	}
	return QByteArray();
}

void MockCommsCarrier::mockSetHasPendingData(bool hasPendingData)
{
	qDebug()<<"Mock-Setting HasPendingData to "<< hasPendingData;
	mHasPendingData=hasPendingData;
}

void MockCommsCarrier::mockSetWriteBatchSize(qint64 writeBatchSize)
{
	qDebug()<<"Mock-Setting WriteBatchSize to "<< writeBatchSize;
	mWriteBatchSize=writeBatchSize;
}

void MockCommsCarrier::mockSetStartFail(bool startFail)
{
	qDebug()<<"Mock-Setting StartFail to "<< startFail;
	mStartFail=	startFail;
}

void MockCommsCarrier::mockSetMinimalPacketInterval(quint64 size)
{
	qDebug()<<"Mock-Setting MinimalPacketInterval to "<< size;
	mMinimalPacketInterval=size;
}

void MockCommsCarrier::mockSetMaximalPacketIntervalImp(quint64 size)
{
	qDebug()<<"Mock-Setting MaximalPacketInterval to "<< size;
	mMaximalPacketInterval=size;
}

void MockCommsCarrier::mockSetAddress(NetworkAddress addr)
{
	qDebug()<<"Mock-Setting OurAddress to "<< addr;
	mOurAddress=addr;
}


void MockCommsCarrier::mockStartSendingTimer()
{
	qDebug()<<"Mock-StartSendingTimer";
	mSendingTimer.start();
}

void MockCommsCarrier::mockStopSendingTimer()
{
	qDebug()<<"Mock-StopSendingTimer";
	mSendingTimer.stop();
}


void MockCommsCarrier::mockTriggerReadyRead()
{
	qDebug()<<"Mock-Triggering ready read signal";
	emit carrierReadyRead();
}


void MockCommsCarrier::mockTriggerErrorSignal(QString error)
{
	qDebug()<<"Mock-Triggering error signal with message '"<<error<<"'";
	mLastError=error;
	emit carrierError(error);
}


void MockCommsCarrier::mockTriggerSendingOpportunity(quint64 now)
{
	qDebug()<<"Mock-Triggering sending opportunity signal with now='"<<now<<"'";
	emit carrierSendingOpportunity(now);
}

void MockCommsCarrier::mockTriggerConnectionStatusChanged(bool connected)
{
	qDebug()<<"Mock-Triggering connection status changed signal with new status='"<<connected<<"'";
	emit carrierConnectionStatusChanged(connected);
}

////////////////////////// CommsCarrier overrides to take countrol over sending opportunity timer



bool MockCommsCarrier::start(NetworkAddress address)
{
	if(mOverrideStartStop) {
		return isStarted();
	} else {
		return CommsCarrier::start(address);
	}
}

void MockCommsCarrier::stop()
{
	if(mOverrideStartStop) {
		return;
	} else {
		CommsCarrier::stop();
	}
}




//////////////////////////  CommsCarrier internal interface methods


bool MockCommsCarrier::startImp(NetworkAddress address)
{
	const bool oldIsStarted=mIsStarted;
	if(!mStartFail) {
		mOurAddress=address;
		mIsStarted=true;
	}
	qDebug() << "start() called for address "<< address<< " with mock-fail=" << mStartFail << " and isStarted " << oldIsStarted << " --> " << mIsStarted;
	return mIsStarted;
}


void MockCommsCarrier::stopImp()
{
	qDebug() << "stop() called";
	mIsStarted=false;
}


bool MockCommsCarrier::isStartedImp() const
{
	return mIsStarted;
}


qint64 MockCommsCarrier::writeDataImp(const QByteArray &datagram, const NetworkAddress &address)
{
	const qint64 sz=datagram.size();
	const qint64 written=(mWriteBatchSize>=0)?qMin(mWriteBatchSize, sz):sz;
	if(written > 0) {
		datagram.left(written);
		QString key=address.toString();
		mMockWriteData[key].append(datagram);
	}
	qDebug() << "write() called. wrote "<<written << " of "<< sz <<"bytes to address="<<address.toString()<<" (batch size="<<mWriteBatchSize<<")";
	return written;
}

qint64 MockCommsCarrier::readDataImp(char *data, qint64 maxlen, QHostAddress *host, quint16 *port)
{
	qDebug() << "read() called with maxlen="<<maxlen<<", data="<<(nullptr!=data)<<", host="<<(nullptr!=host)<<" and port="<<(nullptr!=port);
	if(mMockReadData.isEmpty()) {
		qDebug() << "read() returning 0 bytes as no data was available";
		return 0;
	}
	QPair<QString, QByteArray> pair=mMockReadData.takeFirst();
	const QString &key=pair.first;
	NetworkAddress na;
	na.fromString(key);
	QByteArray all=pair.second;
	const qint64 sz=all.size();
	const quint64 bytesRead=qMin(maxlen, sz);
	QByteArray send=all.left(bytesRead);
	// Toss back the remaining data if any
	QByteArray keep=all.right(sz-bytesRead);
	if(keep.size()>0) {
		pair.second=keep;
		mMockReadData.prepend(pair);
	}
	// Send the requested number of bytes
	if(nullptr!=all && bytesRead > 0) {
		memcpy(send.data(), data, bytesRead);
	}
	// Send the host
	if(nullptr!=host) {
		*host=QHostAddress(na.ip());
	}
	// Send the port
	if(nullptr!=port) {
		*port=na.port();
	}
	qDebug() << "read() returning bytesRead="<<bytesRead<<", host="<<(nullptr!=host?host->toString():"nullptr")<<" and port="<<(nullptr!=port?QString::number(*port):"nullptr");
	return bytesRead;
}


bool MockCommsCarrier::hasPendingDataImp()
{
	return (!mMockReadData.isEmpty());
}

qint64 MockCommsCarrier::pendingDataSizeImp()
{
	if(mMockReadData.isEmpty()) {
		qDebug()<<"WARNING: Asking for pending size when no pending data";
		return 0;
	}
	const QPair<QString, QByteArray> &pair=mMockReadData.first();
	return pair.second.size();
}


QString MockCommsCarrier::errorStringImp()
{
	return mLastError;
}


NetworkAddress MockCommsCarrier::addressImp()
{
	return mOurAddress;
}



quint64 MockCommsCarrier::minimalPacketIntervalImp()
{
	return mMinimalPacketInterval;
}

quint64	MockCommsCarrier::maximalPacketIntervalImp()
{
	return mMaximalPacketInterval;
}
