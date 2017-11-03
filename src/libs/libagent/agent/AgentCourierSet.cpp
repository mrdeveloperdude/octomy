#include "AgentCourierSet.hpp"

#include "Agent.hpp"
#include "comms/CommsSignature.hpp"
#include "comms/couriers/AgentStateCourier.hpp"
#include "comms/couriers/SensorsCourier.hpp"
#include "comms/couriers/blob/BlobCourier.hpp"


AgentCourierSet::AgentCourierSet(QString &fullID, Agent &agent)
	: CourierSet()
	, mAgent(agent)
	, mAgentStateCourier(nullptr)
	, mSensorsCourier(nullptr)
	, mBlobCourier(nullptr)
{
	OC_METHODGATE();

	CommsChannel *cc=agent.comms();
	if(nullptr!=cc) {
		mAgentStateCourier=QSharedPointer<AgentStateCourier>(OC_NEW AgentStateCourier(nullptr , *cc, &agent));
		mSensorsCourier=QSharedPointer<SensorsCourier>(OC_NEW SensorsCourier(*cc, &agent));
		mBlobCourier=QSharedPointer<BlobCourier>(OC_NEW BlobCourier(*cc, &agent));
	} else {
		qWarning()<<"ERROR: ClientWidget did not have commschannel";
	}

	if(!mAgentStateCourier.isNull()) {
		mAgentStateCourier->setHookSignals(mAgent, true);
		mAgentStateCourier->setDestination(fullID);
		append(mAgentStateCourier);
	} else {
		qWarning()<<"ERROR: Could not allocate AgentStateCourier";
	}
	if(!mSensorsCourier.isNull()) {
		mSensorsCourier->setDestination(fullID);
		append(mSensorsCourier);
	} else {
		qWarning()<<"ERROR: Could not allocate SensorsCourier";
	}
	if(!mBlobCourier.isNull()) {
		mBlobCourier->setDestination(fullID);
		append(mBlobCourier);
	} else {
		qWarning()<<"ERROR: Could not allocate BlobCourier";
	}
}




AgentCourierSet::~AgentCourierSet()
{
	OC_METHODGATE();
}



QSharedPointer<AgentStateCourier> AgentCourierSet::agentStateCourier()
{
	OC_METHODGATE();
	return mAgentStateCourier;
}
