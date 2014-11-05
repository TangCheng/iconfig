## Process this file with automake to produce Makefile.in

## Created by Anjuta

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPACKAGE_SYSCONFDIR=\""$(sysconfdir)"\" \
	$(ICONFIG_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bindir = $(prefix)/iconfig
bin_PROGRAMS = iconfig

iconfig_SOURCES = \
	database/database.c \
	database/users.c \
	database/params.c \
	msg-handler/ipcam-message-handler.c \
	msg-handler/ipcam-params-handler.c \
	msg-handler/ipcam-users-handler.c \
	action-handler/generic_action_handler.c \
	iconfig.c \
	common.c \
	main.c \
	sysutils.c

iconfig_LDFLAGS = 

iconfig_LDADD = $(ICONFIG_LIBS)


SUBDIRS = \
	config
