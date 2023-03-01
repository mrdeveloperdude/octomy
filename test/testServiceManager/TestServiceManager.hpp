#ifndef TESTSERVICEMANAGER_HPP
#define TESTSERVICEMANAGER_HPP

#include "test/Common.hpp"

class TestServiceManager:public QObject
{
	Q_OBJECT

private slots:
	void testHelpers();
	void testDirectRegistrationAndActivation();
	void testServiceLevels();

};


#endif
// TESTSERVICEMANAGER_HPP
