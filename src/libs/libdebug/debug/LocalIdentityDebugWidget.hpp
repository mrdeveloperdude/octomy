#ifndef LOCALIDENTITYDEBUGWIDGET_HPP
#define LOCALIDENTITYDEBUGWIDGET_HPP

#include "debug/SimpleTableDebugWidget.hpp"

#include "uptime/ConfigureHelper.hpp"

#include <QMap>
#include <QSharedPointer>

class Node;


class LocalIdentityDebugWidget : public SimpleTableDebugWidget
{
	Q_OBJECT
private:
	QSharedPointer <Node> mNode;
	ConfigureHelper mConfigureHelper;

public:
	explicit LocalIdentityDebugWidget(QWidget *parent = nullptr);
	virtual ~LocalIdentityDebugWidget() override;

public:
	void configure(QSharedPointer <Node> node);

public slots:
	void onRealtimeChanged(bool realtime) override;
	void onTimer() override;

};

#endif
// LOCALIDENTITYDEBUGWIDGET_HPP