#ifndef ARDUMYCONTROLLERWIDGET_HPP
#define ARDUMYCONTROLLERWIDGET_HPP

#include "widgets/TryToggleState.hpp"

#include <QWidget>

class ArduMYController;


namespace Ui
{
class ArduMYControllerWidget;
}

class ArduMYControllerWidget : public QWidget
{
	Q_OBJECT
private:
	Ui::ArduMYControllerWidget *ui;
	ArduMYController *mController;

public:
	explicit ArduMYControllerWidget(QWidget *parent = 0);
	~ArduMYControllerWidget();

public:
	void configure(ArduMYController *controller);

private:

	void setUILock(bool lock);

	void reconnectActuatorWidgets();

public slots:

	void onSerialSettingsChanged();
	void onConnectionChanged();
	void onTryConnectChanged(const TryToggleState last, const TryToggleState current);

private slots:
	void on_pushButtonConfigureSerial_clicked();
	void on_pushButtonConfigureActuators_clicked();

	void on_comboBoxAddActuator_currentIndexChanged(int index);
	void on_pushButtonSync_clicked();
};

#endif // ARDUMYCONTROLLERWIDGET_HPP
