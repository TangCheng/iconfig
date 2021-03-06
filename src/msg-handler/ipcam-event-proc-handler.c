/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-event_proc-handler.c
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
#include "ipcam-event-proc-handler.h"
#include "iconfig.h"
#include "common.h"
#include "database/event_proc.h"

G_DEFINE_TYPE (IpcamEventProcMsgHandler, ipcam_event_proc_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_event_proc_msg_handler_init (IpcamEventProcMsgHandler *ipcam_event_proc_msg_handler)
{
}

static void
ipcam_event_proc_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_event_proc_msg_handler_parent_class)->finalize (object);
}

static gboolean
ipcam_event_proc_msg_handler_read_param(IpcamEventProcMsgHandler *handler, JsonBuilder *builder, const gchar *name)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    GVariant *value;

    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_PROC_TYPE, name, "record");
    if (value)
    {
        json_builder_set_member_name(builder, "record");
        json_builder_add_boolean_value(builder, g_variant_get_boolean(value));
        g_variant_unref(value);
    }

    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_PROC_TYPE, name, "sound");
    if (value)
    {
        json_builder_set_member_name(builder, "sound");
        json_builder_add_boolean_value(builder, g_variant_get_boolean(value));
        g_variant_unref(value);
    }

    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_PROC_TYPE, name, "output1");
    if (value)
    {
        json_builder_set_member_name(builder, "output1");
        json_builder_add_boolean_value(builder, g_variant_get_boolean(value));
        g_variant_unref(value);
    }
    
    return TRUE;
}

static gboolean
ipcam_event_proc_msg_handler_update_param(IpcamEventProcMsgHandler *handler, const gchar *name, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    GVariant *value = NULL;
    GList *members, *item;
    gboolean ret = FALSE;

    members = json_object_get_members(value_obj);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *sub_name = item->data;
        value = g_variant_new_boolean(json_object_get_boolean_member(value_obj, sub_name));

        if (value)
        {
            ret = ipcam_iconfig_update(iconfig, IPCAM_EVENT_PROC_TYPE, name, sub_name, value);
            g_variant_unref(value);
            value = NULL;
        }
    }
    g_list_free(members);
    
    return ret;
}

static gboolean
ipcam_event_proc_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);
    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        const gchar *name = json_array_get_string_element(req_array, i);
        if (g_str_equal(name, "input1") ||
            g_str_equal(name, "motion") ||
            g_str_equal(name, "cover"))
        {
            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            ipcam_event_proc_msg_handler_read_param(IPCAM_EVENT_PROC_MSG_HANDLER(handler), builder, name);
            json_builder_end_object(builder);
        }
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_event_proc_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonObject *req_obj;
    GList *members, *item;

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");
    members = json_object_get_members(req_obj);
    json_builder_begin_object(builder);
    json_builder_set_member_name (builder, "items");
    json_builder_begin_object (builder);
    for(item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *name = item->data;
        if (g_str_equal(name, "input1") ||
            g_str_equal(name, "motion") ||
            g_str_equal(name, "cover"))
        {
            JsonObject *value_obj = json_object_get_object_member(req_obj, name);
            ipcam_event_proc_msg_handler_update_param(IPCAM_EVENT_PROC_MSG_HANDLER(handler), name, value_obj);
            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            ipcam_event_proc_msg_handler_read_param(IPCAM_EVENT_PROC_MSG_HANDLER(handler), builder, name);
            json_builder_end_object(builder);
        }
    }
    g_list_free(members);
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_event_proc_msg_handler_class_init (IpcamEventProcMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_event_proc_msg_handler_finalize;

    parent_class->get_action = ipcam_event_proc_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_event_proc_msg_handler_put_action_impl;
}


