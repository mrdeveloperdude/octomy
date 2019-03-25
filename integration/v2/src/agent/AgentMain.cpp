#include "AgentMain.hpp"

#include "app/launcher/AppLauncher.hpp"

//#include "app/launcher/MockApp.hpp"
#include "agent/Agent.hpp"

#include "uptime/SharedPointerWrapper.hpp"

int main(int argc, char *argv[])
{
	QSharedPointer<AppLauncher<Agent> > agentMain=QSharedPointer<AppLauncher<Agent> >(OC_NEW AppLauncher<Agent>());
	int ret=EXIT_FAILURE;
	if(!agentMain.isNull()) {
		agentMain->configure(argc, argv, "agent");
		ret=agentMain->run();
		agentMain.clear();
	} else {
		qWarning()<<"ERROR: Could not allocate launcher";
	}
	exit(ret);
}


