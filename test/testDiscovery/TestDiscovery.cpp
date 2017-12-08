#include "TestDiscovery.hpp"

#include "zoo/ZooServer.hpp"
#include "zoo/ZooClient.hpp"
#include "discovery/DiscoveryClient.hpp"
#include "node/NodeRole.hpp"
#include "node/Node.hpp"
#include "node/AppContext.hpp"

#include <QSignalSpy>

#include <QProcessEnvironment>
#include <QCommandLineParser>
#include <QStandardPaths>

// YOU NEED THIS: http://doc.qt.io/qt-5/qtest.html
void TestDiscovery::test(){
	qDebug()<<"INIT -----------------------";
	QProcessEnvironment env=QProcessEnvironment::systemEnvironment();
	QCommandLineParser opts;
	opts.setApplicationDescription("Test Discovery");
	opts.addHelpOption();

	AppContext *zooContext=OC_NEW AppContext(opts, env, "testDiscoveryZoo", this);
	AppContext *agentContext=OC_NEW AppContext(opts, env, "testDiscoveryAgent", this);
	AppContext *remoteContext=OC_NEW AppContext(opts, env, "testDiscoveryRemote", this);

	QSignalSpy *spyAgentKeyReady=nullptr;
	QSignalSpy *spyRemoteKeyReady=nullptr;

	if(nullptr!=zooContext){
		qDebug()<<"ZOO -----------------------";
		QString port="8123";
		ZooServer *zooServer=OC_NEW ZooServer (zooContext, this);
		QVERIFY(nullptr!=zooServer);
		zooServer->start(port);
		qDebug()<<"APP DIR: "<<QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	}

	if(nullptr!=agentContext){


		qDebug()<<"AGENT -----------------------";
		Node *testAgent=OC_NEW Node(agentContext, NodeRole::ROLE_AGENT, NodeType::TYPE_AGENT, this);
		QVERIFY(nullptr!=testAgent);
		KeyStore *agentKeystore=&testAgent->keyStore();
		QVERIFY(nullptr!=agentKeystore);
		spyAgentKeyReady=OC_NEW QSignalSpy(agentKeystore,SIGNAL(keystoreReady(bool)));
		bool conret1=connect(agentKeystore, &KeyStore::storeReady, [=](bool ok){
			qDebug()<<"AGENT KEYSTORE READY "<<ok;
			QVERIFY(agentKeystore->isReady());
			QVERIFY(!agentKeystore->hasError());
		} );
		QVERIFY(conret1);
		qDebug()<<"APP DIR: "<<QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	}

	if(nullptr!=remoteContext){
		qDebug()<<"REMOTE -----------------------";
		Node *testRemote=OC_NEW Node(remoteContext, NodeRole::ROLE_CONTROL, NodeType::TYPE_REMOTE, this);
		QVERIFY(nullptr!=testRemote);
		KeyStore *remoteKeystore=&testRemote->keyStore();
		QVERIFY(nullptr!=remoteKeystore);
		spyRemoteKeyReady=OC_NEW QSignalSpy(remoteKeystore, SIGNAL(storeReady(bool)));
		bool conret2=connect(remoteKeystore, &KeyStore::storeReady, [=](){
			qDebug()<<"REMOTE KEYSTORE READY";
			QVERIFY(remoteKeystore->isReady());
			QVERIFY(!remoteKeystore->hasError());
		} );
		QVERIFY(conret2);
		qDebug()<<"APP DIR: "<<QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	}

	qDebug()<<"DE-INIT -----------------------";
	quint64 spyWait=20000;//20 sec
	qDebug()<<"SPYTIME: "<<spyWait;

	if(nullptr!=spyAgentKeyReady){
		QVERIFY(spyAgentKeyReady->wait(spyWait));	QVERIFY(1==spyAgentKeyReady->count());
	}
	if(nullptr!=spyRemoteKeyReady){
		QVERIFY(spyRemoteKeyReady->wait(spyWait));	QVERIFY(1==spyRemoteKeyReady->count());
	}
}




OC_TEST_MAIN(test, TestDiscovery)
