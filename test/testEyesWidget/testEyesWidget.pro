include($$TESTS/pris/test.pri)

TARGET = test_eyeswidget

HEADERS += \
	TestEyesWidget.hpp

SOURCES += \
	TestEyesWidget.cpp

include($$PRIS/status.pri)
include($$TESTS/pris/test_autorun.pri)

