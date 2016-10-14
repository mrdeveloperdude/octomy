 TEMPLATE = subdirs

SUBDIRS += \
	libcore \
	libutil \
	libpki \
	libparser \
	libmap \
	libqr \
	libzbar \
	libqhttp \
	liboctostyle \
	libqfi \
	libcl \


# Dependency stuff adapted from https://vilimpoc.org/blog/2014/02/21/qmake-subdirs-project-automatic-dependencies/
libcore.subdir=			libcore
libutil.subdir=			libutil
libpki.subdir=			libpki
libparser.subdir=		libparser
libmap.subdir=			libmap libcore
libqr.subdir=			libqr
libzbar.subdir=			libzbar
libqhttp.subdir=		libqhttp
liboctostyle.subdir=	libstyle
libqfi.subdir=			libqfi
libcl.subdir=			libcl

libcore.depends=libutil

include($$TOP_PWD/common.pri)


