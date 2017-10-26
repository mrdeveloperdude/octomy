#include "ClientWidget.hpp"
#include "ui_ClientWidget.h"

#include "utility/Standard.hpp"

#include "basic/GenericKeyEventHandler.hpp"
#include "basic/Settings.hpp"

#include "widgets/WaitingSpinnerWidget.hpp"
#include "audio/OneOffSpeech.hpp"

#include "security/PortableID.hpp"

#include "widgets/CarSteeringWidget.hpp"

#include "comms/CommsChannel.hpp"
#include "comms/ISyncParameter.hpp"

#include "comms/couriers/AgentStateCourier.hpp"
#include "comms/couriers/SensorsCourier.hpp"
#include "comms/couriers/blob/BlobCourier.hpp"

#include "agent/AgentConstants.hpp"

#include <QScrollBar>

static const QString NODE_ONLINE_SETTINGS_KEY_BASE("octomy.online.");

ClientWidget::ClientWidget(QSharedPointer<Node> controller, QSharedPointer<Associate> nodeAssoc, QWidget *parent)
	: QWidget(parent)
	, ui(OC_NEW Ui::ClientWidget)
	, mController(controller)
	, mNodeAssoc(nodeAssoc)
	, mSpinner(nullptr)
//	, mAgentStateCourier(OC_NEW AgentStateCourier(nullptr, this))
//	, mSensorsCourier(OC_NEW SensorsCourier(this))
//	, mBlobCourier(OC_NEW BlobCourier(this))
	, mCouriers(mNodeAssoc->id(), *this)

{
	OC_METHODGATE();
	qDebug()<<"CREATING CLIENT WIDGET mController="<<(nullptr!=mController?mController->name():"NULL")<<", mNodeAssoc="<<(nullptr!=mNodeAssoc?mNodeAssoc->name():"NULL")<<", parent="<<parent;

	ui->setupUi(this);

	ui->widgetBirthCertificate->configure(false,true);
	if(nullptr!=mNodeAssoc) {
		ui->widgetBirthCertificate->setPortableID(mNodeAssoc->toPortableID());
	} else {
		qWarning()<<"ERROR: no client";
	}
	updateTimer.setInterval(100);
	if(!connect(&updateTimer,SIGNAL(timeout()),this,SLOT(onUpdateTimer()),OC_CONTYPE)) {
		qWarning()<<"ERROR: Could not connect";
	}
	updateTimer.start();
	ui->widgetFace->setDisabled(true);
	ui->tryToggleListen->configure("Connect","Connecting...","Connected", "Disconnecting...", AgentConstants::AGENT_CONNECT_BUTTON_COLOR, AgentConstants::AGENT_CONNECT_TEXT_COLOR);
	ui->tryToggleListen->setState(OFF,false);

	if(!connect(ui->tryToggleListen, &TryToggle::stateChanged, this, &ClientWidget::onConnectButtonStateChanged ,OC_CONTYPE)) {
		qWarning()<<"ERROR: Could not connect";
	}

//	const QString fullID=mNodeAssoc->id();
//	qDebug()<<"REMOTE CLIENT WIDGET DESTINATION SET TO "<<fullID;
//	mAgentStateCourier->setDestination(fullID);
//	mSensorsCourier->setDestination(fullID);
//	mBlobCourier->setDestination(fullID);


//	if(nullptr!=mAgentStateCourier) {
//		mAgentStateCourier->setHookSignals(*this,true);
//	} else {
//		qWarning()<<"ERROR: Could not hook agent state courier events, no courier";
//	}

	if(nullptr!=mController) {
		mController->setHookCommsSignals(*this, true);
	} else {
		qWarning()<<"ERROR: Could not hook controller events, no controller";
	}

	//installEventFilter(this);
	init();

	updateOnlineStatus();
}

ClientWidget::~ClientWidget()
{
	OC_METHODGATE();
	delete ui;
	ui=nullptr;
}

void ClientWidget::prepareSpinner()
{
	OC_METHODGATE();
	mSpinner=OC_NEW WaitingSpinnerWidget(ui->stackedWidgetControl, true, true);
	SpinnerStyle style;
	style.setColor(QColor("white"));
	style.setRelatveSize(true);
	style.setNumberOfLines(24);
	style.setLineLength(10);
	style.setInnerRadius(40);
	style.setLineWidth(3);
	mSpinner->setText("Reconnecting..");
	mSpinner->setStyle(style);
}

void ClientWidget::prepareSteering()
{
	OC_METHODGATE();
	if(!connect(ui->widgetCarSteering, &CarSteeringWidget::steeringChanged, this, &ClientWidget::onSteeringChanged ,OC_CONTYPE)) {
		qWarning()<<"ERROR: Could not connect";
	}
}


void ClientWidget::setSpinnerActive(bool active)
{
	OC_METHODGATE();
	if(nullptr!=mSpinner) {
		mSpinner->setStarted( active);
	}
}


// NOTE: This has a sister method in AgentWindow.cpp around line 625
//       Please remember that while they are similar they are very different!
//       While the other one is a "One agent, one remote" deal
//       this is a "one remote, many agents" deal.
void ClientWidget::updateOnlineStatus()
{
	OC_METHODGATE();
	//qDebug()<<"START UPDATE ONLINE STATUS # # # # ";
	if(nullptr!=mController && nullptr!=mNodeAssoc) {
		// Find if we ARE trying to get online
		bool isTryingToGoOnline=false;
		isTryingToGoOnline=mController->isCommsStarted() && courierRegistration();
		// Find if we WANT to be online
		bool wantToBeOnline=false;
		Settings *s=&mController->settings();
		if(nullptr!=s) {
			wantToBeOnline=s->getCustomSettingBool("octomy.online."+mNodeAssoc->name(), false);
		} else {
			qWarning()<<"ERROR: No settings for client";
		}
		//Spell it out for debugging
		qDebug()<<"Client '"<<mNodeAssoc->name()<<"' is currently trying to be "<<(isTryingToGoOnline?"ONLINE":"OFFLINE")<<" and wants to try for "<<(wantToBeOnline?"ONLINE":"OFFLINE")<<".";
		// Make necessary changes to state
		const TryToggleState currentTryState=ui->tryToggleListen->state();
		TryToggleState nextTryState=currentTryState;
		bool nextOnlineStatus=isTryingToGoOnline;
		if(wantToBeOnline ) {
			if(isTryingToGoOnline ) {
				nextTryState=ON;
			} else {
				nextTryState=GOING_ON;
				//qDebug()<<"Decided to go online";
				nextOnlineStatus=true;
			}
		} else {
			if(isTryingToGoOnline ) {
				//qDebug()<<"Decided to go offline";
				nextOnlineStatus=false;
			} else {
				nextTryState=OFF;
			}
		}
		if(nextOnlineStatus!=isTryingToGoOnline) {
			mController->comms()->setHoneymoonEnd(QDateTime::currentMSecsSinceEpoch()+(1000*60*5));//Set 5 minute honeymoon at every state change
			qDebug()<<"Decided to change online for "<<mNodeAssoc->name();
			setCourierRegistration(nextOnlineStatus);

		} else {
			//qDebug()<<"No change in online status ("<<nextOnlineStatus<<")";
		}
		if(nextTryState!=currentTryState) {
			//qDebug()<<"Decided to change tristate button from "<<currentTryState<<" -> "<<nextTryState;
			ui->tryToggleListen->setState(nextTryState, false);
		} else {
			//qDebug()<<"No change tristate button ("<<nextTryState<<")";
		}
	}
	//qDebug()<<"END UPDATE ONLINE STATUS # # # # ";
}


bool ClientWidget::courierRegistration()
{
	OC_METHODGATE();
	return mCouriers.commsEnabled();
}

void ClientWidget::setCourierRegistration(bool reg)
{
	OC_METHODGATE();
	mCouriers.setCommsEnabled(reg);
}



bool ClientWidget::needConnection()
{
	OC_METHODGATE();
	if(nullptr!=mController && nullptr!=mNodeAssoc) {
		Settings *s=&mController->settings();
		if(nullptr!=s) {
			return s->getCustomSettingBool(NODE_ONLINE_SETTINGS_KEY_BASE+mNodeAssoc->name(), false);
		}
	}
	return false;
}



void ClientWidget::init()
{
	OC_METHODGATE();
//	ui->stackedWidgetControl->setUpdatesEnabled(false);
	prepareSpinner();
	prepareSteering();
	if(nullptr!=mController) {
//		Settings &s=mController->settings();
		ui->labelLocal->setText("WAITING FOR LOCAL");
		ui->labelHub->setText("WAITING FOR HUB");
		ui->labelGPS->setText("WAITING FOR GPS");
		ui->labelCompass->setText("WAITING FOR COMPASS");
		ui->labelGyroscope->setText("WAITING FOR GYRO");
		ui->labelAccelerometer->setText("WAITING FOR ACCELEROMETER");
		ui->labelTouch->setText("WAITING FOR TOUCH");
		//ui->widgetActuatorControl->configure(10);
	} else {
		ui->labelLocal->setText("N/A");
		ui->labelHub->setText("N/A");
		ui->labelGPS->setText("N/A");
		ui->labelCompass->setText("N/A");
		ui->labelGyroscope->setText("N/A");
		ui->labelAccelerometer->setText("N/A");
		qWarning()<<"ERROR: No context provider set!";
	}
//	ui->stackedWidgetControl->setUpdatesEnabled(true);
}


CommsChannel *ClientWidget::comms()
{
	OC_METHODGATE();
	if(nullptr!=mController) {
		return mController->comms();
	}
	return nullptr;
}

QSharedPointer<Associate> ClientWidget::nodeAssoc() const
{
	OC_METHODGATE();
	return mNodeAssoc;
}

void ClientWidget::updateControlLevel(int level)
{
	//qDebug()<<"CONTROL LEVEL IS "<<level;
	ui->stackedWidgetControl->setCurrentIndex(level);
	//Enable spinner when any level is selected except connect and we are not conneted
	//setSpinnerActive(level!=0 && ON != ui->tryToggleListen->state());
}


bool ClientWidget::eventFilter(QObject *object, QEvent *event)
{
	OC_METHODGATE();
	/*

	//qDebug()<<"EVENT: "<<object<<": "<<event;
	if ((u==object) && (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress )) {
		QMouseEvent *r= static_cast<QMouseEvent *>(event);
		ui->labelTouch->setText("TOUCH: <"+QString::number(r->globalX())+", "+QString::number(r->globalY())+">");
		QPointF p=r->localPos();
		QSize s=size();
		//mRemote->onTouchUpdated(QVector2D(p.x()/(qreal)s.width(), p.y()/(qreal)s.height()));
	}
	//object == ui->widgetBackground && (
	*/
	return false;
}


//////////////////////////////////////////////////
// Agent State Courier slots

void ClientWidget::onSyncParameterChanged(ISyncParameter *sp)
{
	OC_METHODGATE();
	qDebug()<<"ClientWidget ASC: ON VALUE CHANGED: "<<sp->toString();
	AgentStateCourier *asc=mCouriers.agentStateCourier();
	if(nullptr!=asc) {
		const bool panic=asc->panic();
		ui->widgetPanic->setPanic(panic);
	}
}

//////////////////////////////////////////////////
// CommsChannel slots


void ClientWidget::onCommsError(QString e)
{
	OC_METHODGATE();
	qDebug()<<"ClientWidget UNIMP Comms error: "<<e;
}

void ClientWidget::onCommsClientAdded(CommsSession *c)
{
	OC_METHODGATE();
	//qDebug()<<"ClientWidget UNIMP Client added: "<<c->toString();
}

void ClientWidget::onCommsConnectionStatusChanged(bool s)
{
	OC_METHODGATE();
	//qDebug() <<"CLIENT WIDGET New connection status: "<<(s?"ONLINE":"OFFLINE");
	updateOnlineStatus();
}


///////////////////////////////////////// // Internal slots


void ClientWidget::onUpdateTimer()
{
	OC_METHODGATE();
	//qDebug()<<"TIME for summary";
}



void ClientWidget::appendLog(const QString& text)
{
	OC_METHODGATE();
	ui->logScroll->appendLog(text);
}




///////////////////////////////////////// // Internal UI slots


void ClientWidget::onConnectButtonStateChanged(const TryToggleState last, const TryToggleState current)
{
	OC_METHODGATE();
	//qDebug()<< "CONNECT BUTTON TRYSTATE CHANGED FROM " << ToggleStateToSTring(last)<< " TO " << ToggleStateToSTring(current);
	if(current!=last) {
		if(nullptr!= mNodeAssoc) {
			Settings &s=mController->settings();
			const bool on=(OFF!=current);
			s.setCustomSettingBool("octomy.online."+mNodeAssoc->name(), on);
		} else {
			qWarning()<<"ERROR: No node assoc";
		}
	}
	updateOnlineStatus();
}

void ClientWidget::onSteeringChanged(qreal throttle, qreal steeringAngle)
{
	OC_METHODGATE();
	//qDebug()<<"THROT: "<<throttle<<" STEER: "<<steeringAngle;
	AgentStateCourier *asc=mCouriers.agentStateCourier();
	if(nullptr!=asc) {
		asc->setPoseValue(0,throttle);
		asc->setPoseValue(1,steeringAngle);
	} else {
		qWarning()<<"ERROR: no ASC when changing THROT: "<<throttle<<" STEER: "<<steeringAngle;
	}
}



void ClientWidget::on_checkBoxShowEyes_toggled(bool checked)
{
	OC_METHODGATE();
	// TODO: Find better storage for these values than settings
	if(nullptr!=mController) {
		Settings&s=mController->settings();
		s.setCustomSettingBool("octomy.face",checked);
		s.setCustomSettingBool("octomy.online.show",checked);
		ui->widgetFace->updateVisibility();
	} else {
		qWarning()<<"ERROR: No controller";
	}
}

void ClientWidget::on_checkBoxShowStats_toggled(bool checked)
{
	OC_METHODGATE();
	// TODO: Find better storage for these values than settings
	if(nullptr!=mController) {
		Settings&s=mController->settings();
		s.setCustomSettingBool("octomy.debug.stats",checked);
		ui->widgetFace->updateVisibility();
	} else {
		qWarning()<<"ERROR: No controller";
	}
}

void ClientWidget::on_checkBoxShowLog_toggled(bool checked)
{
	OC_METHODGATE();
	// TODO: Find better storage for these values than settings
	if(nullptr!=mController) {
		Settings&s=mController->settings();
		s.setCustomSettingBool("octomy.debug.log",checked);
		ui->widgetFace->updateVisibility();
	} else {
		qWarning()<<"ERROR: No controller";
	}
}

void ClientWidget::on_checkBoxShowOnlineButton_toggled(bool checked)
{
	OC_METHODGATE();
	// TODO: Find better storage for these values than settings
	if(nullptr!=mController) {
		Settings&s=mController->settings();
		s.setCustomSettingBool("octomy.online.show",checked);
		ui->widgetFace->updateVisibility();
	} else {
		qWarning()<<"ERROR: No controller";
	}
}



void ClientWidget::on_widgetPanic_toggled(bool panic)
{
	OC_METHODGATE();
	if(panic) {
		QString str="P A N I C !";
		qWarning()<<str;
		appendLog(str);
	} else {
		QString str="Panic averted";
		qWarning()<<str;
		appendLog(str);
	}
	AgentStateCourier *asc=mCouriers.agentStateCourier();
	if(nullptr!=asc) {
		asc->setPanic(panic);
	} else {
		qWarning()<<"ERROR: no ASC when changing panic to "<<panic;
	}

}
