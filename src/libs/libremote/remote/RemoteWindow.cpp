#include "RemoteWindow.hpp"
#include "ui_RemoteWindow.h"
#include "Remote.hpp"
#include "comms/CommsChannel.hpp"
#include "utility/Utility.hpp"
#include "comms/CommsSession.hpp"
#include "widgets/ControlDeliveryWizard.hpp"



#include "map/MapControl.hpp"
#include "map/OSMMapAdapter.hpp"
#include "map/OpenAerialMapAdapter.hpp"
#include "map/MapLayer.hpp"
#include "map/LineString.hpp"

#include "discovery/DiscoveryClient.hpp"

#include "security/PortableID.hpp"

#include "node/ClientWidget.hpp"

#include "identity/Identicon.hpp"


#include <QDebug>
#include <QAccelerometerReading>
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QScrollBar>
#include <QComboBox>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#endif

RemoteWindow::RemoteWindow(Remote *remote, QWidget *parent)
	: QWidget(parent)
	, ui(OC_NEW Ui::RemoteWindow)
	, mRemote(remote)
{
	OC_METHODGATE();
	ui->setupUi(this);
	//ui->stackedWidgetScreen->setUpdatesEnabled(false);
	prepareDelivery();
	prepareDiscovery();
	preparePairing();
	prepareAgentList();
	prepareControlLevelList();
	prepareMenu();

	hookSensorSignals();
	goToStartPage();

#ifdef Q_OS_ANDROID
	showFullScreen();
#endif
	//ui->stackedWidgetScreen->setUpdatesEnabled(true);
}

RemoteWindow::~RemoteWindow()
{
	OC_METHODGATE();
	delete ui;
	ui=nullptr;
}



void RemoteWindow::prepareDelivery()
{
	OC_METHODGATE();
	if(nullptr!=mRemote) {
		ui->widgetDelivery->configure(mRemote);
		// Make sure to go to correct page after delivery is complete
		if(!connect(ui->widgetDelivery, SIGNAL(done(bool)), this, SLOT(onDeliveryDone(bool)), OC_CONTYPE)) {
			qWarning()<<"ERROR: Could not connect ";
		}
	} else {
		qWarning()<<"ERROR: no remote";
	}

}


void RemoteWindow::prepareDiscovery()
{
	OC_METHODGATE();
	if(nullptr!=mRemote) {
		mRemote->updateDiscoveryClient();
		mRemote->hookPeerSignals(*this);
	} else {
		qWarning()<<"ERROR: no remote";
	}
}

void RemoteWindow::preparePairing()
{
	OC_METHODGATE();
	if(nullptr!=mRemote) {
		ui->widgetPairing->configure(mRemote);
		//Make sure to go to correct page when pairing is complete
		connect(ui->widgetPairing, &PairingWizard::done, [=]() {
			ui->stackedWidgetScreen->setCurrentWidget(ui->pageRunning);
		});
	} else {
		qWarning()<<"ERROR: no remote";
	}

}

void RemoteWindow::prepareAgentList()
{
	OC_METHODGATE();
	// Add agents, getting the number of agents added
	updateAgentsList();
}


void RemoteWindow::prepareControlLevelList()
{
	OC_METHODGATE();
	if(nullptr!=mRemote) {
		Settings &s=mRemote->settings();
		ui->comboBoxControlLevel->setCurrentText(s.getCustomSetting("octomy.remote.control.level",ui->comboBoxControlLevel->currentText()));
		updateControlLevel();
	} else {
		qWarning()<<"ERROR: no remote";
	}

}


void RemoteWindow::hookSensorSignals()
{
	OC_METHODGATE();
	if(nullptr!=mRemote) {
		mRemote->hookSensorSignals(*this);
	} else {
		qWarning()<<"ERROR: no remote";
	}
}


void RemoteWindow::goToStartPage()
{
	OC_METHODGATE();
	//qDebug()<<"";
	//qDebug()<<"----------------- - - - - - ------------------------- - - - - ";
	if(nullptr!=mRemote) {
		//Select correct starting page
		auto key=mRemote->keyStore().localKey();
		if(nullptr!=key) {
			if(!key->isValid(true)) {
				//qDebug()<<"STARTING WITH DELIVERY";
				ui->widgetDelivery->reset();
				ui->stackedWidgetScreen->setCurrentWidget(ui->pageDelivery);
			} else if(mRemote->peers().associateCount()<=0) {
				//qDebug()<<"STARTING WITH PAIRING";
				ui->widgetPairing->reset();
				ui->stackedWidgetScreen->setCurrentWidget(ui->pagePairing);
			} else {
				//qDebug()<<"STARTING WITH RUN";
				ui->stackedWidgetScreen->setCurrentWidget(ui->pageRunning);
			}
		} else {
			qWarning()<<"ERROR: no key";
		}
	} else {
		qWarning()<<"ERROR: no remote";
	}
}



int RemoteWindow::updateAgentsList()
{
	OC_METHODGATE();
	int ct=0;
	ui->comboBoxAgent->clear();
	if(nullptr!=mRemote) {
		QSharedPointer<Associate> local=mRemote->nodeIdentity();
		if(nullptr!=local) {
			const QString myID=local->id();
			AddressBook &peerStore=mRemote->peers();
			QMap<QString, QSharedPointer<Associate> > &peers=peerStore.all();
			for(QMap<QString, QSharedPointer<Associate> >::iterator i=peers.begin(), e=peers.end(); i!=e; ++i) {
				const QString id=i.key();
				QSharedPointer<Associate> peer=i.value();
				if(DiscoveryType::TYPE_REMOTE!=peer->type()) {
					addAgentToList(peer);
					ct++;
				}
			}
		}
	} else {
		qWarning()<<"ERROR: no remote";
	}
	return ct;
}



void RemoteWindow::addAgentToList(QSharedPointer<Associate> peer)
{
	OC_METHODGATE();
	if(nullptr!=peer) {
		QString name=peer->name().trimmed();
		if(""==name) {
			name=peer->id().mid(0,16);
		}
		QVariant userData=peer->id();

		QIcon icon;
		//QString iconPath=":/icons/agent.svg";		icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
		PortableID pid=peer->toPortableID();
		Identicon identicon(pid);
		icon=QIcon(identicon.pixmap());


		//qDebug()<<"INSERTING AGENT IN DROPDOWN "<<peer->toPortableID().toPortableString();
		ui->comboBoxAgent->insertItem(0, icon, name, userData);
		ui->comboBoxAgent->setCurrentIndex(0);
	}
}


Settings &RemoteWindow::settings()
{
	OC_METHODGATE();
	return mRemote->settings();
}


bool RemoteWindow::needConnection()
{
	OC_METHODGATE();
	bool ret=false;
	for(QMap<int, ClientWidget *>::iterator it=mClientWidgets.begin(), end=mClientWidgets.end();it!=end;++it){
		ClientWidget * cw=it.value();
		if(nullptr!=cw){
			ret|=cw->needConnection();
		}
	}
	return ret;
}




void RemoteWindow::notifyAndroid(QString s)
{
	OC_METHODGATE();
	(void)s;
	//TODO: This crashes with some jni exception stuff. Figure out why
#ifdef Q_OS_ANDROID
	qDebug()<<"QT: Android NOTIF: "<<s;
	QAndroidJniObject::callStaticMethod<void>("org/octomy/remote/Remote",
			"notify",
			"(Ljava/lang/String;)V",
			QAndroidJniObject::fromString(s).object<jstring>());
#endif
}


void RemoteWindow::toastAndroid(QString s)
{
	OC_METHODGATE();
	(void)s;
	//TODO: This crashes with some jni exception stuff. Figure out why
#ifdef Q_OS_ANDROID
	qDebug()<<"QT: Android TOAST: "<<s;
	QAndroidJniObject::callStaticMethod<void>("org/octomy/remote/Remote",
			"toast",
			"(Ljava/lang/String;)V",
			QAndroidJniObject::fromString(s).object<jstring>());
#endif
}



void RemoteWindow::keyReleaseEvent(QKeyEvent *e)
{
	OC_METHODGATE();
	if(Qt::Key_Back==e->key()) {
		/*
		if(ui->pageConnect==ui->stackedWidgetScreen->currentWidget()){
			appendLog("EXITING APP ON BACK BUTTON");
			ui->stackedWidgetScreen->setCurrentWidget(ui->pageConfirmQuit);
		}
		else if(ui->pageStatus==ui->stackedWidgetScreen->currentWidget()){
			appendLog("GOING TO CONNECTION SCREEN ON BACK BUTTON");
			ui->stackedWidgetScreen->setCurrentWidget(ui->pageConnect);
		}
		else
		*/


		if(ui->pageRunning==ui->stackedWidgetScreen->currentWidget()) {
			appendLog("GOING TO CONFIRM QUIT SCREEN ON BACK BUTTON");
			ui->stackedWidgetScreen->setCurrentWidget(ui->pageConfirmQuit);
		} else if(ui->pageConfirmQuit==ui->stackedWidgetScreen->currentWidget()) {
			appendLog("GOING TO RUNNING SCREEN ON BACK BUTTON");
			ui->stackedWidgetScreen->setCurrentWidget(ui->pageRunning);
		} else {
			appendLog("ERROR ON BACK BUTTON");
		}
		e->accept();
	} else {
		//appendLog("UNKNOWN BUTTON: "+QString::number(e->key()));
	}
}



void RemoteWindow::updateControlLevel()
{
	OC_METHODGATE();
	//qDebug()<<"SWITCHING CONTROL LEVEL TO "<<ui->comboBoxControlLevel->currentText();
	const int idx=ui->comboBoxControlLevel->currentIndex();
	ClientWidget *clientWidget = qobject_cast<ClientWidget *>(ui->stackedWidgetControl->currentWidget());
	if(nullptr!=clientWidget) {
		clientWidget->updateControlLevel(idx);
	}
}


void RemoteWindow::updateActiveAgent()
{
	OC_METHODGATE();
	if(nullptr!=mRemote) {
		const int idx=ui->comboBoxAgent->currentIndex();
		const QString agentName=ui->comboBoxAgent->currentText();
		const QString agentID=ui->comboBoxAgent->currentData().toString();
		QSharedPointer<Associate>  client=mRemote->peers().associateByID(agentID);
		//qDebug()<<"SWITCHING ACTIVE AGENT TO "<<agentName<<"("<<idx<<", "<<agentID<<")";
		if(idx>=0) {
			QMap<int, ClientWidget *>::const_iterator it=mClientWidgets.find(idx);
			ClientWidget *cw=nullptr;
			if(mClientWidgets.end()==it) {
				//qDebug()<<"CREATING CLIENT WIDGET WITH ID "<<client->toPortableID().toPortableString();
				cw=OC_NEW ClientWidget(QSharedPointer<Node>(mRemote), client, this);
				if(nullptr!=cw) {
					//qDebug()<<"Created client widget for index "<<idx;
					mClientWidgets[idx]=cw;
					ui->stackedWidgetControl->addWidget(cw);
				} else {
					qWarning()<<"ERROR: Could not allocate client widget";
				}
			} else {
				cw=it.value();
			}
			if(nullptr!=cw) {
				const ClientWidget *old=qobject_cast<ClientWidget *>(ui->stackedWidgetControl->currentWidget());
				const int oldIndex=ui->stackedWidgetControl->currentIndex();
				const QString oldName=(nullptr==old)?"NULL":old->nodeAssoc()->name();
				ui->stackedWidgetControl->setCurrentWidget(cw);
				const int newIndex=ui->stackedWidgetControl->currentIndex();
				const QString newName=(nullptr==cw)?"NULL":cw->nodeAssoc()->name();
				qDebug()<<"SWITCHING FROM "<<old << "("<<oldIndex << ", '"<<oldName << "') TO "<<cw <<"("<<newIndex << ", '"<<newName << "')";
				updateControlLevel();
			}
		}
	}
	//Remove the placeholder
	ui->stackedWidgetControl->removeWidget(ui->widgetClientPlaceholder);
	int ct=ui->stackedWidgetControl->count();
	//qDebug()<<"CLIENT WIDGETS: "<<ct;
	//Add the placeholder back if count <=0
	if(ct<=0) {
		//qDebug()<<"ADDING PLACEHOLDER";
		ui->stackedWidgetControl->addWidget(ui->widgetClientPlaceholder);
	}
}

void RemoteWindow::prepareMenu()
{
	OC_METHODGATE();
	QAction *pairingAction = OC_NEW QAction(tr("Pair"), this);
	pairingAction->setStatusTip(tr("Do the pairing dance"));
	pairingAction->setIcon(QIcon(":/icons/pair.svg"));
	connect(pairingAction, &QAction::triggered, this, &RemoteWindow::onStartPairing);
	mMenu.addAction(pairingAction);

	QAction *certAction = OC_NEW QAction(tr("Show My ID"), this);
	certAction->setStatusTip(tr("Show the identification of this remote"));
	certAction->setIcon(QIcon(":/icons/certificate.svg"));
	connect(certAction, &QAction::triggered, this, &RemoteWindow::onStartShowBirthCertificate);
	mMenu.addAction(certAction);
}



void RemoteWindow::appendLog(const QString& text)
{
	OC_METHODGATE();
	ui->logScroll->appendPlainText(text);
	QScrollBar *vsb=ui->logScroll->verticalScrollBar();
	if(nullptr!=vsb) {
		vsb->setValue(vsb->maximum());
	}
}



//////////////////////////////////////////////////
// Internal slots



void RemoteWindow::onStartShowBirthCertificate()
{
	OC_METHODGATE();
	PortableID id;
	Settings *s=(nullptr!=mRemote)?(&mRemote->settings()):nullptr;
	if(nullptr!=s) {
		id.fromPortableString(s->getCustomSetting("octomy.portable.id",""));
	}
	auto key=mRemote->keyStore().localKey();
	if(nullptr!=key) {
		id.setID(key->id());
	} else {
		qWarning()<<"ERROR: No key";
	}
	id.setType(TYPE_REMOTE);
	ui->widgetBirthCertificate->setPortableID(id);
	ui->stackedWidgetScreen->setCurrentWidget(ui->pageMyID);
}


void RemoteWindow::onStartPairing()
{
	OC_METHODGATE();
	ui->widgetPairing->reset();
	ui->stackedWidgetScreen->setCurrentWidget(ui->pagePairing);
}


void RemoteWindow::onStartPlanEditor()
{
	OC_METHODGATE();
	ui->stackedWidgetScreen->setCurrentWidget(ui->pagePlan);
}


//////////////////////////////////////////////////
// Delivery Wizard slots


void RemoteWindow::onDeliveryDone(bool pairNow)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW DELIVERY DONE ";
	prepareDiscovery();
	preparePairing();
	if(pairNow) {
		ui->widgetPairing->reset();
	}
	ui->stackedWidgetScreen->setCurrentWidget(pairNow?ui->pagePairing:ui->pageRunning);
}

//////////////////////////////////////////////////
// Keystore slots


void RemoteWindow::onKeystoreReady(bool ok)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW keystore load compelte: "<<(ok?"OK":"ERROR");
	prepareDiscovery();
	preparePairing();
}

//////////////////////////////////////////////////
// Peer Store slots


void RemoteWindow::onAssociateAdded(QString id)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW peer added: "<<id;
	// Handled by change handler
}

void RemoteWindow::onAssociateRemoved(QString id)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW peer removed: "<<id;
	// Handled by change handler
}


void RemoteWindow::onAssociateChanged()
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW peers changed: ";
	updateAgentsList();
}



void RemoteWindow::onAddressBookReady(bool ok)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW peer store load compelte: "<<(ok?"OK":"ERROR");
	prepareDiscovery();
	preparePairing();
}


//////////////////////////////////////////////////
// CommsChannel slots


void RemoteWindow::onError(QString e)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW comms: error "<<e;
}

void RemoteWindow::onClientAdded(CommsSession *c)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW comms: client added "<<(0==c?"null":QString::number(c->localSessionID(),16));
}

void RemoteWindow::onConnectionStatusChanged(bool c)
{
	OC_METHODGATE();
	qDebug()<<"REMOTEW comms: OC_NEW connection status: "<<c;
	//ui->widgetConnection->setConnectState(c?ON:OFF);
}

//////////////////////////////////////////////////
// Internal sensor slots

void RemoteWindow::onPositionUpdated(const QGeoPositionInfo &info)
{
	OC_METHODGATE();
//	ui->labelGPS->setText("GPS: "+QString::number(info.coordinate().latitude())+", "+QString::number(info.coordinate().longitude()));
	appendLog("GPS update: "+info.coordinate().toString()+"@"+info.timestamp().toString());
}


void RemoteWindow::onCompassUpdated(QCompassReading *r)
{
	OC_METHODGATE();
//	ui->labelCompass->setText("COMPASS: "+QString::number(r->azimuth()));
}

void RemoteWindow::onAccelerometerUpdated(QAccelerometerReading *r)
{
	OC_METHODGATE();
//	ui->labelAccelerometer->setText("ACCEL: <"+QString::number(r->x())+", "+QString::number(r->y())+", "+ QString::number(r->z())+">");
}

void RemoteWindow::onGyroscopeUpdated(QGyroscopeReading *r)
{
	OC_METHODGATE();
	//ui->labelGyroscope->setText("GYRO: <"+QString::number(r->x())+", "+QString::number(r->y())+", "+ QString::number(r->z())+">");
}


///////////////////////////////////
//Internal UI slots




void RemoteWindow::on_comboBoxAgent_currentIndexChanged(int index)
{
	OC_METHODGATE();
	updateActiveAgent();
}

void RemoteWindow::on_comboBoxControlLevel_activated(const QString &cLevel)
{
	Settings *settings=(nullptr!=mRemote)?(&mRemote->settings()):nullptr;
	if(nullptr!=settings) {
		settings->setCustomSetting("octomy.remote.control.level",cLevel);
	}
	updateControlLevel();
}


void RemoteWindow::on_pushButtonConfirmQuit_clicked()
{
	OC_METHODGATE();
	exit(0);
}


void RemoteWindow::on_pushButtonMenu_clicked()
{
	OC_METHODGATE();
	mMenu.exec(mapToGlobal(ui->pushButtonMenu->pos()));
}

void RemoteWindow::on_pushButtonBack_6_clicked()
{
	OC_METHODGATE();
	ui->stackedWidgetScreen->setCurrentWidget(ui->pageRunning);
}

void RemoteWindow::on_pushButtonStartPairing_clicked()
{
	OC_METHODGATE();
	ui->widgetPairing->reset();
	ui->stackedWidgetScreen->setCurrentWidget(ui->pagePairing);
}
