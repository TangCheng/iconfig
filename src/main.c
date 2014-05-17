/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2014 TangCheng <tangcheng_@163.com>
 * 
 */

#include <stdio.h>
#include "iconfig.h"

int main()
{
	IpcamIConfig *iconfig = g_object_new(IPCAM_ICONFIG_TYPE, "name", "iconfig", NULL);
	ipcam_base_service_start(IPCAM_BASE_SERVICE(iconfig));
	return (0);
}

