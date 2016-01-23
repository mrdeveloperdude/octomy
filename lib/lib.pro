TARGET = octomy
TEMPLATE = lib

include($$TOP_PWD/common.pri)

SOURCES +=\
	basic/Settings.cpp \
	basic/Standard.cpp \
	basic/UniquePlatformFingerprint.cpp \
	comms/FlowControl.cpp \
	comms/messages/StatusMessage.cpp \
	comms/ReliabilitySystem.cpp \
	map/arrowpoint.cpp \
	map/bingapimapadapter.cpp \
	map/circlepoint.cpp \
	map/curve.cpp \
	map/emptymapadapter.cpp \
	map/fixedimageoverlay.cpp \
	map/geometry.cpp \
	map/geometrylayer.cpp \
	map/googleapimapadapter.cpp \
	map/googlemapadapter.cpp \
	map/gps_position.cpp \
	map/imagemanager.cpp \
	map/imagepoint.cpp \
	map/invisiblepoint.cpp \
	map/layer.cpp \
	map/layermanager.cpp \
	map/linestring.cpp \
	map/mapadapter.cpp \
	map/mapcontrol.cpp \
	map/maplayer.cpp \
	map/mapnetwork.cpp \
	map/openaerialmapadapter.cpp \
	map/osmmapadapter.cpp \
	map/point.cpp \
	map/tilemapadapter.cpp \
	map/wmsmapadapter.cpp \
	plot/qcustomplot.cpp \
	plot/StatsWindow.cpp \
	utility/BaseTranscode.cpp \
	utility/Status.cpp \
	utility/Utility.cpp \
	widgets/EnumEntry.cpp \
	widgets/LightWidget.cpp \
	widgets/TextEntry.cpp \
	widgets/TryToggle.cpp \
	widgets/FlowLayout.cpp \
	widgets/CompasWidget.cpp \
	widgets/Logo.cpp \
    basic/StyleManager.cpp \
    widgets/ImageLabel.cpp \
    widgets/SvgWidget.cpp \
    sensory/SensorInput.cpp \
    comms/CommsChannel.cpp \
    comms/messages/QueryMessage.cpp \
    comms/messages/QueryResultMessage.cpp \
    widgets/MultiView.cpp \
    models/ClientModel.cpp \
    hub/Client.cpp \
    hub/ClientWindow.cpp \
    hub/Hub.cpp \
    hub/HubWindow.cpp \
    remote/Remote.cpp \
    remote/RemoteWindow.cpp \
    basic/LogDestination.cpp \
    3d/PoseView.cpp \
    3d/QtLogo3D.cpp \
    widgets/IdenticonWidget.cpp


HEADERS  += \
	basic/LogDestination.hpp \
	basic/Settings.hpp \
	basic/Standard.hpp \
	basic/UniquePlatformFingerprint.hpp \
	comms/FlowControl.hpp \
	comms/messages/StatusMessage.hpp \
	comms/ReliabilitySystem.hpp \
	map/arrowpoint.h \
	map/bingapimapadapter.h \
	map/circlepoint.h \
	map/curve.h \
	map/emptymapadapter.h \
	map/fixedimageoverlay.h \
	map/geometry.h \
	map/geometrylayer.h \
	map/googleapimapadapter.h \
	map/googlemapadapter.h \
	map/gps_position.h \
	map/imagemanager.h \
	map/imagepoint.h \
	map/invisiblepoint.h \
	map/layer.h \
	map/layermanager.h \
	map/linestring.h \
	map/mapadapter.h \
	map/mapcontrol.h \
	map/maplayer.h \
	map/mapnetwork.h \
	map/openaerialmapadapter.h \
	map/osmmapadapter.h \
	map/point.h \
	map/qmapcontrol_global.h \
	map/tilemapadapter.h \
	map/wmsmapadapter.h \
	plot/qcustomplot.hpp \
	plot/StatsWindow.hpp \
	utility/BaseTranscode.hpp \
	utility/InfInt.hpp \
	utility/Status.hpp \
	utility/Utility.hpp \
	widgets/EnumEntry.hpp \
	widgets/LightWidget.hpp \
	widgets/TextEntry.hpp \
	widgets/TryToggle.hpp \
	widgets/FlowLayout.hpp \
	widgets/CompasWidget.hpp \
	widgets/Logo.hpp \
    basic/StyleManager.hpp \
    widgets/ImageLabel.hpp \
    widgets/SvgWidget.hpp \
    sensory/SensorInput.hpp \
    comms/CommsChannel.hpp \
    comms/messages/QueryMessage.hpp \
    comms/messages/QueryResultMessage.hpp \
    comms/messages/MessageType.hpp \
    comms/DataType.hpp \
    comms/DeviceStatusType.hpp \
    comms/DeviceType.hpp \
    comms/QueryType.hpp \
    widgets/MultiView.hpp \
    models/ClientModel.hpp \
    remote/RemoteWindow.hpp \
    remote/Remote.hpp \
    hub/Hub.hpp \
    hub/HubWindow.hpp \
    hub/ClientWindow.hpp \
    hub/Client.hpp \
    3d/PoseView.hpp \
    3d/QtLogo3D.hpp \
    widgets/IdenticonWidget.hpp

FORMS    += \
	ui/TryToggle.ui \
	ui/StatsWindow.ui \
	ui/ResponsiveTest.ui \
	ui/ClientWindow.ui \
    ui/RemoteWindow.ui \
    ui/HubWindow.ui \
    widgets/MultiView.ui

RESOURCES += \
	resources/icons.qrc \
    resources/fonts.qrc \
    resources/style.qrc


include(ext.pri)

message("FROM lib.pro:")
include($$TOP_PWD/status.pri)
