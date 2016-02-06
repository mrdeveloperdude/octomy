# Include helper for the library project

# IMPORTANT: TOP_PWD and TOP_BUILD are defined in .qmake.config in project root

OCTOMY_IN_DIR = $$TOP_PWD/lib
OCTOMY_OUT_DIR = $$TOP_BUILD/lib

LIBS+=-L$$OCTOMY_OUT_DIR -loctomy
INCLUDEPATH+=$$OCTOMY_IN_DIR
DEPENDPATH+=$$OCTOMY_IN_DIR
PRE_TARGETDEPS+=$$OCTOMY_OUT_DIR/liboctomy.a


contains(DEFINES, ASSIMP_LIB){
ASSIMP_IN_DIR = $$TOP_PWD/assimp
ASSIMP_OUT_DIR = $$TOP_BUILD/assimp

LIBS+=-L$$ASSIMP_OUT_DIR -lassimp
INCLUDEPATH+=$$ASSIMP_IN_DIR
DEPENDPATH+=$$ASSIMP_IN_DIR
PRE_TARGETDEPS+=$$ASSIMP_OUT_DIR/libassimp.a

}
