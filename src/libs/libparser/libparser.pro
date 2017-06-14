TARGET =	parser
TEMPLATE =	lib
CONFIG +=	staticlib

include($$PRIS/common.pri)
include($$PRIS/lib.pri)
#QT         += core core-private gui gui-private
INCLUDEPATH += $$SRCS/libs/libparser/

# Link to flex library
LIBS += -lfl

# Specify flex sources.
# NOTE: the basename of this file will need to match class names etc.
#       Please see extra comments inside file for more details.
FLEXSOURCES = octomy.l

# Specify qlalr sources.
# NOTE: the basename of this file will need to match class names etc.
#       Please see extra comments inside file for more details.
QLALRSOURCES = octomy.g

# Include the qmake_extra_compilers for flex and qlalr
include(flex.pri)
include(qlalr.pri)

OTHER_FILES += \
	$$FLEXSOURCES \
	$$QLALRSOURCES \

HEADERS += \
	Context.hpp \
	ParseTreeNode.hpp \
	ParseError.hpp \
	OctomyParseContext.hpp \

SOURCES += \
	Context.cpp \
	ParseTreeNode.cpp \
	ParseError.cpp \
	OctomyParseContext.cpp \

contains(DEFINES, USE_STATUS){
message("FROM libparser.pro")
include($$PRIS/status.pri)
}
