
SEC_IN_DIR =		$$TOP_PWD/libs/libsec
SEC_OUT_DIR =		$$TOP_BUILD/libs/libsec
LIBS +=				-L$$SEC_OUT_DIR -lsec
INCLUDEPATH +=		$$SEC_IN_DIR
DEPENDPATH +=		$$SEC_IN_DIR
PRE_TARGETDEPS +=	$$SEC_OUT_DIR/libsec.a
