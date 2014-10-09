/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-osd-handler.c
 * Copyright (C) 2014 Watson Xu <xuhuashan@gmail.com>
 *
 * iconfig is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * iconfig is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib.h>
#include <glib/gprintf.h>
#include "ipcam-osd-handler.h"
#include "iconfig.h"
#include "common.h"

G_DEFINE_TYPE (IpcamOsdMsgHandler, ipcam_osd_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_osd_msg_handler_init (IpcamOsdMsgHandler *ipcam_osd_msg_handler)
{
}

static void
ipcam_osd_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_osd_msg_handler_parent_class)->finalize (object);
}

static gboolean
ipcam_osd_msg_handler_read_param(IpcamOsdMsgHandler *handler, JsonBuilder *builder, const gchar *name)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    gboolean isshow;
    guint size;
    guint left, top;
    Color color;

    if (ipcam_iconfig_get_osd(iconfig, name, &isshow, &size, &left, &top, &color.value))
    {
        gchar **osd_name = g_strsplit(name, ":", -1);
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, osd_name[1]);
        g_strfreev(osd_name);
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "isshow");
        json_builder_add_boolean_value(builder, isshow);
        json_builder_set_member_name(builder, "size");
        json_builder_add_int_value(builder, size);
        json_builder_set_member_name(builder, "left");
        json_builder_add_int_value(builder, left);
        json_builder_set_member_name(builder, "top");
        json_builder_add_int_value(builder, top);
        json_builder_set_member_name(builder, "color");
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "red");
        json_builder_add_int_value(builder, color.red);
        json_builder_set_member_name(builder, "green");
        json_builder_add_int_value(builder, color.green);
        json_builder_set_member_name(builder, "blue");
        json_builder_add_int_value(builder, color.blue);
        json_builder_set_member_name(builder, "alpha");
        json_builder_add_int_value(builder, color.alpha);
        json_builder_end_object(builder);
        json_builder_end_object(builder);
        json_builder_end_object(builder);
    }
    return TRUE;
}

static gboolean
ipcam_osd_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;
    gchar first_char = '\0';
    gboolean array = FALSE;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);
    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        const gchar *name = json_array_get_string_element(req_array, i);
        if (first_char != name[0])
        {
            first_char = name[0];
            if (array)
            {
                json_builder_end_array(builder);
                array = FALSE;
            }

            if (g_str_has_prefix(name, "master"))
            {
                json_builder_set_member_name(builder, "master");
            }
            else
            {
                json_builder_set_member_name(builder, "slave");
            }
            json_builder_begin_array(builder);
            array = TRUE;
        }
        ipcam_osd_msg_handler_read_param(IPCAM_OSD_MSG_HANDLER(handler), builder, name);
    }
    json_builder_end_array(builder);
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_osd_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_arr;
    JsonObject *req_obj;
    JsonObject *color_obj;
    int i;

    req_arr = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name (builder, "items");
    json_builder_begin_array (builder);
    for(i = 0; i < json_array_get_length(req_arr); i++)
    {
        const gchar *name;
        gboolean isshow;
        guint size, left, top;
        Color color;

        req_obj = json_array_get_object_element(req_arr, i);
        name = json_object_get_string_member(req_obj, "name");
        isshow = json_object_get_boolean_member(req_obj, "isshow");
        size = json_object_get_int_member(req_obj, "size");
        left = json_object_get_int_member(req_obj, "left");
        top = json_object_get_int_member(req_obj, "top");
        color_obj = json_object_get_object_member(req_obj, "color");
        color.red = json_object_get_int_member(color_obj, "red");
        color.green = json_object_get_int_member(color_obj, "green");
        color.blue = json_object_get_int_member(color_obj, "blue");
        color.alpha = json_object_get_int_member(color_obj, "alpha");

        ipcam_iconfig_set_osd(iconfig, name, isshow, size, left, top, color.value);

        ipcam_iconfig_get_osd(iconfig, name, &isshow, &size, &left, &top, &color.value);
        json_builder_begin_object(builder);
        json_builder_set_member_name (builder, "name");
        json_builder_add_string_value (builder, name);
        json_builder_set_member_name (builder, "isshow");
        json_builder_add_int_value (builder, isshow);
        json_builder_set_member_name (builder, "size");
        json_builder_add_int_value (builder, size);
        json_builder_set_member_name (builder, "left");
        json_builder_add_int_value (builder, left);
        json_builder_set_member_name (builder, "top");
        json_builder_add_int_value (builder, top);
        json_builder_set_member_name (builder, "color");
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "red");
        json_builder_add_int_value(builder, color.red);
        json_builder_set_member_name(builder, "green");
        json_builder_add_int_value(builder, color.green);
        json_builder_set_member_name(builder, "blue");
        json_builder_add_int_value(builder, color.blue);
        json_builder_set_member_name(builder, "alpha");
        json_builder_add_int_value(builder, color.alpha);
        json_builder_end_object(builder);
        json_builder_end_object (builder);
    }
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_osd_msg_handler_class_init (IpcamOsdMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_osd_msg_handler_finalize;

    parent_class->get_action = ipcam_osd_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_osd_msg_handler_put_action_impl;
}


