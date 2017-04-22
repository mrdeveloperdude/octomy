TEMPLATE = subdirs

include($$TOP_PWD/common.pri)

SUBDIRS += \
	libcore \
	libutil \
	libpki \
	libparser \
	libmap \
	libqr \
	libzbar \
	libweb \
	liboctostyle \
	libqfi \
	librng \
	libaudio \
	libdynamics \
	libpuppet \



# Dependency stuff adapted from https://vilimpoc.org/blog/2014/02/21/qmake-subdirs-project-automatic-dependencies/
libcore.subdir=			libcore
libutil.subdir=			libutil
libpki.subdir=			libpki
libparser.subdir=		libparser
libmap.subdir=			libmap
libqr.subdir=			libqr
libzbar.subdir=			libzbar
libweb.subdir=		    libweb
liboctostyle.subdir=	libstyle
libqfi.subdir=			libqfi
librng.subdir=			librng
libaudio.subdir=		libaudio
libdynamics.subdir=		libdynamics
libpuppet.subdir=		libpuppet


libcore.depends= libutil librng libweb libpuppet
libaudio.depends= librng


contains(DEFINES, EXTERNAL_LIB_OPENCL){
message("ADDING LIBCLT TO BUILD")
SUBDIRS += \
	libclt \

libclt.subdir=			libclt

}
else{
message("NO LIBCLT IN BUILD")
}




contains(DEFINES, ARDUINO_ARCH_AVR){
message("ADDING ARDUINO TO BUILD")
SUBDIRS += \
	arduino \

arduino.subdir=			arduino

}
else{
message("NO ARDUINO IN BUILD")
}


include($$TOP_PWD/common.pri)


