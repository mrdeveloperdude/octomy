TARGET =	core
TEMPLATE =	lib
CONFIG +=	staticlib
QT +=		serialport

include($$PRIS/common.pri)
include($$PRIS/lib.pri)


includes = util rng comms puppet

for(i, includes) {
	INCLUDEE=$$i
	include($$PRIS/libincluder.pri)
}

INCLUDEPATH += ./


SOURCES +=\
	basic/AgentNameGenerator.cpp \
	basic/AsyncStore.cpp \
	basic/AtomicBoolean.cpp \
	basic/Fingerprint.cpp \
	basic/GenerateRunnable.cpp \
	basic/GenericKeyEventHandler.cpp \
	basic/Iconv.cpp \
	basic/LogDestination.cpp \
	basic/LogHandler.cpp \
	basic/NodeAssociate.cpp \
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
	conscious/Consciousness.cpp \
	discovery/DiscoveryClient.cpp \
	discovery/DiscoveryRole.cpp \
	discovery/DiscoveryServer.cpp \
	discovery/DiscoveryServerSession.cpp \
	discovery/NodeAssociateStore.cpp \
	hw/bluetooth/BluetoothList.cpp \
	hw/controllers/ActuatorControllerFactory.cpp \
	hw/controllers/ardumy/ArduMYActuatorWidget.cpp \
	hw/controllers/ardumy/ArduMYController.cpp \
	hw/controllers/ardumy/ArduMYControllerWidget.cpp \
	hw/controllers/ardumy/ArduMYTypeConversions.cpp \
	hw/controllers/IActuatorController.cpp \
	hw/controllers/servotor32/HexyLeg.cpp \
	hw/controllers/servotor32/HexySerial.cpp \
	hw/controllers/servotor32/Servotor32ActuatorWidget.cpp \
	hw/controllers/servotor32/Servotor32Controller.cpp \
	hw/controllers/servotor32/Servotor32ControllerWidget.cpp \
	hw/serial/SerialList.cpp \
	hw/serial/SerialSettings.cpp \
	hw/serial/SerialSettingsWidget.cpp \
	locus/HoveringLocus.cpp \
	locus/HoveringLocusWidget.cpp \
	locus/LeggedLocus.cpp \
	locus/LeggedLocusWidget.cpp \
	locus/LocusController.cpp \
	locus/Locus.cpp \
	locus/LocusFactory.cpp \
	locus/LocusSet.cpp \
	locus/LocusWidget.cpp \
	locus/TrackedLocus.cpp \
	locus/TrackedLocusWidget.cpp \
	locus/WheeledLocus.cpp \
	locus/WheeledLocusWidget.cpp \
	models/ClientModel.cpp \
	models/HardwareTemplateModel.cpp \
	models/PairingListModel.cpp \
	models/SerialDeviceListModel.cpp \
	models/TestListModel.cpp \
	models/TriggerListModel.cpp \
	sensory/SensorInput.cpp \
	storage/Hashstore.cpp \
	storage/HashstoreRecord.cpp \
	storage/HashstoreRecordState.cpp \
	trigger/Action.cpp \
	trigger/Condition.cpp \
	trigger/Trigger.cpp \
	trigger/TriggerSet.cpp \
	widgets/ActuatorManagerWidget.cpp \
	widgets/ActuatorSelector.cpp \
	widgets/AgentDeliveryWizard.cpp \
	widgets/ArduinoPinCombobox.cpp \
	widgets/ArduinoPin.cpp \
	widgets/ArduinoPinFacilitator.cpp \
	widgets/ArduinoPinFilter.cpp \
	widgets/ArduinoPinMap.cpp \
	widgets/ArduinoPinSelector.cpp \
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
	widgets/PortableIDWidget.cpp \
	widgets/QRWidget.cpp \
	widgets/RealtimeValuesWidget.cpp \
	widgets/SpeechControlWidget.cpp \
	widgets/SvgWidget.cpp \
	widgets/TestWidget.cpp \
	widgets/TextEntry.cpp \
	widgets/TriggerManagerWidget.cpp \
	widgets/TriggerWidget.cpp \
	widgets/TryToggle.cpp \
	widgets/TryToggleState.cpp \
	widgets/WaitingSpinnerWidget.cpp \
	zoo/PunchRegistry.cpp \
	zoo/WebRequest.cpp \
	zoo/ZooClient.cpp \
	zoo/ZooConstants.cpp \
	zoo/ZooServer.cpp \





HEADERS  += \
	basic/AgentNameGenerator.hpp \
	basic/AsyncStore.hpp \
	basic/AtomicBoolean.hpp \
	basic/Fingerprint.hpp \
	basic/GenerateRunnable.hpp \
	basic/GenericKeyEventHandler.hpp \
	basic/Iconv.hpp \
	basic/LogDestination.hpp \
	basic/LogHandler.hpp \
	basic/NodeAssociate.hpp \
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
	conscious/Consciousness.hpp \
	discovery/DiscoveryClient.hpp \
	discovery/DiscoveryRole.hpp \
	discovery/DiscoveryServer.hpp \
	discovery/DiscoveryServerSession.hpp \
	discovery/NodeAssociateStore.hpp \
	hw/bluetooth/BluetoothList.hpp \
	hw/controllers/ActuatorControllerFactory.hpp \
	hw/controllers/ardumy/ArduMYActuatorWidget.hpp \
	hw/controllers/ardumy/ArduMYController.hpp \
	hw/controllers/ardumy/ArduMYControllerWidget.hpp \
	hw/controllers/ardumy/ArduMYTypeConversions.hpp \
	hw/controllers/IActuatorController.hpp \
	hw/controllers/servotor32/HexyLeg.hpp \
	hw/controllers/servotor32/HexySerial.hpp \
	hw/controllers/servotor32/Servotor32ActuatorWidget.hpp \
	hw/controllers/servotor32/Servotor32Controller.hpp \
	hw/controllers/servotor32/Servotor32ControllerWidget.hpp \
	hw/serial/SerialList.hpp \
	hw/serial/SerialSettings.hpp \
	hw/serial/SerialSettingsWidget.hpp \
	locus/HoveringLocus.hpp \
	locus/HoveringLocusWidget.hpp \
	locus/LeggedLocus.hpp \
	locus/LeggedLocusWidget.hpp \
	locus/LocusController.hpp \
	locus/LocusFactory.hpp \
	locus/Locus.hpp \
	locus/LocusSet.hpp \
	locus/LocusWidget.hpp \
	locus/TrackedLocus.hpp \
	locus/TrackedLocusWidget.hpp \
	locus/WheeledLocus.hpp \
	locus/WheeledLocusWidget.hpp \
	models/ClientModel.hpp \
	models/HardwareTemplateModel.hpp \
	models/PairingListModel.hpp \
	models/SerialDeviceListModel.hpp \
	models/TestListModel.hpp \
	models/TriggerListModel.hpp \
	sensory/SensorInput.hpp \
	storage/Hashstore.hpp \
	storage/HashstoreRecord.hpp \
	storage/HashstoreRecordState.hpp \
	trigger/Action.hpp \
	trigger/Condition.hpp \
	trigger/Trigger.hpp \
	trigger/TriggerSet.hpp \
	widgets/ActuatorManagerWidget.hpp \
	widgets/ActuatorSelector.hpp \
	widgets/AgentDeliveryWizard.hpp \
	widgets/ArduinoPinCombobox.hpp \
	widgets/ArduinoPinFacilitator.hpp \
	widgets/ArduinoPinFilter.hpp \
	widgets/ArduinoPin.hpp \
	widgets/ArduinoPinMap.hpp \
	widgets/ArduinoPinSelector.hpp \
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
	widgets/PortableIDWidget.hpp \
	widgets/QRWidget.hpp \
	widgets/RealtimeValuesWidget.hpp \
	widgets/SpeechControlWidget.hpp \
	widgets/SvgWidget.hpp \
	widgets/TestWidget.hpp \
	widgets/TextEntry.hpp \
	widgets/TriggerManagerWidget.hpp \
	widgets/TriggerWidget.hpp \
	widgets/TryToggle.hpp \
	widgets/TryToggleState.hpp \
	widgets/WaitingSpinnerWidget.hpp \
	zoo/PunchRegistry.hpp \
	zoo/WebRequest.hpp \
	zoo/ZooClient.hpp \
	zoo/ZooConstants.hpp \
	zoo/ZooServer.hpp \


FORMS    += \
	locus/HoveringLocusWidget.ui \
	locus/LeggedLocusWidget.ui \
	locus/TrackedLocusWidget.ui \
	ui/ActuatorManagerWidget.ui \
	ui/AgentDeliveryWizard.ui \
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
	ui/PortableIDWidget.ui \
	ui/RealtimeValuesWidget.ui \
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


contains(DEFINES, USE_STATUS){
message("FROM libcore.pro:")
include($$PRIS/status.pri)
}

