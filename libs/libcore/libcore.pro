TARGET =	core
TEMPLATE =	lib
CONFIG +=	staticlib
QT +=		serialport

include($$TOP_PWD/common.pri)

SOURCES +=\
	agent/AgentConstants.cpp \
	agent/Agent.cpp \
	agent/AgentWindow.cpp \
	basic/AgentNameGenerator.cpp \
	basic/AppContext.cpp \
	basic/AsyncStore.cpp \
	basic/AtomicBoolean.cpp \
	basic/Fingerprint.cpp \
	basic/GenerateRunnable.cpp \
	basic/GenericKeyEventHandler.cpp \
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
	camera/DCT.cpp \
	camera/IDCT.cpp \
	camera/PoorMansProbe.cpp \
	comms/CommsChannel.cpp \
	comms/CommsSession.cpp \
	comms/CommsSessionDirectory.cpp \
	comms/CommsSignature.cpp \
	comms/couriers/AgentStateCourier.cpp \
	comms/couriers/BlobChunk.cpp \
	comms/couriers/BlobCourier.cpp \
	comms/couriers/Blob.cpp \
	comms/couriers/BlobFuture.cpp \
	comms/couriers/Courier.cpp \
	comms/couriers/CourierMandate.cpp \
	comms/couriers/DiscoveryCourier.cpp \
	comms/couriers/SensorsCourier.cpp \
	comms/FlowControl.cpp \
	comms/ISyncParameter.cpp \
	comms/messages/AgentMode.cpp \
	comms/messages/MessageType.cpp \
	comms/messages/SensorsMessage.cpp \
	comms/ReliabilitySystem.cpp \
	comms/SyncContext.cpp \
	comms/SyncParameter.cpp \
	conscious/Consciousness.cpp \
	discovery/DiscoveryClient.cpp \
	discovery/DiscoveryRole.cpp \
	discovery/DiscoveryServer.cpp \
	discovery/DiscoveryServerSession.cpp \
	discovery/NodeAssociateStore.cpp \
	hub/ClientWidget.cpp \
	hub/Hub.cpp \
	hub/HubWindow.cpp \
	hub/IContextProvider.cpp \
	models/ClientModel.cpp \
	models/HardwareTemplateModel.cpp \
	models/PairingListModel.cpp \
	models/SerialDeviceListModel.cpp \
	plan/parser/PlanHighlighter.cpp \
	plot/NetworkStats.cpp \
	plot/qcustomplot.cpp \
	plot/StatsWindow.cpp \
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
	widgets/ActuatorManagerWidget.cpp \
	widgets/ActuatorSelector.cpp \
	widgets/AgentDeliveryWizard.cpp \
	widgets/CameraPairingWidget.cpp \
	widgets/CarSteeringWidget.cpp \
	widgets/CLGLView.cpp \
	widgets/CompasWidget.cpp \
	widgets/ConnectionManager.cpp \
	widgets/ConnectionWidget.cpp \
	widgets/ControlDeliveryWizard.cpp \
	widgets/EnumEntry.cpp \
	widgets/FaceWidget.cpp \
	widgets/FlowLayout.cpp \
	widgets/HardwareTemplate.cpp \
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
	widgets/LocusManagerWidget.cpp \
	widgets/Logo.cpp \
	widgets/LogWidget.cpp \
	widgets/MapEditor.cpp \
	widgets/MultiView.cpp \
	widgets/NetworkSettingsWidget.cpp \
	widgets/NumberEntry.cpp \
	widgets/PairingEditButtonDelegate.cpp \
	widgets/PairingWizard.cpp \
	widgets/PanicButton.cpp \
	widgets/PKIManager.cpp \
	widgets/planedit/CodeEditor.cpp \
	widgets/planedit/LineNumberArea.cpp \
	widgets/planedit/PlanEditor.cpp \
	widgets/PortableIDWidget.cpp \
	widgets/QRWidget.cpp \
	widgets/RealtimeValuesWidget.cpp \
	widgets/SpeechControlWidget.cpp \
	widgets/SvgWidget.cpp \
	widgets/TestWidget.cpp \
	widgets/TextEntry.cpp \
	widgets/TryToggle.cpp \
	widgets/TryToggleState.cpp \
	widgets/WaitingSpinnerWidget.cpp \
	zoo/PunchRegistry.cpp \
	zoo/WebRequest.cpp \
	zoo/ZooClient.cpp \
	zoo/ZooConstants.cpp \
	zoo/ZooServer.cpp \
	widgets/ArduinoPinMap.cpp \
	widgets/ArduinoPinCombobox.cpp \
	widgets/ArduinoPinSelector.cpp \
	widgets/ArduinoPinFacilitator.cpp \
	widgets/ArduinoPinFilter.cpp \
	widgets/ArduinoPin.cpp \
	hw/controllers/ardumy/ArduMYActuatorWidget.cpp \
	hw/controllers/ardumy/ArduMYController.cpp \
	hw/controllers/ardumy/ArduMYControllerWidget.cpp \
	hw/controllers/servotor32/HexyLeg.cpp \
	hw/controllers/servotor32/HexySerial.cpp \
	locus/Locus.cpp \
	hw/serial/SerialList.cpp \
	hw/serial/SerialSettings.cpp \
	hw/serial/SerialSettingsWidget.cpp \
	hw/controllers/servotor32/Servotor32Controller.cpp \
	locus/WheeledLocus.cpp \
	locus/WheeledLocusWidget.cpp \
	hw/controllers/ActuatorControllerFactory.cpp \
	hw/controllers/IActuatorController.cpp \
	hw/bluetooth/BluetoothList.cpp \
	agent/AgentConfig.cpp \
	agent/AgentConfigStore.cpp \
	agent/AgentMobilityType.cpp \
	hw/controllers/servotor32/Servotor32ActuatorWidget.cpp \
	hw/controllers/servotor32/Servotor32ControllerWidget.cpp \
	locus/LocusController.cpp \
	locus/LocusSet.cpp \
	locus/LocusWidget.cpp \
	locus/LeggedLocusWidget.cpp \
	locus/LeggedLocus.cpp \
	agent/CourierSet.cpp \
	agent/AgentControls.cpp \
	locus/LocusFactory.cpp \
	locus/HoveringLocus.cpp \
	locus/HoveringLocusWidget.cpp \
	locus/TrackedLocus.cpp \
	locus/TrackedLocusWidget.cpp \
	hw/controllers/ardumy/ArduMYTypeConversions.cpp \
	models/TriggerListModel.cpp \
	models/TestListModel.cpp \
	widgets/TriggerManagerWidget.cpp \
	trigger/Trigger.cpp \
	trigger/Action.cpp \
	trigger/Condition.cpp \
	trigger/TriggerSet.cpp \
	widgets/TriggerWidget.cpp \



HEADERS  += \
	agent/AgentConstants.hpp \
	agent/Agent.hpp \
	agent/AgentWindow.hpp \
	basic/AgentNameGenerator.hpp \
	basic/AppContext.hpp \
	basic/AsyncStore.hpp \
	basic/AtomicBoolean.hpp \
	basic/Fingerprint.hpp \
	basic/GenerateRunnable.hpp \
	basic/GenericKeyEventHandler.hpp \
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
	camera/DCT.hpp \
	camera/IDCT.hpp \
	camera/PoorMansProbe.hpp \
	comms/CommsChannel.hpp \
	comms/CommsSessionDirectory.hpp \
	comms/CommsSession.hpp \
	comms/CommsSignature.hpp \
	comms/couriers/AgentStateCourier.hpp \
	comms/couriers/BlobChunk.hpp \
	comms/couriers/BlobCourier.hpp \
	comms/couriers/BlobFuture.hpp \
	comms/couriers/Blob.hpp \
	comms/couriers/Courier.hpp \
	comms/couriers/CourierMandate.hpp \
	comms/couriers/DiscoveryCourier.hpp \
	comms/couriers/SensorsCourier.hpp \
	comms/FlowControl.hpp \
	comms/ISyncParameter.hpp \
	comms/messages/AgentMode.hpp \
	comms/messages/MessageType.hpp \
	comms/messages/SensorsMessage.hpp \
	comms/ReliabilitySystem.hpp \
	comms/SyncContext.hpp \
	comms/SyncParameter.hpp \
	conscious/Consciousness.hpp \
	discovery/DiscoveryClient.hpp \
	discovery/DiscoveryRole.hpp \
	discovery/DiscoveryServer.hpp \
	discovery/DiscoveryServerSession.hpp \
	discovery/NodeAssociateStore.hpp \
	hub/ClientWidget.hpp \
	hub/Hub.hpp \
	hub/HubWindow.hpp \
	hub/IContextProvider.hpp \
	models/ClientModel.hpp \
	models/HardwareTemplateModel.hpp \
	models/PairingListModel.hpp \
	models/SerialDeviceListModel.hpp \
	plan/parser/PlanHighlighter.hpp \
	plot/NetworkStats.hpp \
	plot/qcustomplot.hpp \
	plot/StatsWindow.hpp \
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
	widgets/ActuatorManagerWidget.hpp \
	widgets/ActuatorSelector.hpp \
	widgets/AgentDeliveryWizard.hpp \
	widgets/CameraPairingWidget.hpp \
	widgets/CarSteeringWidget.hpp \
	widgets/CLGLView.hpp \
	widgets/CompasWidget.hpp \
	widgets/ConnectionManager.hpp \
	widgets/ConnectionWidget.hpp \
	widgets/ControlDeliveryWizard.hpp \
	widgets/EnumEntry.hpp \
	widgets/FaceWidget.hpp \
	widgets/FlowLayout.hpp \
	widgets/HardwareTemplate.hpp \
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
	widgets/LocusManagerWidget.hpp \
	widgets/Logo.hpp \
	widgets/LogWidget.hpp \
	widgets/MapEditor.hpp \
	widgets/MultiView.hpp \
	widgets/NetworkSettingsWidget.hpp \
	widgets/NumberEntry.hpp \
	widgets/PairingEditButtonDelegate.hpp \
	widgets/PairingWizard.hpp \
	widgets/PanicButton.hpp \
	widgets/PKIManager.hpp \
	widgets/planedit/CodeEditor.hpp \
	widgets/planedit/LineNumberArea.hpp \
	widgets/planedit/PlanEditor.hpp \
	widgets/PortableIDWidget.hpp \
	widgets/QRWidget.hpp \
	widgets/RealtimeValuesWidget.hpp \
	widgets/SpeechControlWidget.hpp \
	widgets/SvgWidget.hpp \
	widgets/TestWidget.hpp \
	widgets/TextEntry.hpp \
	widgets/TryToggle.hpp \
	widgets/TryToggleState.hpp \
	widgets/WaitingSpinnerWidget.hpp \
	zoo/PunchRegistry.hpp \
	zoo/WebRequest.hpp \
	zoo/ZooClient.hpp \
	zoo/ZooConstants.hpp \
	zoo/ZooServer.hpp \
	widgets/ArduinoPinMap.hpp \
	widgets/ArduinoPinCombobox.hpp \
	widgets/ArduinoPinSelector.hpp \
	widgets/ArduinoPinFacilitator.hpp \
	widgets/ArduinoPinFilter.hpp \
	widgets/ArduinoPin.hpp \
	hw/controllers/ActuatorControllerFactory.hpp \
	hw/controllers/ardumy/ArduMYControllerWidget.hpp \
	hw/controllers/ardumy/ArduMYController.hpp \
	hw/controllers/servotor32/HexyLeg.hpp \
	hw/controllers/servotor32/HexySerial.hpp \
	locus/Locus.hpp \
	hw/serial/SerialList.hpp \
	hw/serial/SerialSettings.hpp \
	hw/serial/SerialSettingsWidget.hpp \
	hw/controllers/servotor32/Servotor32Controller.hpp \
	locus/WheeledLocus.hpp \
	locus/WheeledLocusWidget.hpp \
	hw/controllers/IActuatorController.hpp \
	hw/controllers/ardumy/ArduMYActuatorWidget.hpp \
	hw/bluetooth/BluetoothList.hpp \
	agent/AgentConfig.hpp \
	agent/AgentConfigStore.hpp \
	agent/AgentMobilityType.hpp \
	hw/controllers/servotor32/Servotor32ActuatorWidget.hpp \
	hw/controllers/servotor32/Servotor32ControllerWidget.hpp \
	locus/LocusController.hpp \
	locus/LocusSet.hpp \
	locus/LocusWidget.hpp \
	locus/LeggedLocusWidget.hpp \
	locus/LeggedLocus.hpp \
	agent/CourierSet.hpp \
	agent/AgentControls.hpp \
	locus/LocusFactory.hpp \
	locus/HoveringLocus.hpp \
	locus/HoveringLocusWidget.hpp \
	locus/TrackedLocus.hpp \
	locus/TrackedLocusWidget.hpp \
	hw/controllers/ardumy/ArduMYTypeConversions.hpp \
	models/TriggerListModel.hpp \
	models/TestListModel.hpp \
	widgets/TriggerManagerWidget.hpp \
	trigger/Trigger.hpp \
	trigger/Action.hpp \
	trigger/Condition.hpp \
	trigger/TriggerSet.hpp \
	widgets/TriggerWidget.hpp \



FORMS    += \
	locus/HoveringLocusWidget.ui \
	locus/LeggedLocusWidget.ui \
	locus/TrackedLocusWidget.ui \
	ui/ActuatorManagerWidget.ui \
	ui/AgentDeliveryWizard.ui \
	ui/AgentWindow.ui \
	ui/ArduMYActuatorWidget.ui \
	ui/ArduMYControllerWidget.ui \
	ui/CameraPairingWidget.ui \
	ui/CameraSettings.ui \
	ui/Camera.ui \
	ui/CarSteeringWidget.ui \
	ui/ClientWidget.ui \
	ui/ConnectionManager.ui \
	ui/ConnectionWidget.ui \
	ui/ControlDeliveryWizard.ui \
	ui/FaceWidget.ui \
	ui/HardwareWizard.ui \
	ui/HubWindow.ui \
	ui/HUDWidget.ui \
	ui/LocusManagerWidget.ui \
	ui/LogWidget.ui \
	ui/MapEditor.ui \
	ui/MultiView.ui \
	ui/NetworkSettingsWidget.ui \
	ui/NumberEntry.ui \
	ui/PairingWizard.ui \
	ui/PanicButton.ui \
	ui/PKIManager.ui \
	ui/PlanEditor.ui \
	ui/PortableIDWidget.ui \
	ui/RealtimeValuesWidget.ui \
	ui/RemoteWindow.ui \
	ui/ResponsiveTest.ui \
	ui/SerialSettingsWidget.ui \
	ui/Servotor32ActuatorWidget.ui \
	ui/Servotor32ControllerWidget.ui \
	ui/SpeechControlWidget.ui \
	ui/StatsWindow.ui \
	ui/TryToggle.ui \
	ui/WheeledLocusWidget.ui \
	widgets/ArduinoPinSelector.ui \
	widgets/TestWidget.ui \
	widgets/TriggerManagerWidget.ui \
	widgets/TriggerWidget.ui \


RESOURCES += \
	resources/3d.qrc \
	resources/data.qrc \
	resources/fonts.qrc \
	resources/icons.qrc \
	resources/images.qrc \
	resources/style.qrc \




OTHER_FILES += \

#According to the link below, native widget support for Qt3D will not be available until at least Qt5.8 or later
#Untill that time comes we will hold back the qt3d enthusiasm and write generalized 3D code that is easy to port
#http://stackoverflow.com/questions/35074830/show-qt3d-stuff-inside-qwidget-in-qt5
contains(DEFINES, USE_QT3D){
HEADERS += \

SOURCES += \

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



ARDUMY_TOP= ../../libs/arduino
ARDUMY_HEADERS += \
	$$ARDUMY_TOP/ArduMYActuatorConfig.hpp \
	$$ARDUMY_TOP/ArduMYActuatorConfigParser.hpp \
	$$ARDUMY_TOP/ArduMYActuatorConfigSerializerBase.hpp \
	$$ARDUMY_TOP/ArduMYActuatorConfigSerializer.hpp \
	$$ARDUMY_TOP/ArduMYActuator.hpp \
	$$ARDUMY_TOP/ArduMYActuatorSet.hpp \
	$$ARDUMY_TOP/ArduMYActuatorState.hpp \
	$$ARDUMY_TOP/ArduMYActuatorValue.hpp \
	$$ARDUMY_TOP/ArduMYActuatorValueParser.hpp \
	$$ARDUMY_TOP/ArduMYActuatorValueRepresentation.hpp \
	$$ARDUMY_TOP/ArduMYActuatorValueSerializerBase.hpp \
	$$ARDUMY_TOP/ArduMYActuatorValueSerializer.hpp \
	$$ARDUMY_TOP/ArduMYCommandParser.hpp \
	$$ARDUMY_TOP/ArduMYCommandSerializer.hpp \
	$$ARDUMY_TOP/MagicDetector.hpp \



ARDUMY_SOURCES += \
	$$ARDUMY_TOP/ArduMYActuatorConfig.cpp \
	$$ARDUMY_TOP/ArduMYActuatorConfigParser.cpp \
	$$ARDUMY_TOP/ArduMYActuatorConfigSerializerBase.cpp \
	$$ARDUMY_TOP/ArduMYActuatorConfigSerializer.cpp \
	$$ARDUMY_TOP/ArduMYActuator.cpp \
	$$ARDUMY_TOP/ArduMYActuatorSet.cpp \
	$$ARDUMY_TOP/ArduMYActuatorState.cpp \
	$$ARDUMY_TOP/ArduMYActuatorValue.cpp \
	$$ARDUMY_TOP/ArduMYActuatorValueParser.cpp \
	$$ARDUMY_TOP/ArduMYActuatorValueRepresentation.cpp \
	$$ARDUMY_TOP/ArduMYActuatorValueSerializerBase.cpp \
	$$ARDUMY_TOP/ArduMYActuatorValueSerializer.cpp \
	$$ARDUMY_TOP/ArduMYCommandParser.cpp \
	$$ARDUMY_TOP/ArduMYCommandSerializer.cpp \
	$$ARDUMY_TOP/MagicDetector.cpp \

HEADERS += $$ARDUMY_HEADERS
SOURCES += $$ARDUMY_SOURCES

contains(DEFINES, USE_STATUS){
message("FROM libcorepro:")
include($$TOP_PWD/status.pri)
}
