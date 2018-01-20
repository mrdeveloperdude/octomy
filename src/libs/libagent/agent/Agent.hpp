#ifndef AGENT_HPP
#define AGENT_HPP

#include "sensory/SensorInput.hpp"
#include "comms/CommsChannel.hpp"

#include "widgets/TryToggle.hpp"
#include "camera/CameraList.hpp"

#include "node/Node.hpp"
#include "node/NodeLauncher.hpp"

#include "AgentConfigStore.hpp"
#include "hw/controllers/IActuatorController.hpp"


#include <QObject>
#include <QList>
#include <QCommandLineParser>

class ZooClient;
class DiscoveryClient;
class AgentWindow;
class AgentStateCourier;
class ISyncParameter;
class Agent;


class AgentCourierSet;




class Agent : public Node, public QEnableSharedFromThis<Agent>
{
	Q_OBJECT
private:
	AgentConfigStore mAgentConfigStore;
	IActuatorController *mActuatorController;

	QSharedPointer<AgentWindow> mWindow;


	bool mKeyStoreReady;
	bool mConfigStoreReady;
	bool mAgentConfigStoreReady;


public:
	explicit Agent(NodeLauncher<Agent> &launcher, QObject *parent = nullptr);
	virtual ~Agent();

	virtual QSharedPointer<QWidget> showWindow();


public:

	void setPanic(bool);

	AgentConfigStore &configurationStore();

	QSharedPointer<PoseMapping> poseMapping();

	IActuatorController *actuatorController();
	void reloadController();


	void setNodeCouriersRegistration(bool reg) Q_DECL_OVERRIDE;

	QSharedPointer<Node> sharedThis() Q_DECL_OVERRIDE;

private:

	bool checkLoadCompleted();


	//Agent State Courier slots
public slots:
	void onSyncParameterChanged(ISyncParameter *);

};



#endif // AGENT_HPP
