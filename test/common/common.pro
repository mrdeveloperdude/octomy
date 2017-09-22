TEMPLATE=lib
TARGET=common

# Make this library behave like the others
include($$PRIS/common.pri)
include($$PRIS/lib_headers.pri)
# Add the test libraries
QT += testlib
CONFIG += staticlib


SOURCES+= \
	CourierTester.cpp \
    Corruptifier.cpp

HEADERS+= \
	CourierTester.hpp \
    Corruptifier.hpp


common.depends=		$${SRCS}/libs
