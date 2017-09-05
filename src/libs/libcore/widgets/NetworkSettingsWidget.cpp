#include "NetworkSettingsWidget.hpp"
#include "ui_NetworkSettingsWidget.h"

#include "utility/Utility.hpp"

#include <QComboBox>



NetworkSettingsWidget::NetworkSettingsWidget(QWidget *parent)
	: QWidget(parent)
	, ui(OC_NEW Ui::NetworkSettingsWidget)
{
	ui->setupUi(this);
	QList<QHostAddress> localAddresses=utility::getAllLocalNetworkAddresses();
	ui->comboBoxLocalAddress->clear();
	for(QHostAddress adr:localAddresses) {
		if((QAbstractSocket::IPv4Protocol==adr.protocol()) && (!adr.isLoopback()) ) {
			ui->comboBoxLocalAddress->addItem(adr.toString());
		}
	}
	ui->comboBoxLocalAddress->setCurrentText(QHostAddress(QHostAddress::LocalHost).toString());
	setPort(0);
	if(!connect(ui->lineEditLocalPort,&QLineEdit::textChanged,this,&NetworkSettingsWidget::onPortEditChanged,OC_CONTYPE) ) {
		qWarning()<<"ERROR: Could not connect";
	}
	if(!connect(ui->comboBoxLocalAddress, SIGNAL(currentIndexChanged(int)), this, SLOT(onLocalAddressChanged(int)), OC_CONTYPE) ) {
		qWarning()<<"ERROR: Could not connect";
	}
	updateTextView();
	ui->stackedWidget->setCurrentWidget(ui->pageView);
}

NetworkSettingsWidget::~NetworkSettingsWidget()
{
	delete ui;
	ui=nullptr;
}



NetworkAddress NetworkSettingsWidget::address()
{
	return NetworkAddress(QHostAddress(ui->comboBoxLocalAddress->currentText()),quint16(ui->lineEditLocalPort->text().toInt()));
}


bool NetworkSettingsWidget::setPort(quint16 port)
{
	ui->lineEditLocalPort->setText(QString::number(port));
	return verify();
}


quint16 NetworkSettingsWidget::port()
{
	return ui->lineEditLocalPort->text().toInt();
}

bool NetworkSettingsWidget::verify(bool sendSignal)
{
	QUdpSocket udp;
	QString txt=ui->lineEditLocalPort->text();
	if(txt.isEmpty()) {
		return false;
	} else {
		quint16 port=quint16(txt.toInt());
		bool ok=udp.bind(QHostAddress(ui->comboBoxLocalAddress->currentText()), port);
		if(0==port) {
			port=udp.localPort();
		}
		ui->lineEditLocalPort->setText(QString::number(port));
		ui->widgetStatus->setLightOn(true);
		ui->widgetStatus->setLightColor(ok?Qt::green:Qt::red);
		if(sendSignal) {
			emit validityChanged(ok);
		}
		return ok;
	}
}

void NetworkSettingsWidget::updateTextView()
{
	ui->pushButtonEdit->setText(ui->comboBoxLocalAddress->currentText()+" : "+ui->lineEditLocalPort->text());
}

void NetworkSettingsWidget::onPortEditChanged()
{
	const bool last=ui->lineEditLocalPort->blockSignals(true);
	bool ok=false;
	ui->lineEditLocalPort->text().toInt(&ok);
	if(ok) {
		verify();
	}
	updateTextView();
	ui->lineEditLocalPort->blockSignals(last);
}


void NetworkSettingsWidget::onLocalAddressChanged(int index)
{
	verify();
	updateTextView();
}

void NetworkSettingsWidget::on_pushButtonEdit_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->pageEdit);
}

void NetworkSettingsWidget::on_pushButtonSave_clicked()
{
	ui->stackedWidget->setCurrentWidget(ui->pageView);
}
