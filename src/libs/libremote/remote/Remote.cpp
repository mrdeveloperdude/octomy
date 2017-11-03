#include "Remote.hpp"

#include "utility/Standard.hpp"
#include "comms/CommsSession.hpp"
#include "comms/CommsSessionDirectory.hpp"
#include "RemoteWindow.hpp"
#include "node/AppContext.hpp"



#include <QDebug>
#include <QDataStream>
#include <QSharedPointer>
#include <QAccelerometerReading>
#include <QGyroscopeReading>
#include <QGeoPositionInfo>


Remote::Remote(NodeLauncher<Remote> &launcher, QObject *parent)
	: Node(OC_NEW AppContext(launcher.options(), launcher.environment(), "remote", parent), ROLE_CONTROL, TYPE_REMOTE, parent)
	, window(nullptr)
	, mThis(this)
{
	OC_METHODGATE();
}

Remote::~Remote()
{
	OC_METHODGATE();
}


QWidget *Remote::showWindow()
{
	OC_METHODGATE();
	if(nullptr==window) {
		window=OC_NEW RemoteWindow(mThis, nullptr);
	}
	if(nullptr!=window) {
		window->show();
	}
	return window;
}

