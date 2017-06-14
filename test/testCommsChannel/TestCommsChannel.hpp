#ifndef TESTCOMMSCHANNEL_HPP
#define TESTCOMMSCHANNEL_HPP

#include "comms/CommsChannel.hpp"


#include <QTest>
#include <QHostAddress>



class CommsSignalLogger: public QObject
{
	Q_OBJECT

private:
	QString mName;
public:
	explicit CommsSignalLogger(QString name, QObject *parent=nullptr);
	virtual ~CommsSignalLogger();

public slots:
	void onCommsError(QString message);
	void onCommsClientAdded(CommsSession *c);
	void onCommsConnectionStatusChanged(bool c);

};



class CommsTester;
class TestCourier:public Courier
{
	Q_OBJECT

public:

	CommsTester *mCt;
	quint16 mSoFar;

	const QByteArray mDatagram;
	const qint32 mMaxSends;
	const qint32 mMaxRecs;
	qint32 mSendCount;
	qint32 mRecCount;
	CourierMandate mMandate;

public:
	explicit TestCourier(QString dest, QByteArray datagram, CommsChannel &comms, const qint32 mMaxSends=1, const qint32 mMaxRecs=1, CommsTester *parent = nullptr);
	virtual ~TestCourier();

public:

	//Let the CommChannel know what we want
	CourierMandate mandate() const Q_DECL_OVERRIDE;

	//Override to act on sending opportunity.
	//Return nubmer of bytes sent ( >0 ) if you took advantage of the opportunity
	quint16 sendingOpportunity(QDataStream &ds)	Q_DECL_OVERRIDE;

	//Override to act on data received
	//Return number of bytes actually read.
	quint16 dataReceived(QDataStream &ds, quint16 availableBytes) Q_DECL_OVERRIDE;

	void writeSummary();
};



class RNG;
class CommsTester:public QObject
{
	Q_OBJECT

public:
	QString mName;
	QHostAddress mMyAddress;
	quint16 mMyPort;


	quint16 mBasePort;
	quint16 mPortRange;
//		ClientSignature sig;
	KeyStore &mKeyStore;
	CommsChannel mCc;
	quint16 mTestCount;

	RNG *mRng;


public:
	explicit CommsTester(QString name, QHostAddress myAddress, quint16 myPort, quint16 basePort, quint16 portRange, quint16 testCount, KeyStore &keyStore, QObject *parent=nullptr);
	virtual ~CommsTester() {}
	QString toString();

public slots:
	void startSendTest();
	void onError(QString);
	void onReadyRead();
signals:
	void finished();

};

class TestCommsChannel:public QObject
{
	Q_OBJECT


	void testMultiple();
private slots:
	void testSingle();



};


#endif // TESTCOMMSCHANNEL_HPP
