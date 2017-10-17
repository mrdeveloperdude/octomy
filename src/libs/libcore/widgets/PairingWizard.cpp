#include "PairingWizard.hpp"
#include "ui_PairingWizard.h"


#include "basic/Settings.hpp"
#include "node/Node.hpp"
#include "identity/Identicon.hpp"
#include "basic/Associate.hpp"
#include "security/PortableID.hpp"
#include "utility/Standard.hpp"

#include "basic/AddressEntry.hpp"
#include "models/PairingListModel.hpp"
#include "PairingEditButtonDelegate.hpp"

#include <QDebug>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QUrl>

#include <QWidget>
#include <QItemDelegate>
#include <QMouseEvent>
#include <QPainter>
#include <QSvgRenderer>
#include <QMessageBox>

// QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=mTiqZm-Ea70", QUrl::TolerantMode));

#include "zoo/ZooConstants.hpp"



PairingWizard::PairingWizard(QWidget *parent)
	: QWidget(parent)
	, ui(OC_NEW Ui::PairingWizard)
	, mNode(nullptr)
	, mList(nullptr)
	, mDelegate (nullptr)

{
	ui->setupUi(this);
	ui->widgetMyCertificate->configure(false,true);
	mTemplate=ui->labelBodyPair->text();
	ui->labelBodyPair->setText("<h1>Please wait...<h1>");

	reset();

	// Hook onward buttons to go to the correct page in stack
	QList<QPushButton *> onwardButtons = ui->stackedWidget->findChildren<QPushButton *>(QRegularExpression("pushButtonOnward.*"));
	//qDebug()<<"FOUND "<<onwardButtons.size()<<" ONWARDs";
	for (QList<QPushButton*>::iterator it = onwardButtons.begin(), e=onwardButtons.end(); it != e; ++it) {
		QPushButton*onward=(*it);
		//qDebug()<<" + ONWARD: "<<onward->objectName();
		connect(onward, &QPushButton::clicked,this,[=](bool b) {
			// Skip pages that are not relevant to the selection made in "basic" page
			int next = (ui->stackedWidget->currentIndex() + 1) % ui->stackedWidget->count();
			ui->stackedWidget->setCurrentIndex(next);
		}, OC_CONTYPE);
	}
}


void PairingWizard::onNetworkSettingsChange(QHostAddress address, quint16 port, bool valid)
{
	if(nullptr!=mNode) {
		if(valid) {
			mNode->localAddresses().setCurrent(address, port);
		}
	}
	updateNetworkSettings();
}


void PairingWizard::updateNetworkSettings()
{
	if(nullptr!=mNode) {
		DiscoveryClient *client=mNode->discoveryClient();
		if(nullptr!=client) {
			const bool visible=this->isVisible();
			//TODO: Only attemt to start discovery client when address is valid
			const bool valid=true;//mNode->localAddresses().currentAddress().isValid()
			if(visible) {
				if(!client->isStarted()) {
					client->start();
				}
				ui->progressBarSearching->setVisible(true);
			} else {
				if(client->isStarted()) {
					client->stop();
				}
				ui->progressBarSearching->setVisible(false);
			}
		}
	}
}

PairingWizard::~PairingWizard()
{
	delete ui;
	ui=nullptr;
}


void PairingWizard::configure(Node *n)
{
	mNode=n;
	if(nullptr!=mNode) {


		DiscoveryClient *discovery=mNode->discoveryClient();
		DiscoveryType type=mNode->type();
		QSharedPointer<Associate>  ass=mNode->nodeIdentity();
		if(nullptr!=ass) {
			PortableID pid=ass->toPortableID();
			//qDebug()<<"CONFIGURE PAIRING WIZ FOR "<<pid.toPortableString();
			ui->widgetMyCertificate->setPortableID(pid);
			if(nullptr==ui->listViewNodes->model()) {

				mList=OC_NEW PairingListModel(mNode->peers(),type,*this);
				ui->listViewNodes->setModel(mList);

				if(nullptr==mDelegate) {
					mDelegate=OC_NEW PairingEditButtonDelegate(*this);
				}
				ui->listViewNodes->setItemDelegate(mDelegate);
			}
			if(!connect(discovery, &DiscoveryClient::nodeDiscovered, [=](QString partID) {
			//qDebug()<<"PAIRING WIZARD partID: "<<partID;
			ui->listViewNodes->update();
			}
					   )) {
				qWarning()<<"ERROR: Could not connect";
			}


			switch(type) {
			case(TYPE_ZOO):
			default:
			case(TYPE_UNKNOWN):
			case(TYPE_AGENT): {
				ui->labelBodyPair->setText(mTemplate.replace(QRegularExpression("\\[SOURCE\\]"), "Agent").replace(QRegularExpression("\\[DEST\\]"), "Control"));
				ui->stackedWidgetNoMessage->setCurrentWidget(ui->pageNoMessageAgent);
			}
			break;
			case(TYPE_REMOTE): {
				ui->labelBodyPair->setText(mTemplate.replace(QRegularExpression("\\[SOURCE\\]"), "Remote").replace(QRegularExpression("\\[DEST\\]"), "Agent"));
				ui->stackedWidgetNoMessage->setCurrentWidget(ui->pageNoMessageControl);
			}
			break;
			case(TYPE_HUB): {
				ui->labelBodyPair->setText(mTemplate.replace(QRegularExpression("\\[SOURCE\\]"), "Hub").replace(QRegularExpression("\\[DEST\\]"), "Agent"));
				ui->stackedWidgetNoMessage->setCurrentWidget(ui->pageNoMessageControl);
			}
			break;
			}

			if(!connect(ui->widgetNetworkSettings, &NetworkSettingsWidget::addressChanged, this, &PairingWizard::updateNetworkSettings, OC_CONTYPE)) {
				qWarning()<<"ERROR: Could not connect";
			}

			ui->widgetNetworkSettings->set(mNode->localAddress().ip(), mNode->localAddress().port());

		} else {
			qWarning()<<"ERROR: No local ass";
		}

	} else {
		qWarning()<<"ERROR: No node";
	}
}


void PairingWizard::reset()
{
	ui->stackedWidget->setCurrentWidget(ui->pagePairWithPeers);
}


void PairingWizard::startEdit(int row)
{
	qDebug()<<"STARTING EDIT FOR "<<row;
	QModelIndex index=mList->index(row,0);
	if(index.isValid()) {
		//setUpdatesEnabled(false);
		QVariantMap map=index.data(Qt::DisplayRole).toMap();
		qDebug()<<"DATA FOR "<<row<<" DURING EDIT IS: "<<map;
		if(nullptr!=mNode) {
			AddressBook &peerStore=mNode->peers();
			mCurrentlyEditing=map["key"].toMap()["id"].toString();
			qDebug()<<"CURRENTLY EDITING ID "<<mCurrentlyEditing;
			QSharedPointer<Associate> peer=peerStore.associateByID(mCurrentlyEditing);
			if(nullptr!=peer) {
				const QStringList trusts=peer->trusts();
				const DiscoveryType type=peer->type();
				const bool take=trusts.contains("take-control");
				const bool give=trusts.contains("give-control");
				const bool block=trusts.contains("block");
				int index=0;
				if(block) {
					index=2;
				} else {
					switch(type) {
					default:
					case(TYPE_ZOO):
					case(TYPE_UNKNOWN):
					case(TYPE_AGENT): {
						if(give) {
							index=1;
						}
					}
					break;
					case(TYPE_REMOTE): {
						if(take) {
							index=1;
						}
					}
					break;
					case(TYPE_HUB): {
						if(give || take) {
							index=1;
						}
					}
					break;
					}
				}
				ui->comboBoxTrust->setCurrentIndex(index);
				qDebug()<<"EDITING STARTS WITH trusts: "<<peer->trusts();
				qDebug()<<"EDITING STARTS WITH name: "<<peer->name();
				ui->widgetParticipantCertificate->configure(false,true);
				ui->widgetParticipantCertificate->setPortableID(peer->toPortableID());
			}
		}
		ui->stackedWidget->setCurrentWidget(ui->pagePeerDetail);
		//	setUpdatesEnabled(true);
	} else {
		qWarning()<<"ERROR: Index was invalid for row "<<row;
	}
}

Node *PairingWizard::getNode()
{
	return mNode;
}

void PairingWizard::showEvent(QShowEvent *)
{
	updateNetworkSettings();
}

void PairingWizard::hideEvent(QHideEvent *)
{
	updateNetworkSettings();
}

void PairingWizard::on_pushButtonMaybeOnward_clicked()
{
	if(nullptr!=mNode) {
		AddressBook &store=mNode->peers();
		if(store.all().size()>0) {
			emit done();
			return;
		}
	}
	ui->stackedWidget->setCurrentWidget(ui->pageNoPeers);
}


void PairingWizard::on_pushButtonTryAgain_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->pagePairWithPeers);
}

void PairingWizard::on_pushButtonDone_clicked()
{
	emit done();
}


void PairingWizard::on_pushButtonCameraPair_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->pageCameraPairing);
}

void PairingWizard::on_pushButtonSaveEdits_clicked()
{
	qDebug()<<"SAVING AFTER EDIT OF "<<mCurrentlyEditing;
	if(nullptr!=mNode) {
		AddressBook &peers=mNode->peers();
		QSharedPointer<Associate> peer=peers.associateByID(mCurrentlyEditing);
		if(nullptr!=peer) {
			DiscoveryType type=peer->type();
			const int index=ui->comboBoxTrust->currentIndex();
			peer->removeTrust("take-control");
			peer->removeTrust("give-control");
			peer->removeTrust("block");
			if(2==index) {
				peer->addTrust("block");
			} else if(1==index) {
				switch(type) {
				default:
				case(TYPE_ZOO):
				case(TYPE_UNKNOWN):
				case(TYPE_AGENT): {
					peer->addTrust("give-control");
				}
				break;
				case(TYPE_REMOTE): {
					peer->addTrust("take-control");
				}
				break;
				case(TYPE_HUB): {
					peer->addTrust("take-control");
					peer->addTrust("give-control");
				}
				break;
				}
			}
			qDebug()<<"EDITING ENDS WITH trusts: "<<peer->trusts();
			peers.save();
		} else {
			qWarning()<<"ERROR: No peer while saving trust edits";
		}
		//ui->listViewNodes->update();
		ui->stackedWidget->setCurrentWidget(ui->pagePairWithPeers);
	} else {
		qWarning()<<"ERROR: No node while saving trust edits";
	}
	mCurrentlyEditing="";
}

void PairingWizard::on_pushButtonRemove_clicked()
{
	if (QMessageBox::Yes == QMessageBox::question(this, "Warning", "Are you sure you want to permanently DELETE this peer?", QMessageBox::Yes|QMessageBox::No)) {

		if(nullptr!=mNode) {
			AddressBook &peers=mNode->peers();
			peers.removeAssociate(mCurrentlyEditing);
			peers.save();
		}
		ui->stackedWidget->setCurrentWidget(ui->pagePairWithPeers);
	}
}

void PairingWizard::on_pushButtonRefresh_clicked()
{
	qDebug()<<mList->status();
}
