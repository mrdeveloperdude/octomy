#include "CommsTester.hpp"

#include "rng/RNG.hpp"
#include "TestCourier.hpp"

CommsTester::CommsTester(QString name, QHostAddress myAddress, quint16 myPort, quint16 basePort, quint16 portRange, quint16 testCount, KeyStore &keyStore, NodeAssociateStore &peers, QObject *parent)
	: QObject(parent)
	, mName(name)
	, mMyAddress(myAddress)
	, mMyPort(myPort)
	, mBasePort(basePort)
	, mPortRange(portRange)
	, mCarrier()
	, mKeyStore(keyStore)
	, mPeers(peers)
	, mCc(mCarrier, mKeyStore, mPeers)
	, mTestCount(testCount)
	, mRng(RNG::sourceFactory("mt"))
{
	QVERIFY(nullptr!=mRng);
	mRng->init(mMyPort);
	QVERIFY(mMyPort>=mBasePort);
	QVERIFY(mMyPort<=mBasePort+mPortRange);
	QVERIFY(connect(&mCc,SIGNAL(commsError(QString)),this,SLOT(onError(QString)),OC_CONTYPE));
	for(quint16 toPort=mBasePort; toPort<mBasePort+mPortRange; ++toPort) {
		if(mMyPort==toPort) {
			continue;
		}
		if(mRng->generateReal2()>0.7 || true) {
			qDebug() << mMyAddress << ":" << mMyPort << " --> " << toPort;
			QString myID="1234";
			//CommsSignature sig(myID, NetworkAddress(mMyAddress, toPort));
			TestCourier *tc=OC_NEW TestCourier(mName+"Courier", myID, "This is my humble payload", mCc, mTestCount, mTestCount, this);
			QVERIFY(nullptr!=tc);
			mCc.setCourierRegistered(*tc, true);
		} else {
			qDebug() << mMyAddress << ":" << mMyPort << " --> SKIPPED";
		}
	}
}


void CommsTester::onError(QString e)
{
	qWarning()<<toString()<<"::onError(): "<<e;
}



void CommsTester::onReadyRead()
{
	qDebug()<<toString()<<"::onReadyRead()";
}


void CommsTester::startSendTest()
{
	qDebug()<<"Starting test for "<<toString();
	mCc.start(NetworkAddress(mMyAddress, mMyPort));
}


QString CommsTester::toString()
{
	QString ret=mName;
	ret+="(";
	ret+=mMyAddress.toString();
	ret+=":";
	ret+=QString::number(mMyPort);
	ret+=")";
	return ret;
}



