TEMPLATE=lib
TARGET=common

# Make this library behave like the others
include($$PRIS/common.pri)
include($$PRIS/lib_headers.pri)
# Add the test libraries
QT += testlib


SOURCES+= \
	CourierTester.cpp

HEADERS+= \
	CourierTester.hpp




