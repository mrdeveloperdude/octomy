#include "PortableIDWidget.hpp"
#include "ui_PortableIDWidget.h"

#include "uptime/MethodGate.hpp"
#include "uptime/New.hpp"

#include <QDateTime>

PortableIDWidget::PortableIDWidget(QWidget *parent)
	: QWidget(parent)
	, ui(OC_NEW Ui::PortableIDWidget)
{
	OC_METHODGATE();
	ui->setupUi(this);
	configure(true,false);
}

PortableIDWidget::~PortableIDWidget()
{
	OC_METHODGATE();
	delete ui;
	ui=nullptr;
}

void PortableIDWidget::configure(bool showCertificateFirst, bool userCanChange)
{
	OC_METHODGATE();
	ui->pushButtonToggleView->setVisible(userCanChange);
	ui->stackedWidget->setCurrentWidget(showCertificateFirst?ui->pageCertificate:ui->pageNameplate);
}



void PortableIDWidget::setPortableID(PortableID id)
{
	OC_METHODGATE();
	//qDebug()<<"portable id widget was updated with "<<id;
	mID=id;
	const NodeType type=mID.type();
	const bool isAgent=TYPE_AGENT==type;
	ui->groupBoxBirthCertificate->setTitle(isAgent?"Birth Certificate": "Identification");

	ui->labelType->setVisible(!isAgent);
	ui->labelTypeCaption->setVisible(!isAgent);
	ui->labelType->setText(nodeTypeToString(type));

	ui->labelName->setVisible(isAgent && !mID.name().isEmpty());
	ui->labelNameCaption->setVisible(isAgent && !mID.name().isEmpty());
	ui->labelName->setText(mID.name());

	ui->labelGender->setVisible(isAgent && !mID.gender().isEmpty());
	ui->labelGenderCaption->setVisible(isAgent && !mID.gender().isEmpty());
	ui->labelGender->setText(mID.gender());

	ui->labelIDCaption->setText(isAgent?"Soul ID": "ID");
	ui->labelID->setText(mID.id());

	ui->widgetIdenticon->setPortableID(id);
	ui->widgetQR->setQRData(mID.id());
	const quint64 ts=mID.birthDate();
	if(ts>0) {
		ui->labelBirthdate->setVisible(true);
		ui->labelBirthdateCaption->setVisible(true);
		ui->labelBirthdate->setText(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(ts)).toString(PortableID::dateFMT));
		ui->labelBirthdateCaption->setText(isAgent?"Birth date": "Create date");
	} else {
		ui->labelBirthdate->setVisible(false);
		ui->labelBirthdateCaption->setVisible(false);
	}

	ui->widgetBadgeIdenticon->setPortableID(id);
	ui->labelBadgeName->setText(mID.identifier());
	ui->labelBadgeName->setToolTip(mID.toPortableString());

	//ui->widgetIdenticon->setMinimumHeight(100);	ui->widgetQR->setMinimumHeight(100);
	update();
}

PortableID PortableIDWidget::getPortableID()
{
	OC_METHODGATE();
	return mID;
}

void PortableIDWidget::on_pushButtonToggleView_toggled(bool checked)
{
	OC_METHODGATE();
	ui->stackedWidget->setCurrentWidget(checked?ui->pageCertificate:ui->pageNameplate);
}
