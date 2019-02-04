#include "Agent.hpp"

#include "widgets/agent/AgentWindow.hpp"

#include "uptime/MethodGate.hpp"
#include "uptime/New.hpp"
#include "app/AppContext.hpp"

#include "AgentConfigStore.hpp"

#include "hardware/controllers/IActuatorController.hpp"
#include "hardware/controllers/ActuatorControllerFactory.hpp"

#include <QDebug>

Agent::Agent(QObject *parent)
	: Node(parent)
	, mAgentConfigStore(OC_NEW AgentConfigStore())
{
	OC_METHODGATE();
	qDebug()<<"Agent()";
	// NOTE: Please do not put code here that generates events. Instead put them in init()
}

Agent::~Agent()
{
	OC_METHODGATE();
	qDebug()<<"~Agent()";
}


void Agent::nodeConfigure()
{
	OC_METHODGATE();
	qDebug()<<"nodeConfigure()";
}



void Agent::nodeInit()
{
	OC_METHODGATE();
	qDebug()<<"nodeInit()";
	auto ctx=context();
	if(!ctx.isNull()) {
		// Initialize Agent Configuration Store
		mAgentConfigStore->configure(ctx->baseDir()+ "/agent_config.json");

		mAgentConfigStore->setInitialized(mAgentConfigStore.data());
		mAgentConfigStore->synchronize([](SimpleDataStore &store, bool ok) {
			Q_UNUSED(store);
			Q_UNUSED(ok);
		});
		emit nodeInitDone();
	} else {
		qDebug()<<"ERROR: No context, closing";
		emit nodeRequestClose();
	}
}


void Agent::nodeDeInit()
{
	OC_METHODGATE();
	qDebug()<<"nodeDeInit()";
	emit nodeDeInitDone();
}


QSharedPointer<QWidget> Agent::nodeWindow()
{
	OC_METHODGATE();
	qDebug()<<"nodeWindow()";
	if(mWindow.isNull()) {
		QSharedPointer<Agent> sp=this->QEnableSharedFromThis<Agent>::sharedFromThis();
		if(sp.isNull()) {
			volatile int ba=0;
			qWarning()<<"SHARED POINTER TO THIS WAS NULL!"<<ba;
		}
		mWindow=QSharedPointer<AgentWindow>(OC_NEW AgentWindow(nullptr));
		if(!mWindow.isNull()) {
			mWindow->nodeWindowConfigure(sp);
		}
		else{
			qWarning()<<"ERROR: Could not allpcate AgentWindow";
		}
	}
	return mWindow;
}



NodeRole Agent::nodeRole()
{
	OC_METHODGATE();
	return ROLE_AGENT;
}


NodeType Agent::nodeType()
{
	OC_METHODGATE();
	return TYPE_AGENT;
}


QSharedPointer<Node> Agent::sharedThis()
{
	OC_METHODGATE();
	return sharedFromThis();
}


void Agent::setPanic(bool panic)
{
	OC_METHODGATE();
	if(mPanic!=panic) {
		mPanic=panic;
		qWarning()<<"Panic set to: "<<mPanic;
	}
}


bool Agent::panic()
{
	OC_METHODGATE();
	return mPanic;
}



QSharedPointer<AgentConfigStore> Agent::configurationStore()
{
	OC_METHODGATE();
	return mAgentConfigStore;
}



QSharedPointer<IActuatorController> Agent::actuatorController()
{
	OC_METHODGATE();
	return mActuatorController;
}

void Agent::unloadController()
{
	OC_METHODGATE();
	// Unload old controller
	if(nullptr != mActuatorController) {
		//qDebug()<<"Unloading old actuator controller";
		mActuatorController->setConnected(false);
		mActuatorController->deleteLater();
		mActuatorController=nullptr;
	}

}

void Agent::reloadController()
{
	OC_METHODGATE();
	unloadController();
	if(!mAgentConfigStore.isNull()) {
		QSharedPointer<AgentConfig> config=mAgentConfigStore->agentConfig();
		if(!config.isNull()) {
			QString controllerName=config->controllerName().trimmed();
			if(!controllerName.isEmpty()) {
				//qDebug()<<"Attempting to generate actuator controller of type "<<controllerName;
				ActuatorControllerFactory factory;
				mActuatorController=factory.controllerFactory(controllerName);
				if(!mActuatorController.isNull()) {
					//qDebug()<<"Actuator controller created, configuring";
					QVariantMap controllerConfig=config->controllerConfig();
					mActuatorController->setConfiguration(controllerConfig);
					mActuatorController->setConnected(true);
				}
			} else {
				qDebug()<<"ERROR: No actuator controller named in agent config";
			}
		} else {
			qDebug()<<"ERROR: No agent config";
		}
	} else {
		qDebug()<<"ERROR: No agent config store";
	}
}
