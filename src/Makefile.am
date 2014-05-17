## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(ICONFIG_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bindir = $(prefix)/iconfig
bin_PROGRAMS = iconfig

iconfig_SOURCES = \
	database.c \
	base_info.c \
	users.c \
	iconfig.c \
	main.c

iconfig_LDFLAGS = 

iconfig_LDADD = $(ICONFIG_LIBS)


SUBDIRS = \
	config
