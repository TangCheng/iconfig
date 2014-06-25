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
ipcam_osd_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_array(builder);
    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        gboolean isshow;
        guint size;
        guint x, y;
        guint color;
        const gchar *name = json_array_get_string_element(req_array, i);
        if (ipcam_iconfig_get_osd(iconfig, name, &isshow, &size, &x, &y, &color))
        {
            json_builder_begin_object(builder);
            json_builder_set_member_name(builder, "name");
            json_builder_add_string_value(builder, name);
            json_builder_set_member_name(builder, "isshow");
            json_builder_add_boolean_value(builder, isshow);
            json_builder_set_member_name(builder, "size");
            json_builder_add_int_value(builder, size);
            json_builder_set_member_name(builder, "x");
            json_builder_add_int_value(builder, x);
            json_builder_set_member_name(builder, "y");
            json_builder_add_int_value(builder, y);
            json_builder_set_member_name(builder, "color");
            json_builder_add_int_value(builder, color);
            json_builder_end_object(builder);
        }
    }
    json_builder_end_array(builder);
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
    int i;

    req_arr = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name (builder, "changed_items");
    json_builder_begin_array (builder);
    for(i = 0; i < json_array_get_length(req_arr); i++)
    {
        const gchar *name;
        gboolean isshow;
        guint size, x, y, color;

        req_obj = json_array_get_object_element(req_arr, i);
        name = json_object_get_string_member(req_obj, "name");
        isshow = json_object_get_boolean_member(req_obj, "isshow");
        size = json_object_get_int_member(req_obj, "size");
        x = json_object_get_int_member(req_obj, "x");
        y = json_object_get_int_member(req_obj, "y");
        color = json_object_get_int_member(req_obj, "color");

        ipcam_iconfig_set_osd(iconfig, name, isshow, size, x, y, color);

        ipcam_iconfig_get_osd(iconfig, name, &isshow, &size, &x, &y, &color);
        json_builder_begin_object(builder);
        json_builder_set_member_name (builder, "name");
        json_builder_add_string_value (builder, name);
        json_builder_set_member_name (builder, "isshow");
        json_builder_add_int_value (builder, isshow);
        json_builder_set_member_name (builder, "size");
        json_builder_add_int_value (builder, size);
        json_builder_set_member_name (builder, "x");
        json_builder_add_int_value (builder, x);
        json_builder_set_member_name (builder, "y");
        json_builder_add_int_value (builder, y);
        json_builder_set_member_name (builder, "color");
        json_builder_add_int_value (builder, color);
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


