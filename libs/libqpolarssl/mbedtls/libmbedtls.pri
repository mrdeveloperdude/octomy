
MBEDTLS_IN_DIR =	$$TOP_PWD/libs/libmbedtls
MBEDTLS_OUT_DIR =	$$TOP_BUILD/libs/libmbedtls
LIBS +=				-L$$AGENT_OUT_DIR -lmbedtls
INCLUDEPATH +=		$$MBEDTLS_IN_DIR/include
DEPENDPATH +=		$$MBEDTLS_IN_DIR
PRE_TARGETDEPS +=	$$MBEDTLS_OUT_DIR/libmbedtls.a
