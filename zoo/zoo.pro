TEMPLATE = app
TARGET = zoo

# ZOO REPRESENTS THE WEB COMUNITY. YES, IT IS A WEBAPP MADE IN QT!
# YOU DONT NEED THIS BUT IF YOU WANT TO START YOUR OWN ZOO YOU CAN
# PLEASE NOTE THAT INTERFERING WITH THE OFFICIAL INSTANCE OF OCTOMY ZOO
# IS STRONGLY FROWNED UPPON BY THE COMUNITY. BE NICE!

# Start with common for project...
include($$TOP_PWD/common.pri)
include($$TOP_PWD/app.pri)
include($$TOP_PWD/libs/all_libs.pri)

# ... but ZOO is a server app, so better start acting like one!
CONFIG += console


HEADERS += \
	ZooMain.hpp

SOURCES += \
	ZooMain.cpp

contains(DEFINES, USE_STATUS){
message("FROM zoo.pro:")
include($$TOP_PWD/status.pri)
}
