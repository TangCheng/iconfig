## Process this file with automake to produce Makefile.in

## Created by Anjuta

AUTOMAKE_OPTIONS = subdir-objects

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
	database/database.c \
	database/base_info.c \
	database/users.c \
	database/osd.c \
	database/video.c \
	database/scene.c \
	database/network.c \
	database/network_static.c \
	database/network_pppoe.c \
	database/network_port.c \
	database/datetime.c \
	ajax/http_response.c \
	ajax/http_request.c \
	ajax/http_parser.c \
	ajax/ajax.c \
	iconfig.c \
	main.c

iconfig_LDFLAGS = 

iconfig_LDADD = $(ICONFIG_LIBS) -lhttp_parser


SUBDIRS = \
	config
