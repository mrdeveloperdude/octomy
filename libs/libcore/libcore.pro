TARGET =	core
TEMPLATE =	lib
CONFIG +=	staticlib
QT +=		serialport

include($$TOP_PWD/common.pri)

SOURCES +=\
	3d/Logo3DWidget.cpp \
	3d/PoseView.cpp \
	3d/scene/GeometryEngine.cpp \
	3d/scene/Limb.cpp \
	3d/scene/QtLogo3D.cpp \
	3d/scene/Simulation.cpp \
	agent/Agent.cpp \
	agent/AgentWindow.cpp \
	basic/AgentNameGenerator.cpp \
	basic/AppContext.cpp \
	basic/AsyncStore.cpp \
	basic/AtomicBoolean.cpp \
	basic/Fingerprint.cpp \
	basic/GenerateRunnable.cpp \
	basic/Iconv.cpp \
	basic/LogDestination.cpp \
	basic/LogHandler.cpp \
	basic/NetworkAddress.cpp \
	basic/NetworkOptimizer.cpp \
	basic/NodeAssociate.cpp \
	basic/Node.cpp \
	basic/NodeLauncher.cpp \
	basic/Settings.cpp \
	basic/StyleManager.cpp \
	basic/TetraGPSEncoder.cpp \
	basic/UniquePlatformFingerprint.cpp \
	basic/Vibrate.cpp \
	camera/Camera.cpp \
	camera/CameraList.cpp \
	camera/CameraSettings.cpp \
	camera/PoorMansProbe.cpp \
	comms/Client.cpp \
	comms/ClientDirectory.cpp \
	comms/ClientSignature.cpp \
	comms/CommsChannel.cpp \
	comms/couriers/Courier.cpp \
	comms/couriers/CourierMandate.cpp \
	comms/couriers/DiscoveryCourier.cpp \
	comms/couriers/SensorsCourier.cpp \
	comms/FlowControl.cpp \
	comms/messages/MessageType.cpp \
	comms/messages/SensorsMessage.cpp \
	comms/ReliabilitySystem.cpp \
	discovery/DiscoveryClient.cpp \
	discovery/DiscoveryRole.cpp \
	discovery/DiscoveryServer.cpp \
	discovery/DiscoveryServerSession.cpp \
	discovery/NodeAssociateStore.cpp \
	gear/gbody.cpp \
	gear/gconstraint.cpp \
	gear/gconstraint_jointloop.cpp \
	gear/gelement.cpp \
	gear/gjoint_composite.cpp \
	gear/gjoint.cpp \
	gear/gjoint_fixed.cpp \
	gear/gjoint_free.cpp \
	gear/gjoint_planar.cpp \
	gear/gjoint_prismatic.cpp \
	gear/gjoint_revolute.cpp \
	gear/gjoint_spherical.cpp \
	gear/gjoint_translational.cpp \
	gear/gjoint_universal.cpp \
	gear/gspringdamper.cpp \
	gear/gsystem_constrained.cpp \
	gear/gsystem.cpp \
	gear/gsystem_ik.cpp \
	gear/liegroup.cpp \
	gear/liegroup.inl \
	gear/liegroup_rmatrix3_ext.inl \
	gear/rmatrix3j.cpp \
	hub/Hub.cpp \
	hub/HubWindow.cpp \
	hw/actuators/HexyLeg.cpp \
	hw/actuators/HexySerial.cpp \
	hw/actuators/HexyTool.cpp \
	hw/actuators/SerialList.cpp \
	hw/actuators/ServoInput.cpp \
	hw/BluetoothList.cpp \
	models/ClientModel.cpp \
	plan/parser/PlanHighlighter.cpp \
	plot/NetworkStats.cpp \
	plot/qcustomplot.cpp \
	plot/StatsWindow.cpp \
	puppet/EyesWidget.cpp \
	puppet/GaitController.cpp \
	puppet/GaitWidget.cpp \
	puppet/LimbIKWidget.cpp \
	puppet/Pose.cpp \
	random/DevRandomRNG.cpp \
	random/DevURandomRNG.cpp \
	random/EfficientPRNG.cpp \
	random/FileRandomRNG.cpp \
	random/MersennePRNG.cpp \
	random/ParkMillerPRNG.cpp \
	random/RNG.cpp \
	random/SystemPRNG.cpp \
	random/TausPRNG.cpp \
	remote/Remote.cpp \
	remote/RemoteWindow.cpp \
	security/Key.cpp \
	security/KeyStore.cpp \
	security/PortableID.cpp \
	security/SecurityConstants.cpp \
	sensory/SensorInput.cpp \
	storage/Hashstore.cpp \
	storage/HashstoreRecord.cpp \
	storage/HashstoreRecordState.cpp \
	web/Mustache.cpp \
	widgets/AgentDeliveryWizard.cpp \
	widgets/CameraPairingWidget.cpp \
	widgets/CompasWidget.cpp \
	widgets/ConnectionManager.cpp \
	widgets/ConnectionWidget.cpp \
	widgets/ControlDeliveryWizard.cpp \
	widgets/EnumEntry.cpp \
	widgets/FaceWidget.cpp \
	widgets/FlowLayout.cpp \
	widgets/HardwareWizard.cpp \
	widgets/hexedit/QHexEditComments.cpp \
	widgets/hexedit/QHexEdit.cpp \
	widgets/hexedit/QHexEditData.cpp \
	widgets/hexedit/QHexEditDataDevice.cpp \
	widgets/hexedit/QHexEditDataReader.cpp \
	widgets/hexedit/QHexEditDataWriter.cpp \
	widgets/hexedit/QHexEditHighlighter.cpp \
	widgets/hexedit/QHexEditPrivate.cpp \
	widgets/hexedit/SparseRangeMap.cpp \
	widgets/HUDWidget.cpp \
	widgets/Identicon.cpp \
	widgets/IdenticonWidget.cpp \
	widgets/ImageLabel2.cpp \
	widgets/ImageLabel.cpp \
	widgets/LightWidget.cpp \
	widgets/LocalAddressEntry.cpp \
	widgets/Logo.cpp \
	widgets/LogWidget.cpp \
	widgets/MapEditor.cpp \
	widgets/MultiView.cpp \
	widgets/NumberEntry.cpp \
	widgets/PairingEditButtonDelegate.cpp \
	widgets/PairingWizard.cpp \
	widgets/PKIManager.cpp \
	widgets/planedit/CodeEditor.cpp \
	widgets/planedit/LineNumberArea.cpp \
	widgets/planedit/PlanEditor.cpp \
	widgets/PortableIDWidget.cpp \
	widgets/QRWidget.cpp \
	widgets/RealtimeValuesWidget.cpp \
	widgets/SvgWidget.cpp \
	widgets/TextEntry.cpp \
	widgets/TryToggle.cpp \
	widgets/WaitingSpinnerWidget.cpp \
	widgets/CLGLView.cpp \
	zoo/PunchRegistry.cpp \
	zoo/WebRequest.cpp \
	zoo/ZooClient.cpp \
	zoo/ZooConstants.cpp \
	zoo/ZooServer.cpp \
	hub/ClientWidget.cpp \
	hub/IContextProvider.cpp \
	widgets/NetworkSettingsWidget.cpp \
	audio/AudioStream.cpp \
	audio/OneOffSpeech.cpp \
	audio/Reverb.cpp \
	audio/Compressor.cpp \
	audio/CreepyVoice.cpp \
	audio/Tremolo.cpp \
	conscious/Consciousness.cpp \
	widgets/SpeechControlWidget.cpp \
	widgets/ActuatorControlWidget.cpp \
	widgets/TryToggleState.cpp \
	basic/GenericKeyEventHandler.cpp \
	comms/couriers/BlobCourier.cpp \
	comms/couriers/Blob.cpp \
	comms/couriers/BlobChunk.cpp \
	comms/couriers/BlobFuture.cpp \
	comms/couriers/AgentStateCourier.cpp \
	comms/messages/AgentMode.cpp \
	comms/SyncParameter.cpp \
	comms/SyncContext.cpp \
	comms/ISyncParameter.cpp \
    widgets/CarSteeringWidget.cpp \
    camera/DCT.cpp \
    camera/IDCT.cpp \
    puppet/PoseMapping.cpp \
    puppet/AgentMobilityType.cpp \
    hw/actuators/SerialSettingsWidget.cpp \
    hw/actuators/SerialSettings.cpp \
    widgets/HardwareTemplate.cpp \
    models/HardwareTemplateModel.cpp \
    models/PairingListModel.cpp \
    models/SerialDeviceListModel.cpp \
    widgets/PoseMappingWidget.cpp \
    widgets/PoseMappingView.cpp \
    puppet/PoseMappingStore.cpp \
    hw/actuators/IServoController.cpp \
    hw/actuators/Servotor32Controller.cpp \
    puppet/PoseSequence.cpp


HEADERS  += \
	3d/Logo3DWidget.hpp \
	3d/PoseView.hpp \
	3d/scene/GeometryEngine.hpp \
	3d/scene/Limb.hpp \
	3d/scene/QtLogo3D.hpp \
	3d/scene/Simulation.hpp \
	agent/Agent.hpp \
	agent/AgentWindow.hpp \
	basic/AgentNameGenerator.hpp \
	basic/AppContext.hpp \
	basic/AsyncStore.hpp \
	basic/AtomicBoolean.hpp \
	basic/Fingerprint.hpp \
	basic/GenerateRunnable.hpp \
	basic/Iconv.hpp \
	basic/LogDestination.hpp \
	basic/LogHandler.hpp \
	basic/NetworkAddress.hpp \
	basic/NetworkOptimizer.hpp \
	basic/NodeAssociate.hpp \
	basic/Node.hpp \
	basic/NodeLauncher.hpp \
	basic/Settings.hpp \
	basic/StyleManager.hpp \
	basic/TetraGPSEncoder.hpp \
	basic/UniquePlatformFingerprint.hpp \
	basic/Vibrate.hpp \
	camera/Camera.hpp \
	camera/CameraList.hpp \
	camera/CameraSettings.hpp \
	camera/PoorMansProbe.hpp \
	comms/ClientDirectory.hpp \
	comms/Client.hpp \
	comms/ClientSignature.hpp \
	comms/CommsChannel.hpp \
	comms/couriers/Courier.hpp \
	comms/couriers/CourierMandate.hpp \
	comms/couriers/DiscoveryCourier.hpp \
	comms/couriers/SensorsCourier.hpp \
	comms/DataType.hpp \
	comms/DeviceStatusType.hpp \
	comms/DeviceType.hpp \
	comms/FlowControl.hpp \
	comms/messages/MessageType.hpp \
	comms/messages/SensorsMessage.hpp \
	comms/QueryType.hpp \
	comms/ReliabilitySystem.hpp \
	discovery/DiscoveryClient.hpp \
	discovery/DiscoveryRole.hpp \
	discovery/DiscoveryServer.hpp \
	discovery/DiscoveryServerSession.hpp \
	discovery/NodeAssociateStore.hpp \
	gear/gbody.h \
	gear/gconstraint.h \
	gear/gconstraint_jointloop.h \
	gear/gcoordinate.h \
	gear/gear.h \
	gear/gelement.h \
	gear/gforce.h \
	gear/gjoint_composite.h \
	gear/gjoint_fixed.h \
	gear/gjoint_free.h \
	gear/gjoint.h \
	gear/gjoint_planar.h \
	gear/gjoint_prismatic.h \
	gear/gjoint_revolute.h \
	gear/gjoint_spherical.h \
	gear/gjoint_translational.h \
	gear/gjoint_universal.h \
	gear/greal.h \
	gear/gspringdamper.h \
	gear/gsystem_constrained.h \
	gear/gsystem.h \
	gear/gsystem_ik.h \
	gear/liegroup.h \
	gear/liegroup_rmatrix3_ext.h \
	gear/rmatrix3j.h \
	hub/Hub.hpp \
	hub/HubWindow.hpp \
	hw/actuators/HexyLeg.hpp \
	hw/actuators/HexySerial.hpp \
	hw/actuators/HexyTool.hpp \
	hw/actuators/SerialList.hpp \
	hw/actuators/ServoInput.hpp \
	hw/BluetoothList.hpp \
	models/ClientModel.hpp \
	plan/parser/PlanHighlighter.hpp \
	plot/NetworkStats.hpp \
	plot/qcustomplot.hpp \
	plot/StatsWindow.hpp \
	puppet/EyesWidget.hpp \
	puppet/GaitController.hpp \
	puppet/GaitWidget.hpp \
	puppet/LimbIKWidget.hpp \
	puppet/Pose.hpp \
	random/DevRandomRNG.hpp \
	random/DevURandomRNG.hpp \
	random/EfficientPRNG.hpp \
	random/FileRandomRNG.hpp \
	random/MersennePRNG.hpp \
	random/ParkMillerPRNG.hpp \
	random/RNG.hpp \
	random/SystemPRNG.hpp \
	random/TausPRNG.hpp \
	remote/Remote.hpp \
	remote/RemoteWindow.hpp \
	security/Key.hpp \
	security/KeyStore.hpp \
	security/PortableID.hpp \
	security/SecurityConstants.hpp \
	sensory/SensorInput.hpp \
	storage/Hashstore.hpp \
	storage/HashstoreRecord.hpp \
	storage/HashstoreRecordState.hpp \
	web/Mustache.hpp \
	widgets/AgentDeliveryWizard.hpp \
	widgets/CameraPairingWidget.hpp \
	widgets/CompasWidget.hpp \
	widgets/ConnectionManager.hpp \
	widgets/ConnectionWidget.hpp \
	widgets/ControlDeliveryWizard.hpp \
	widgets/EnumEntry.hpp \
	widgets/FaceWidget.hpp \
	widgets/FlowLayout.hpp \
	widgets/HardwareWizard.hpp \
	widgets/hexedit/QHexEditComments.hpp \
	widgets/hexedit/QHexEditDataDevice.hpp \
	widgets/hexedit/QHexEditData.hpp \
	widgets/hexedit/QHexEditDataReader.hpp \
	widgets/hexedit/QHexEditDataWriter.hpp \
	widgets/hexedit/QHexEditHighlighter.hpp \
	widgets/hexedit/QHexEdit.hpp \
	widgets/hexedit/QHexEditPrivate.hpp \
	widgets/hexedit/SparseRangeMap.hpp \
	widgets/HUDWidget.hpp \
	widgets/Identicon.hpp \
	widgets/IdenticonWidget.hpp \
	widgets/ImageLabel2.hpp \
	widgets/ImageLabel.hpp \
	widgets/LightWidget.hpp \
	widgets/LocalAddressEntry.hpp \
	widgets/Logo.hpp \
	widgets/LogWidget.hpp \
	widgets/MapEditor.hpp \
	widgets/MultiView.hpp \
	widgets/NumberEntry.hpp \
	widgets/PairingEditButtonDelegate.hpp \
	widgets/PairingWizard.hpp \
	widgets/PKIManager.hpp \
	widgets/planedit/CodeEditor.hpp \
	widgets/planedit/LineNumberArea.hpp \
	widgets/planedit/PlanEditor.hpp \
	widgets/PortableIDWidget.hpp \
	widgets/QRWidget.hpp \
	widgets/RealtimeValuesWidget.hpp \
	widgets/SvgWidget.hpp \
	widgets/TextEntry.hpp \
	widgets/TryToggle.hpp \
	widgets/WaitingSpinnerWidget.hpp \
	widgets/CLGLView.hpp \
	zoo/PunchRegistry.hpp \
	zoo/WebRequest.hpp \
	zoo/ZooClient.hpp \
	zoo/ZooConstants.hpp \
	zoo/ZooServer.hpp \
	hub/ClientWidget.hpp \
	hub/IContextProvider.hpp \
	widgets/NetworkSettingsWidget.hpp \
	audio/AudioStream.hpp \
	audio/IAudioSource.hpp \
	audio/OneOffSpeech.hpp \
	audio/Reverb.hpp \
	audio/Compressor.hpp \
	audio/CreepyVoice.hpp \
	audio/Tremolo.hpp \
	conscious/Consciousness.hpp \
	widgets/SpeechControlWidget.hpp \
	widgets/ActuatorControlWidget.hpp \
	widgets/TryToggleState.hpp \
	basic/GenericKeyEventHandler.hpp \
	comms/couriers/BlobCourier.hpp \
	comms/couriers/Blob.hpp \
	comms/couriers/BlobChunk.hpp \
	comms/couriers/BlobFuture.hpp \
	comms/couriers/AgentStateCourier.hpp \
	comms/messages/AgentMode.hpp \
	comms/SyncParameter.hpp \
	comms/SyncContext.hpp \
	comms/ISyncParameter.hpp \
    widgets/CarSteeringWidget.hpp \
    camera/DCT.hpp \
    camera/IDCT.hpp \
    puppet/PoseMapping.hpp \
    puppet/AgentMobilityType.hpp \
    hw/actuators/SerialSettingsWidget.hpp \
    hw/actuators/SerialSettings.hpp \
    widgets/HardwareTemplate.hpp \
    models/HardwareTemplateModel.hpp \
    models/PairingListModel.hpp \
    models/SerialDeviceListModel.hpp \
    widgets/PoseMappingWidget.hpp \
    widgets/PoseMappingView.hpp \
    puppet/PoseMappingStore.hpp \
    hw/actuators/IServoController.hpp \
    hw/actuators/Servotor32Controller.hpp \
    puppet/PoseSequence.hpp


FORMS    += \
	ui/AgentWindow.ui \
	ui/CameraSettings.ui \
	ui/Camera.ui \
	ui/ConnectionWidget.ui \
	ui/HexyTool.ui \
	ui/HubWindow.ui \
	ui/MultiView.ui \
	ui/NumberEntry.ui \
	ui/RemoteWindow.ui \
	ui/ResponsiveTest.ui \
	ui/StatsWindow.ui \
	ui/TryToggle.ui \
	ui/ServoInput.ui \
	ui/PlanEditor.ui \
	ui/CameraPairingWidget.ui \
	ui/MapEditor.ui \
	ui/RealtimeValuesWidget.ui \
	ui/PairingWizard.ui \
	ui/AgentDeliveryWizard.ui \
	ui/ControlDeliveryWizard.ui \
	ui/FaceWidget.ui \
	ui/LogWidget.ui \
	ui/PKIManager.ui \
	ui/PortableIDWidget.ui \
	ui/ConnectionManager.ui \
	ui/HUDWidget.ui \
	ui/HardwareWizard.ui \
	ui/ClientWidget.ui \
	widgets/NetworkSettingsWidget.ui \
	widgets/SpeechControlWidget.ui \
	widgets/ActuatorControlWidget.ui \
    widgets/CarSteeringWidget.ui \
    ui/SerialSettingsWidget.ui \
    widgets/PoseMappingWidget.ui


RESOURCES += \
	resources/icons.qrc \
	resources/fonts.qrc \
	resources/style.qrc \
	resources/3d.qrc \
	resources/data.qrc \
	resources/images.qrc



OTHER_FILES += \

#According to the link below, native widget support for Qt3D will not be available until at least Qt5.8 or later
#Untill that time comes we will hold back the qt3d enthusiasm and write generalized 3D code that is easy to port
#http://stackoverflow.com/questions/35074830/show-qt3d-stuff-inside-qwidget-in-qt5
contains(DEFINES, USE_QT3D){
HEADERS += \
	3d/qt3d/Qt3DWindow.hpp \
	3d/qt3d/SceneModifier.hpp \

SOURCES += \
	3d/qt3d/Qt3DWindow.cpp \
	3d/qt3d/SceneModifier.cpp \

}


contains(DEFINES, EXTERNAL_LIB_OPENCL){
HEADERS += \
	hub/HelloGLCLViewRenderer.hpp \
	hub/HelloCLWorkerFactory.hpp \
	hub/HelloCLWorker.hpp \

SOURCES += \
	hub/HelloGLCLViewRenderer.cpp \
	hub/HelloCLWorkerFactory.cpp \
	hub/HelloCLWorker.cpp \

}

message("FROM lib.pro:")
include($$TOP_PWD/status.pri)

