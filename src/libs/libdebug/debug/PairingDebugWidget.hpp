#ifndef PAIRINGDEBUGWIDGET_HPP
#define PAIRINGDEBUGWIDGET_HPP

#include "uptime/ConfigureHelper.hpp"


#include <QWidget>
#include <QSharedPointer>
#include <QTimer>

class Node;
class PairingTrustActivity;

namespace Ui
{
class PairingDebugWidget;
}

class PairingDebugWidget : public QWidget
{
	Q_OBJECT
private:
	Ui::PairingDebugWidget *ui;
	QSharedPointer <Node> mNode;
	QSharedPointer <PairingTrustActivity> mTrustWidget;
	ConfigureHelper mConfigureHelper;

public:
	explicit PairingDebugWidget(QWidget *parent = nullptr);
	virtual ~PairingDebugWidget() override;

public:
	void configure(QSharedPointer <Node> node);

public slots:
	void onUpdate();

};

#endif
// PAIRINGDEBUGWIDGET_HPP


