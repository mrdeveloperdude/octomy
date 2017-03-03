#ifndef AGENT_HPP
#define AGENT_HPP

#include "sensory/SensorInput.hpp"
#include "comms/CommsChannel.hpp"

#include "widgets/TryToggle.hpp"
#include "camera/CameraList.hpp"

#include "basic/Node.hpp"
#include "basic/NodeLauncher.hpp"

#include "agent/AgentConfigStore.hpp"
#include "hw/controllers/IActuatorController.hpp"

#include "AgentControls.hpp"

#include <QObject>
#include <QList>
#include <QCommandLineParser>

class ZooClient;
class DiscoveryClient;
class AgentWindow;
class AgentStateCourier;
class ISyncParameter;
class Agent;


class CourierSet;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////







////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



class Agent : public Node
{
	Q_OBJECT
private:
	AgentControls mControls;
	AgentConfigStore mAgentConfigStore;
	IActuatorController *mActuatorController;
	AgentWindow *mWindow;

public:
	explicit Agent(NodeLauncher<Agent> &launcher, QObject *parent = nullptr);
	virtual ~Agent();

	virtual QWidget *showWindow();

	void setPanic(bool);


	const AgentControls &controls() const ;

	AgentConfigStore &configurationStore();

	QSharedPointer<PoseMapping> poseMapping();

	IActuatorController *actuatorController();
	void reloadController();

	// Agent Config Store slots
public slots:
	void onAgentConfigStoreReady(bool);

	// NodeAssociate slots
public slots:
	void onPeerStoreReady(bool);
	void onPeerAdded(QString);
	void onPeerRemoved(QString);
	void onPeersChanged();

	//Agent State Courier slots
public slots:
	void onSyncParameterChanged(ISyncParameter *);

	// CommsChannel slots
private slots:
	virtual void onCommsError(QString) Q_DECL_OVERRIDE;
	virtual void onCommsClientAdded(CommsSession *)Q_DECL_OVERRIDE;
	virtual void onCommsConnectionStatusChanged(bool)Q_DECL_OVERRIDE;



};



#endif // AGENT_HPP
