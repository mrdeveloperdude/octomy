include($$TOP_PWD/test/test.pri)

TARGET = test_rng

HEADERS += \
	TestRNG.hpp

SOURCES += \
	TestRNG.cpp

include($$TOP_PWD/status.pri)
include($$TOP_PWD/test/test_autorun.pri)