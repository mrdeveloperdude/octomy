include($$TOP_PWD/pri/common.pri)
include($$TOP_PWD/libs/libs_inc.pri)

CONFIG += testcase
QT += testlib

DISTFILES += \
	$$PWD/test_autorun.pri


TESTCOMMON_IN_DIR =		$$TOP_PWD/test/testCommon
TESTCOMMON_OUT_DIR =	$$TOP_BUILD/test/testCommon
LIBS +=					-L$$TESTCOMMON_OUT_DIR -ltest_common
INCLUDEPATH +=			$$TESTCOMMON_IN_DIR
DEPENDPATH +=			$$TESTCOMMON_IN_DIR
PRE_TARGETDEPS +=		$$TESTCOMMON_OUT_DIR/libtest_common.so


