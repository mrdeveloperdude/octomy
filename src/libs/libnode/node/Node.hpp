/* This file is distributed under the license as specified in the LICENSE file
* that accompanied it. If there was no LICENSE file accompanying this source,
* it is not to be modified, redistributed or used without prior written
* permission from original author and owner "Lennart Rolland".
*/

#ifndef NODE_HPP
#define NODE_HPP

#include "comms/CommsCarrierUDP.hpp"

#include "discovery/DiscoveryRole.hpp"
#include "discovery/AddressBook.hpp"

#include "basic/Settings.hpp"
#include "security/KeyStore.hpp"
#include "basic/LocalAddressList.hpp"

#include <QObject>
#include <QHostAddress>
#include <QUrl>

class CommsCarrier;
class CommsChannel;
class ZooClient;
class DiscoveryClient;


class SensorInput;
class CameraList;
class SensorsMessage;
class QCommandLineParser;

class QGeoPositionInfo;
class QCompassReading;
class QAccelerometerReading;
class QGyroscopeReading;
class CommsSession;
class AppContext;
class SensorsCourier;
class BlobCourier;
class BlobFuture;


class Node : public QObject
{
	Q_OBJECT
protected:

	AppContext *mContext;
	KeyStore mKeystore;
	AddressBook mAssociates;

	DiscoveryClient *mDiscovery;
	DiscoveryRole mRole;
	DiscoveryType mType;
	CommsCarrier *mCarrier;
	CommsChannel *mComms;
	ZooClient *mZooClient;
	SensorInput *mSensors;

	SensorsCourier *mSensorsCourier;
	BlobCourier *mBlobCourier;

	//NetworkAddress mPartnerAddress;
	CameraList *mCameras;

	qint64 mLastStatusSend;
//	SensorsMessage *mSensorMessage;

	QUrl mServerURL;

	LocalAddressList mAddresses;


public:
	explicit Node(AppContext *context, DiscoveryRole mRole, DiscoveryType mType, QObject *parent = nullptr);
	virtual ~Node();

public:

	void init();
	void deInit();

	void updateDiscoveryClient();

	void hookColorSignals(QObject &o);
	void unHookColorSignals(QObject &o);

	void hookSensorSignals(QObject &o);
	void unHookSensorSignals(QObject &o);

	void setHookCommsSignals(QObject &o, bool hook);

	void hookPeerSignals(QObject &o);
	void unHookPeerSignals(QObject &o);

	// Selectors
public:
	const QCommandLineParser &commandLine() const;
	Settings &settings();
	KeyStore &keyStore();
	AddressBook &peers();
	DiscoveryClient *discoveryClient();
	DiscoveryRole role();
	DiscoveryType type();
	QString name();
	CommsChannel *comms();
	ZooClient *zooClient();
	SensorInput *sensorInput();

	QSharedPointer<Associate> nodeIdentity();

	CameraList *cameras();

	// Actions
public:

	virtual QWidget *showWindow();

	NetworkAddress localAddress();
	LocalAddressList &localAddresses();
	void startComms();
	void stopComms();
	bool isCommsStarted();
	bool isCommsConnected();

	// Blob courier
	BlobFuture submitBlobForSending(QByteArray data, QString name);


	// Blob courier
signals:
	void blobReceived(QString name);
	void blobSendComplete(QString name);

	// KeyStore slots
private slots:
	void onKeystoreReady(bool);

	// CommsChannel slots
private slots:
	virtual void onCommsError(QString);
	virtual void onCommsClientAdded(CommsSession *);
	virtual void onCommsConnectionStatusChanged(bool);


	// SensorInput slots
private slots:
	void onPositionUpdated(const QGeoPositionInfo &info);
	void onCompassUpdated(QCompassReading *);
	void onAccelerometerUpdated(QAccelerometerReading *);
	void onGyroscopeUpdated(QGyroscopeReading *r);

};

#endif // NODE_HPP
