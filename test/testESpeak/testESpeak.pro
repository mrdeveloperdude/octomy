include($$TESTS/test.pri)

TARGET = test_espeak

HEADERS += \
	TestESpeak.hpp

SOURCES += \
	TestESpeak.cpp

include($$PRIS/status.pri)
include($$TESTS/test_autorun.pri)

