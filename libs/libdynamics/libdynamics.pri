
DYNAMICS_IN_DIR =	$$TOP_PWD/libs/libdynamics
DYNAMICS_OUT_DIR =	$$TOP_BUILD/libs/libdynamics
LIBS +=				-L$$DYNAMICS_OUT_DIR -ldynamics
INCLUDEPATH +=		$$DYNAMICS_IN_DIR
DEPENDPATH +=		$$DYNAMICS_IN_DIR
PRE_TARGETDEPS +=	$$DYNAMICS_OUT_DIR/libdynamics.a
