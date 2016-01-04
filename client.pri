#Align these with the actual build of Qt in use (see image/scripts/qt.sh for options)
CONFIG += qt c++11 # xcb debug thread 3dnow mmx stl sse sse2 largefile
CONFIG -= rtti exceptions c++0x c++14
CONFIG += static
#CONFIG += console

# RUN TESTS ON RELEASE BUILD
# NOTE: RUN_WEB_TESTS SHOULD BE ON WHEN BUILDING MACHINE IS CONNECTED TO INTERNET
# NOTE: RUN_DISPENSER_TESTS SHOULD BE ON WHEN BUILDING MACHINE IS CONNECTED TO DISPENSER

DEFINES += USE_BASIC_TESTS
#DEFINES += USE_WEB_TESTS
#DEFINES += USE_DISPENSER_TESTS

# OPTIONAL FEATURES AND EXTERNAL LIBS
# NOTE: ALL ARE OFF BY DEFAULT TO KEEP DEPENDENCIES LOW & PREDICTABLE
# NOTE: PLEASE USE THE local_overrides.pri FILE TO INCLUDE THEM (SEE BELOW)
# NOTE: LISTED HERE AS A REFERENCE ONLY

#DEFINES += USE_FEATURE_BUFFER_HONEYPOT
#DEFINES += USE_FEATURE_BOTTLE_RIG
#DEFINES += USE_FEATURE_FACTORY_RIG
#DEFINES += USE_FEATURE_XINPUT_CALIBRATOR
#DEFINES += EXTERNAL_LIB_UFR
#DEFINES += EXTERNAL_LIB_NODAVE
#DEFINES += EXTERNAL_LIB_QRENCODE

# PUT YOUR LOCAL OVERRIDES IN THIS FILE AND DON'T INCLUDE IT IN GIT!
!include( local_overrides.pri ) {
	LOCAL_OVERRIDES=false
}else{
	LOCAL_OVERRIDES=true
}

contains(DEFINES, USE_FEATURE_BOTTLE_RIG){
	DEFINES += EXTERNAL_LIB_ESPEAK
	DEFINES += EXTERNAL_LIB_OPENCV
}


# Add only plugins that are used
QT += core gui opengl widgets xml network multimedia multimediawidgets svg quick sql testlib


static {
	DEFINES += STATIC
}

shared {
	DEFINES += SHARED
}

dynamic {
	DEFINES += DYNAMIC
}

# ensure one "debug_and_release" in CONFIG, for clarity...
debug_and_release {
	CONFIG -= debug_and_release
	CONFIG += debug_and_release
}

# ensure one "debug" or "release" in CONFIG so they can be used as
# conditionals instead of writing "CONFIG(debug, debug|release)"...
CONFIG(debug, debug|release) {
	CONFIG -= debug release
	CONFIG += debug
}

CONFIG(release, debug|release) {
	CONFIG -= debug release
	CONFIG += release
}

release{
	DEFINES += RELEASE
}

debug{
	DEFINES += DEBUG
}


# Strive towards the most modern standard of C++ language available
QMAKE_CXXFLAGS -= -std=c++14
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS -= -std=c++0x


# Add support for ccache
#unix:exists(/usr/bin/ccache):QMAKE_CXX=ccache $$QMAKE_CXX

# The most comprehensive and strict set of checks available
QMAKE_CXXFLAGS += -pedantic -Wall -Wextra
QMAKE_CXXFLAGS += -Wredundant-decls
#QMAKE_CXXFLAGS += -Wsign-promo
QMAKE_CXXFLAGS += -Wmissing-include-dirs   -Woverloaded-virtual
QMAKE_CXXFLAGS += -Wformat=2 -Winit-self  -Wmissing-declarations
QMAKE_CXXFLAGS += -Wcast-qual  -Wdisabled-optimization
#QMAKE_CXXFLAGS += -Wcast-align # Warn about alignment changes due to casts. Not really useful at this time so commented out
QMAKE_CXXFLAGS += -Wstrict-overflow=5
#QMAKE_CXXFLAGS += -Wundef # Warn about referenced but undefined preprocessor defines


#Only relevant in GPP
g++ {
		 QMAKE_CXXFLAGS += -Wstrict-null-sentinel
		 QMAKE_CXXFLAGS += -Wnoexcept
		 QMAKE_CXXFLAGS += -Wlogical-op
		 QMAKE_CXXFLAGS += -Wstack-usage=2048
}

# Some hotshot with a book suddenly got his very own warning in GCC attributed to it.
#QMAKE_CXXFLAGS += -Weffc++

# Remove redundant/annoying warnings
QMAKE_CXXFLAGS -= -Wold-style-cast -Wctor-dtor-privacy -Wshadow -Wswitch-default
QMAKE_CXXFLAGS -= -Wsign-conversion -Wno-gnu-array-member-paren-init
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-format-contains-nul  -Wno-format-security -Wno-vla
#Make stacktrace beautiful dureing debugging
QMAKE_CXXFLAGS += -fno-omit-frame-pointer
# Brave...
#QMAKE_CXXFLAGS += -Werror

# Build in all available bug protection
QMAKE_CXXFLAGS += -fstack-protector-all -fstrict-overflow -fstack-usage
#QMAKE_CXXFLAGS +=-fsanitize=address

#    Enable AddressSanitizer, a fast memory error detector. Memory access instructions will be instrumented to detect out-of-bounds and use-after-free bugs. See http://code.google.com/p/address-sanitizer/ for more details. The run-time behavior can be influenced using the ASAN_OPTIONS environment variable; see https://code.google.com/p/address-sanitizer/wiki/Flags#Run-time_flags for a list of supported options.
#QMAKE_CXXFLAGS +=-fsanitize=kernel-address
#    Enable AddressSanitizer for Linux kernel. See http://code.google.com/p/address-sanitizer/wiki/AddressSanitizerForKernel for more details.
#QMAKE_CXXFLAGS +=-fsanitize=thread

#include(asan.pri)

#WE ACTUALLY HAVE SOME PURE C CODE AS WELL (snap7 etc)

QMAKE_CFLAGS += -Wno-unused-parameter -Wno-sometimes-uninitialized


# Remove GCC-only stuff if other compilers are used. PS: contains() is used to avoid problems when ccache is used
!contains(QMAKE_CXX, g++){
	QMAKE_CXXFLAGS -= -fstack-usage -Wno-format-contains-nul
}



# Do not optimize by default
QMAKE_CXXFLAGS -= -O
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O3

# Optimize for size in release
release{
	QMAKE_CXXFLAGS += -Os
}

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_DEBUG -= -O
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O3

QMAKE_CXXFLAGS_RELEASE += -Os

QMAKE_LFLAGS += $$QMAKE_CXXFLAGS

DISTFILES += \
	$$PWD/local_overrides.pri

