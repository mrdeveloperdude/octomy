#include "TestParser.hpp"

#include "octomy_parser.hpp"

// YOU NEED THIS: http://doc.qt.io/qt-5/qtest.html
void TestParser::test(){
	OctomyParser p;
	p.parse("plan{ remote{} hub {} agent{puppy{ memeber { } } } }");
	qDebug()<<"PARSE RESULT: "<<p.treesToString();
}



OC_TEST_MAIN(test, TestParser)
