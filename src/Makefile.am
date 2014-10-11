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
	database/base_info.c \
	database/users.c \
	database/privacy_block.c \
	database/day_night_mode.c \
	database/szyc.c \
	database/osd.c \
	database/video.c \
	database/image.c \
	database/network.c \
	database/network_static.c \
	database/network_pppoe.c \
	database/network_port.c \
	database/datetime.c \
	database/misc.c \
	database/event_input.c \
	ajax/http_response.c \
	ajax/http_request.c \
	ajax/http_query_string_parser.c \
	ajax/http_request_handler.c \
	ajax/http_base_info_handler.c \
	ajax/http_proc.c \
	ajax/http_parser.c \
	ajax/ajax.c \
	ctemplate.c \
	iconfig.c \
	common.c \
	main.c \
	ajax/http_osd_handler.c \
	ajax/http_video_handler.c \
	ajax/http_image_handler.c \
	ajax/http_network_handler.c \
	ajax/http_datetime_handler.c \
	ajax/http_users_handler.c \
	ajax/http_misc_handler.c \
	ajax/http_privacy_block_handler.c \
	ajax/http_day_night_mode_handler.c \
	ajax/http_szyc_handler.c \
	ajax/http_event_input_handler.c \
	msg-handler/ipcam-message-handler.c \
	msg-handler/ipcam-base-info-handler.c \
	msg-handler/ipcam-osd-handler.c \
	msg-handler/ipcam-video-handler.c \
	msg-handler/ipcam-image-handler.c \
	action-handler/generic_action_handler.c \
	msg-handler/ipcam-network-handler.c \
	msg-handler/ipcam-datetime-handler.c \
	msg-handler/ipcam-users-handler.c \
	msg-handler/ipcam-misc-handler.c \
	msg-handler/ipcam-privacy-block-handler.c \
	msg-handler/ipcam-day-night-mode-handler.c \
	msg-handler/ipcam-szyc-handler.c \
	msg-handler/ipcam-event-input-handler.c \
	sysutils.c

iconfig_LDFLAGS = 

iconfig_LDADD = $(ICONFIG_LIBS) -lhttp_parser


SUBDIRS = \
	config
