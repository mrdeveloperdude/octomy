#ifndef HARDWAREWIZARD_HPP
#define HARDWAREWIZARD_HPP

#include "puppet/PoseMapping.hpp"
#include "hw/controllers/ActuatorControllerFactory.hpp"
#include "agent/AgentConfigStore.hpp"

#include <QWidget>
#include <QSharedPointer>

class HardwareTemplate;
class HardwareTemplateModel;
class IActuatorController;
class Agent;

namespace Ui
{
class HardwareWizard;
}



struct ControllerStanza {
	QString nickName;
	QString fullName;
	QString iconURL;

	ControllerStanza(
		QString nickName
		, QString fullName
		, QString iconURL
	)
		: nickName (nickName)
		, fullName (fullName)
		, iconURL (iconURL)
	{

	}
};

struct ControllerStanzaList: public QList<ControllerStanza> {

};


class SerialDeviceListModel;
class HardwareWizard : public QWidget
{
	Q_OBJECT

private:

	Ui::HardwareWizard *ui;
	SerialDeviceListModel *mSerialDevicesModel;
	HardwareTemplateModel *mHardwareTemplateModel;
	HardwareTemplate *mSelectedTempalte;

	/*
	ActuatorControllerFactory mControllerFactory;
	IActuatorController *mController;
	AgentConfigStore *mConfigStore;
	*/
	Agent *mAgent;
	ControllerStanzaList controllerStanzas;

public:
	explicit HardwareWizard(QWidget *parent = 0);
	virtual ~HardwareWizard();

private:

	void initControllerList();
	int controllerIndexByName(QString name);
	QString selectedControllerName();
	void moveTo(int next);
	void save();
	void loadFromTemplate();

public:

	void configure(Agent *agent);
	void reset();

signals:

	void done();

private slots:
	void on_pushButtonOnward_clicked();

	void on_pushButtonBack_clicked();


	void on_listWidgetController_doubleClicked(const QModelIndex &index);
	void on_comboBoxAddLocus_currentIndexChanged(int index);
};

#endif // HARDWAREWIZARD_HPP

