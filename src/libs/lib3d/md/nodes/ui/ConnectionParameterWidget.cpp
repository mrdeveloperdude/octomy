#include "ConnectionParameterWidget.hpp"
#include "ui_ConnectionParameterWidget.h"

#include "md/nodes/Connection.hpp"
#include "md/nodes/Node.hpp"

ConnectionParameterWidget::ConnectionParameterWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ConnectionParameterWidget)
{
	ui->setupUi(this);
}

ConnectionParameterWidget::~ConnectionParameterWidget()
{
	delete ui;
}

void ConnectionParameterWidget::setConnection(QSharedPointer<Connection> connection){
	mConnection = connection;
	setWindowTitle(QString("Connection %1 <--> %2").arg(mConnection->from()->name()).arg(mConnection->to()->name()));
}
