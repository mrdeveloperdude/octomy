
QFI_IN_DIR =		$$TOP_PWD/libs/libqfi
QFI_OUT_DIR =		$$TOP_BUILD/libs/libqfi
LIBS +=				-L$$QFI_OUT_DIR -lqfi
INCLUDEPATH +=		$$QFI_IN_DIR
DEPENDPATH +=		$$QFI_IN_DIR
PRE_TARGETDEPS +=	$$QFI_OUT_DIR/libqfi.a
