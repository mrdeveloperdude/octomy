#include "DiscoveryClient.hpp"

#include "utility/Standard.hpp"
#include "utility/Utility.hpp"
#include "basic/Associate.hpp"
#include "zoo/ZooClient.hpp"
#include "zoo/ZooConstants.hpp"
#include "security/KeyStore.hpp"
#include "comms/couriers/DiscoveryCourier.hpp"
#include "node/Node.hpp"
#include "comms/CommsChannel.hpp"


#include "qhttp/qhttpclient.hpp"
#include "qhttp/qhttpclientrequest.hpp"
#include "qhttp/qhttpclientresponse.hpp"

#include "security/SecurityConstants.hpp"


#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>

#include <functional>

DiscoveryClient::DiscoveryClient(Node &node)
	: QObject(&node)
	, mLastZooPair(0)
	, mClient(OC_NEW qhttp::client::QHttpClient(this))
	, mNode(node)
	, mKey(mNode.keyStore().localKey())
	  //, ourPubKey(node.getKeyStore().getLocalPublicKey())
	  //, ourID(utility::toHash(ourPubKey))
	  //, zeroID(utility::toHash(""))
{
	if(nullptr==&mNode) {
		qWarning()<<"ERROR: node was nullreference";
	}
	mTimer.setInterval(500);
	mTimer.setTimerType(Qt::VeryCoarseTimer);
	if(!connect(&mTimer,SIGNAL(timeout()),this,SLOT(onTimer()),OC_CONTYPE)) {
		qWarning()<<"ERROR: Could not connect";
	}
}

bool DiscoveryClient::isStarted()
{
	return mTimer.isActive();
}

void DiscoveryClient::start()
{
	qDebug()<<"DISCOVERY CLIENT STARTED";
	if(!mTimer.isActive()) {
		onTimer();
	}
	mTimer.start();
}

void DiscoveryClient::stop()
{
	qDebug()<<"DISCOVERY CLIENT STOPPED";
	mTimer.stop();
}




void DiscoveryClient::setURL(const QUrl& serverURL)
{
	//TODO: Investigate why this gets called twice
	mServerURL  = serverURL;
	//qDebug()<<"Setting new URL: "<<mServerURL;
}


static void merge( QVariantMap  &c, QVariantMap  &b)
{
	//QVariantMap  c(a);
	for(QVariantMap::const_iterator i=b.begin(), e=b.end() ; i!=e ; ++i) {
		c.insert(i.key(), i.value());
	}
	//return c;
}

void DiscoveryClient::discover()
{
	qDebug()<<"DISCOVERY CLIENT RUN "<<mServerURL;

	qhttp::client::TRequstHandler reqHandler= [this](qhttp::client::QHttpRequest* req) {

		auto key=mNode.keyStore().localKey();
		if(nullptr!=key) {
			QVariantMap cmd;
			cmd["action"] = ZooConstants::OCTOMY_ZOO_API_DO_DISCOVERY_ESCROW;
			cmd["key"] = key->toVariantMap(true);
			cmd["manualPin"] ="12345";
			QSharedPointer<Associate> me=mNode.nodeIdentity();
			if(nullptr!=me) {
				QVariantMap map=me->toVariantMap();
				merge(cmd, map);

				/*
								cmd["localAddress"] = "10.0.0.3";
								cmd["localPort"] = 12345;
								cmd["publicPort"] = 54321;
								cmd["name"] = node.name();
								cmd["gender"] = node
								cmd["role"] = DiscoveryRoleToString(node.role());
								cmd["type"] = DiscoveryTypeToString(node.type());
										*/
			} else {

				qWarning()<<"ERROR: no me";
			}

			//qDebug()<<"SENDING VAR: "<<cmd;
			QJsonDocument jdoc=QJsonDocument::fromVariant(cmd);
			//qDebug()<<"SENDING JDOC: "<<jdoc;
			QByteArray body  = jdoc.toJson(QJsonDocument::Indented);
			qDebug()<<"SENDING RAW JSON: "<<body;
			req->addHeader("user-agent",			ZooConstants::OCTOMY_USER_AGENT);
			req->addHeader(ZooConstants::OCTOMY_API_VERSION_HEADER,		ZooConstants::OCTOMY_API_VERSION_CURRENT);
			req->addHeader("accept",				"application/json");
			req->addHeader("content-type",			"application/json");
			req->addHeader("connection",			"keep-alive");
			req->addHeaderValue("content-length", 	body.length());
			req->end(body);
			//qDebug()<<"Getting node by OCID:"<<OCID << " REQ END";
		} else {

			qWarning()<<"ERROR: no key";
		}
	};

	qhttp::client::TResponseHandler resHandler=	[this](qhttp::client::QHttpResponse* res) {
		//qDebug()<<"RES";
		res->collectData(20000);
		res->onEnd([this, res]() {
			//qDebug()<<"RES to "<<m_serverURL;
			//qDebug()<<"RES END";
			qhttp::TStatusCode status=res->status();
			bool ok=true;
			QString message="";
			if(qhttp::ESTATUS_OK!=status) {
				ok=false;
				message="ERROR: HTTP Code was "+QString::number(status)+" instead of 200 OK";
			} else {
				QJsonDocument doc = QJsonDocument::fromJson(res->collectedData());
				QByteArray data=doc.toJson();
				QVariantMap root = QJsonDocument::fromJson(data).toVariant().toMap();
				if ( root.isEmpty() ) {
					qWarning() << "ERROR: The result is an invalid json";
					qWarning() << "ERROR: OFFENDING JSON IS: "<<data;
					message="ERROR: invalid json body in response";
					ok=false;
				} else {
					//qDebug()<<"RETURNED STATUS WAS: "<<root.value("status").toString()<<", MSG:  "<<root.value("message").toString();
					bool rok=("ok"==root.value("status").toString());
					if(!rok) {
						message=root.value("message").toString();
					}
					ok=ok && rok;
					if(ok) {
						if(root.contains("peers")) {
							//qDebug()<<"ASSOCIATES: "<<root.value("peers");
							QVariantList partList=root.value("peers").toList();
							if(partList.size()<=0) {
								qWarning()<<" + Associate count was 0 in response";
								message="ERROR: Associate count was 0 in response";
								ok=false;
							}
							//qWarning()<<" + Got "<<partList.size()<<" associates";
							for(QVariant part:partList) {
								QVariantMap map=part.toMap();
								registerPossibleAssociate(map);
							}
						} else {
							qWarning()<<" + No associates in response";
							message="ERROR: No associates in response";
							ok=false;
						}
					} else {
						qWarning()<<" + Not OK. DATA: "<<data;
					}
				}
			}
			if(!ok) {
				//qDebug()<<"RETURNED STATUS ("<<ok<<"):"<<message;
			}
		});
		//qDebug()<<"Getting node by OCID:"<<OCID << " RES DONE";
	};
	//qDebug()<<"DISCOVERY CLIENT OUTREACH TO "<<mServerURL;
	mClient->request(qhttp::EHTTP_POST, mServerURL, reqHandler, resHandler);


}

static const QString zeroID=utility::toHash("", OCTOMY_KEY_HASH);

void DiscoveryClient::registerPossibleAssociate(QVariantMap map)
{
	//qDebug()<<"REG";
	QSharedPointer<Key> key=QSharedPointer<Key>(OC_NEW Key(map["key"].toMap(),true));
	if(nullptr!=key) {
		KeyStore  &keyStore=mNode.keyStore();
		auto ourKey=keyStore.localKey();
		if(nullptr!=ourKey) {
			const QString partID=key->id();
			const QString ourID=ourKey->id();
			if(partID==zeroID) {
				qWarning()<<"ERROR: Skipping new participant with zero ID: "<<partID;
			} else if(partID==ourID) {
				//qDebug()<<" + Skipping new participant with our ID: "<<partID;
			} else {
				AddressBook &peers=mNode.peers();
				QSharedPointer<Associate> part;
				if(peers.hasAssociate(partID)) {
					qDebug()<<" + Updating participant with ID: "<<partID;
					part=peers.associateByID(partID);
					part->update(map, false);
					emit peers.associatesChanged();
					emit nodeDiscovered(partID);
				} else {
					part=QSharedPointer<Associate>(OC_NEW Associate(map));
					if(nullptr!=part) {
						if(part->isValidForClient()) {
							CommsChannel *comms=mNode.comms();
							if(nullptr!=comms) {
								DiscoveryCourier *courier=OC_NEW DiscoveryCourier(part, *comms);
								if(nullptr!=courier) {
									peers.upsertAssociate(part);
									courier->setDestination(part->id());
									comms->setCourierRegistered(*courier, true);
									qDebug()<<" + Adding new participant with ID: "<<partID;
									emit nodeDiscovered(partID);
								} else {
									qWarning()<<"ERROR: Could not create courier for part with ID "<<partID;
								}
							} else {
								qWarning()<<"ERROR: Node had no comms";
							}
						} else {
							qDebug()<<" + Deleting invalid new participant:"<<partID;
						}
					} else {
						qWarning()<<"ERROR: Could not allocate participant: "<<partID;
					}
				}
				// Update the key while we are at it...
				//TODO: Check for security implications
				keyStore.setPubKeyForID(map["key"].toMap()["publicKey"].toString());
			}
		} else {
			qWarning()<<"ERROR: no ourKey";
		}
	} else {
		qWarning()<<"ERROR: no key";
	}
}

Node &DiscoveryClient::getNode()
{
	return mNode;
}


const quint64 ZOO_PAIR_INTERVAL=20000;

void DiscoveryClient::onTimer()
{
	//qDebug()<<"PING";
	const quint64 now=QDateTime::currentMSecsSinceEpoch();
	if(now>ZOO_PAIR_INTERVAL+mLastZooPair) {
		//qDebug()<<"ZOO PAIR TIME!";
		mLastZooPair=now;
		//		discover();
		//TODO: node.getComms()->unregisterCourier(courier); <-- remove old unused and timed out couriers
	}
	discover();
}
