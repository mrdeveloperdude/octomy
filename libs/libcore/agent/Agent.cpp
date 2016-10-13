#include "Agent.hpp"
#include "AgentWindow.hpp"

#include "basic/Standard.hpp"

#include "comms/Client.hpp"
#include "comms/ClientDirectory.hpp"

#include "discovery/DiscoveryClient.hpp"
#include "zoo/ZooClient.hpp"
#include "basic/AppContext.hpp"


#include <QDebug>
#include <QDataStream>
#include <QSharedPointer>
#include <QAccelerometerReading>
#include <QGyroscopeReading>
#include <QGeoPositionInfo>


Agent::Agent(NodeLauncher<Agent> &launcher, QObject *parent)
	: Node(new AppContext(launcher.getOptions(), launcher.getEnvironment(), "agent", parent), ROLE_AGENT, TYPE_AGENT, parent)
	, window(nullptr)
{
}

Agent::~Agent(){
}



void Agent::start(const NetworkAddress &localAddress, const NetworkAddress &partnerAddress)
{
	mPartnerAddress=partnerAddress;
	if(nullptr!=mComms){
		QSharedPointer<Client> c=mComms->getClients()->getByAddress(partnerAddress,true);
		if(nullptr!=c){
			qDebug()<<"comms.start agent "<<localAddress.toString()<< " -> partner "<<partnerAddress.toString();
			mComms->start(localAddress);
		}
		else{
			qWarning()<<"ERROR: could not get client for agent";
		}
	}
}





QWidget *Agent::showWindow(){
	if(nullptr==window){
		window=new AgentWindow(this, nullptr);
	}
	if(nullptr!=window){
		window->show();
	}
	return window;
}

void Agent::onConnectionStatusChanged(TryToggleState){

}
