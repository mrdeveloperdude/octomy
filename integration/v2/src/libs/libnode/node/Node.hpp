/* This file is distributed under the license as specified in the LICENSE file
* that accompanied it. If there was no LICENSE file accompanying this source,
* it is not to be modified, redistributed or used without prior written
* permission from original author and owner "Lennart Rolland".
*/

#ifndef NODE_HPP
#define NODE_HPP


#include "app/Settings.hpp"

#include "node/NodeRole.hpp"
#include "node/NodeType.hpp"
#include "node/LocalIdentityStore.hpp"

#include "store/DataStore.hpp"


#include "discovery/AddressBook.hpp"
#include "security/KeyStore.hpp"

#include "address/LocalAddressList.hpp"
#include "client/ClientList.hpp"
/*

#include "widgets/TryToggleState.hpp"

#include "comms/CommsCarrierUDP.hpp"
*/

#include "comms/IConnectionStatus.hpp"

#include "uptime/ConfigureHelper.hpp"

#include <QObject>
#include <QHostAddress>
#include <QUrl>

class ServiceManager;
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

class IAppLauncher;

class Associate;

class ScopedTimer;



struct NodeActivationState {
	bool keyStoreOK=false;
	bool localIdentityOK=false;
	bool localAddressesOK=false;
	bool addressBookOK=false;
	bool discoveryClientOK=false;
	bool commsCarrierOK=false;
};

/*!
 * \brief The Node class is the base class for agents, remotes and hubs.
 *
 * It holds all functionality that is common among them, including mechanisms
 * for discovering, pairing, communicating with other nodes, storage of vital
 * state such as the identity of associates, communication with OctoMY zoo and
 * more
 *
 */

class Node : public QObject, public IConnectionStatus//, public QEnableSharedFromThis<Node>
{
	Q_OBJECT

private:
	// The application launcher that launched us
	QSharedPointer<IAppLauncher> mLauncher;
	// Helper to keep track of appConfigure() and appActivate() state
	ConfigureHelper mAppConfigureHelper;
	// Manager for maintaining the lifecycle and interdependencies of services
	QSharedPointer<ServiceManager> mServiceManager;
	// Our key store
	QSharedPointer<KeyStore> mKeyStore;
	// Our local ID
	QSharedPointer<LocalIdentityStore> mLocalIdentityStore;
	// Our local network addresses
	QSharedPointer<LocalAddressList> mLocalAddresses;
	// Clients address book (clients are nodes we communicate with)
	QSharedPointer<AddressBook> mAddressBook;
	// Client instances
	ClientList mClients;
	// Our identity
	QSharedPointer<Associate> mNodeIdentity;
	// Discovery client to help in automaitc discovery of other nodes
	QSharedPointer<DiscoveryClient> mDiscovery;
	// Comms channel carrier the underlying carrier such as udb/bluetooth etc for comms
	QSharedPointer<CommsCarrier> mCarrier;
	// Comms channel used to communicate with other nodes
	QSharedPointer<CommsChannel> mComms;
	// The zoo client is used to exchange data via the central repository server (zoo)
	QSharedPointer<ZooClient> mZooClient;
	// Helper to access all sensors available via Qt's sensor API
	QSharedPointer<SensorInput> mSensors;
	// Comms Courier to manage transmission of changes in sensor data
	QSharedPointer<SensorsCourier> mSensorsCourier;
	// Comms Courier to manage transmission of blobs  such as images, files and video streams (they are treated as blobs)
	QSharedPointer<BlobCourier> mBlobCourier;
	// Camera list manages attached cameras adapting to dis- and re- connections
	QSharedPointer<CameraList> mCameras;
	// Unix ms timestamp of last sending time of status
	qint64 mLastStatusSend;
	// The URL of the zoo server
	QUrl mServerURL;
	// The current state during activation
	NodeActivationState mNodeActivationState;
	// Timer used to log how long activation takes from start to finish
	ScopedTimer *mNodeStepActivationTimer;


public:
	explicit Node();
	virtual ~Node() Q_DECL_OVERRIDE;

	// AppLauncher interface
public:

	// Called by launcher to give the app a chance to allocate data before initialization
	virtual void appConfigure(QSharedPointer<IAppLauncher> launcher);

	// Called with on=true by launcher to give the app a chance to initialize
	// After calling this with on=true, the launcher will pause further initialization and showing of windows until app emits an "appInitDone" signal

	// Called with on=false by launcher as a response to an appRequestClose event being emitted by anyone that wants the app to close.
	// After calling this with on=false, the launcher will complete termination and return to OS
	virtual void appActivate(const bool on);


	void stepActivation(const bool on);

	// Called by launcher to get a handle to the app's main window
	// Will be called when launcher wants to show window during initialization
	// NOTE: Launcher will NOT hide the window, as it is expected that app should do this by itself upon de-initialization and/or destruction
	virtual QSharedPointer<QWidget> appWindow();

	// Node interface
public:
	// Called by Node::appConfigure();
	virtual void nodeConfigure()=0;

	// Called by Node::appActivate()
	virtual void nodeActivate(const bool on)=0;

	// Called by Node::appWindow()
	virtual QSharedPointer<QWidget> nodeWindow()=0;

	// Provide role
	virtual NodeRole nodeRole()=0;

	// Provide type
	virtual NodeType nodeType()=0;


	// Selectors
public:

	// Provide launcher instance
	QSharedPointer<IAppLauncher> launcher();

	// Provide current context (adapter for context in app launcher instance)
	QSharedPointer<AppContext> context();

	// Provide the key store
	QSharedPointer<KeyStore> keyStore();

	// Provide the local address list (network addresses)
	QSharedPointer<LocalAddressList> localAddressList();

	// Convenience wrapper to get context's settings
	QSharedPointer<Settings> settings();

	// Provide local identity
	QSharedPointer<LocalIdentityStore> localIdentityStore();


	// Provide the address book
	QSharedPointer<AddressBook> addressBook();

	//Provide the client list
	// QSharedPointer<ClientList> clientList();

	// Provide the comms channel
	QSharedPointer<CommsChannel> comms();

	// Provide the discovery client
	QSharedPointer<DiscoveryClient> discoveryClient();

	// Provide the zoo client
	QSharedPointer<ZooClient> zooClient();

	// Provide the sensor input
	QSharedPointer<SensorInput> sensorInput();

	// Provide the node identity
	QSharedPointer<Associate> nodeIdentity();

	// Reset the identity and settings for this node. WARNING: This is destructive and only useful when debugging
	void unbirth();

	// Set the OctoMY™ styling to the UI of the application
	void applyStyle();

	// Create the dir in which this node keeps it's data
	bool createBaseDir();


	// Below this line is unrefined
	////////////////////////////////////////////////////////////////////////////////


public:


	void setHookColorSignals(QObject &o, bool hook);
	void setHookSensorSignals(QObject &o, bool hook);
	void setHookCommsSignals(QObject &o, bool hook);



	QString name();
	void setNodeIdentity(QVariantMap map);
	void setNodeIdentity(QSharedPointer<Associate> nodeID);
	CameraList *cameraList();


	//QSet<QSharedPointer<Associate> > allControls();
	//QSet<QSharedPointer<Associate> > controlsWithActiveSessions(quint64 now=0);

	//bool courierRegistration();
	//void  rierRegistration(QSharedPointer<Associate>, bool);
	//void updateCourierRegistration(quint64 now=0);



	//TryToggleState updateOnlineStatus(const TryToggleState currentTryState);

	virtual void identityChanged();


	virtual QSharedPointer<Node> sharedThis();



	// Couriers
public:
	// [Un]register local couriers with comms
	virtual void setNodeCouriersRegistration(const bool reg);
	// [Un]register client specific couriers with comms
	void setClientCouriersRegistration(const bool reg);
	// [Un]register couriers with comms
	void setCouriersRegistration(const bool reg);

	// Update client specific courier registration with comms depending on need
	void updateClientCouriersRegistration();
	// Update node specific courier registration with comms depending on need
	void updateNodeCouriersRegistration();
	// Update courier registration with comms depending on need
	void updateCouriersRegistration();



	// Actions
public:
	bool isCommsStarted();
	bool isCommsConnected();


	// IConnectionStatus interface
public:
	// We want to go online. We might not be online and we might
	bool needsConnection() Q_DECL_OVERRIDE;
	void setNeedsConnection(const bool) Q_DECL_OVERRIDE;
	// We are currently online. We might not need to be online
	bool isConnected() Q_DECL_OVERRIDE;
	void setConnected(const bool) Q_DECL_OVERRIDE;

	// Blob courier
	BlobFuture submitBlobForSending(QByteArray data, QString name);


	// Blob courier
signals:
	void blobReceived(QString name);
	void blobSendComplete(QString name);

	/*
	// KeyStore slots
	private slots:
	void onKeystoreReady(bool);
	*/

	// CommsChannel slots
private slots:
	virtual void onCommsError(QString);
	virtual void onCommsClientAdded(CommsSession *);
	virtual void onCommsConnectionStatusChanged(const bool isConnected, const bool needsConnection);


	// SensorInput slots
private slots:
	void onPositionUpdated(const QGeoPositionInfo &info);
	void onCompassUpdated(QCompassReading *);
	void onAccelerometerUpdated(QAccelerometerReading *);
	void onGyroscopeUpdated(QGyroscopeReading *r);

signals:
	// Expected to be emitted when someone wants node to terminate.
	// Will be caught by app launcher and app launcher will subsequently call appDeInit() to bring termination to fruition
	void nodeRequestExit(const int returnValue);

	// Expected to be emitted with on=true by node as soon as initialization is done in appInit();
	// Expected to be emitted with on=false by node during termination as soon as it no longer needs the eventloop in appDeInit().
	void nodeActivateChanged(const bool on);

};

typedef Node Nodee;


#endif
// NODE_HPP
