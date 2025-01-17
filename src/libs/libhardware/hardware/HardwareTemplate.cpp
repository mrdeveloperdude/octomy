#include "HardwareTemplate.hpp"

#include "uptime/MethodGate.hpp"

#include "uptime/New.hpp"

QVector<HardwareTemplate *> HardwareTemplate::mTemplates;

const QString HardwareTemplate::PROTOCOL_SERVOTOR32_2_1("PROTOCOL_SERVOTOR_2_1");
const QString HardwareTemplate::INTERFACE_SERVOTOR32("INTERFACE_SERVOTOR32");



HardwareTemplate::HardwareTemplate(  const QString &name, const QString &description, const QString &iconURL,const QString &iface, const QString &protocol, const PoseMapping &poseMappings)
	: mName(name)
	, mDescription(description)
	, mIconURL(iconURL)
	, mInterface(iface)
	, mProtocol(protocol)
	, mPoseMapping(poseMappings)
{
	OC_METHODGATE();
}



QString HardwareTemplate::name() const
{
	OC_METHODGATE();
	return mName;
}
QString HardwareTemplate::description() const
{
	OC_METHODGATE();
	return mDescription;
}

QString HardwareTemplate::iconURL() const
{
	OC_METHODGATE();
	return mIconURL;
}
QString HardwareTemplate::iface() const
{
	OC_METHODGATE();
	return mInterface;
}
QString HardwareTemplate::protocol() const
{
	OC_METHODGATE();
	return mProtocol;
}

quint32 HardwareTemplate::poseSize() const
{
	OC_METHODGATE();
	return mPoseMapping.size();
}

const PoseMapping HardwareTemplate::poseMapping() const
{
	OC_METHODGATE();
	return mPoseMapping;
}






QVector<HardwareTemplate *> HardwareTemplate::templates()
{
	OC_FUNCTIONGATE();
	//Only do this once
	if(mTemplates.isEmpty()) {
		//HardwareTemplate(  const QString &name, const QString &description, const QString &iconURL,const QString &interface, const QString &protocol, const PoseMapping &poseMapping);
		{
			PoseMapping pmCar(2);
			pmCar.setName(0,"Throttle");
			pmCar.setName(1,"Steering");

			HardwareTemplate *htCar=OC_NEW HardwareTemplate("RC Car","General purpose RC Car with throttle and steering as only two inputs", ":/icons/wheels.svg", "/dev/ttyACM0","servotor32 2.1", pmCar);
			mTemplates<<htCar;
		}
		{
			PoseMapping pmHexapod(18);
			int index=0;
			for(int leg=0; leg<6; ++leg) {
				for(int limb=0; limb<3; ++limb) {
					pmHexapod.setName(static_cast<quint32>(index), QString("Leg %1-%2").arg(leg).arg(limb));
					index++;
				}
			}
			HardwareTemplate *htHexapod=OC_NEW HardwareTemplate("Hexapod","General purpose hexapod (legged robot with 6 legs) each leg with 3 servos totalling 18 inputs. Enumerated in leg first, limb second, clockwize, outward starting at leg directly to the right of 'head'.", ":/icons/hexapod.svg", "/dev/ttyACM0","servotor32 2.1", pmHexapod);
			mTemplates<<htHexapod;
		}
	}
	return mTemplates;
}
