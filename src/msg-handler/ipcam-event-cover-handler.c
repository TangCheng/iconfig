/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-base-info-handler.c
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
#include "ipcam-event-cover-handler.h"
#include "iconfig.h"
#include "common.h"
#include "sysutils.h"
#include "database/event_cover.h"

G_DEFINE_TYPE (IpcamEventCoverMsgHandler, ipcam_event_cover_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_event_cover_msg_handler_init (IpcamEventCoverMsgHandler *ipcam_event_cover_msg_handler)
{
}

static void
ipcam_event_cover_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_event_cover_msg_handler_parent_class)->finalize (object);
}

static gboolean
ipcam_event_cover_msg_handler_read_param(IpcamEventCoverMsgHandler *handler, JsonBuilder *builder, const gchar *name)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    gboolean ret = FALSE;
    GVariant *value = NULL;

    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_COVER_TYPE, name, "enable");
    if (value)
    {
        json_builder_set_member_name(builder, "enable");
        json_builder_add_boolean_value(builder, g_variant_get_boolean(value));
        g_variant_unref(value);
    }

    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_COVER_TYPE, name, "sensitivity");
    if (value)
    {
        json_builder_set_member_name(builder, "sensitivity");
        json_builder_add_int_value(builder, g_variant_get_uint32(value));
        g_variant_unref(value);
    }

    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_COVER_TYPE, name, "rect");
    if (value)
    {
        Rect *rect;
        if (IS_64BIT_MACHINE)
        {
            rect = GSIZE_TO_POINTER(g_variant_get_uint64(value));
        }
        else
        {
            rect = GSIZE_TO_POINTER(g_variant_get_uint32(value));
        }
        json_builder_set_member_name(builder, "rect");
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "left");
        json_builder_add_int_value(builder, rect->x);
        json_builder_set_member_name(builder, "top");
        json_builder_add_int_value(builder, rect->y);
        json_builder_set_member_name(builder, "width");
        json_builder_add_int_value(builder, rect->width);
        json_builder_set_member_name(builder, "height");
        json_builder_add_int_value(builder, rect->height);
        json_builder_end_object(builder);
        g_variant_unref(value);
        g_free(rect);
    }
    
    value = ipcam_iconfig_read(iconfig, IPCAM_EVENT_COVER_TYPE, name, "schedules");
    if (value)
    {
        Schedules *sche;
        if (IS_64BIT_MACHINE)
        {
            sche = GSIZE_TO_POINTER(g_variant_get_uint64(value));
        }
        else
        {
            sche = GSIZE_TO_POINTER(g_variant_get_uint32(value));
        }
        gint i = 0;
        json_builder_set_member_name(builder, "schedules");
        json_builder_begin_object(builder);
        if (sche)
        {            
            for (i = ENUM_MON; i < ENUM_WEEKDAY_LAST; i++)
            {
                if (sche->schedule[i])
                {
                    json_builder_set_member_name(builder, weekday_name[i]);
                    json_builder_add_string_value(builder, sche->schedule[i]);
                    g_free(sche->schedule[i]);
                }
            }
            g_free(sche);
        }
        json_builder_end_object(builder);

        g_variant_unref(value);
    }
    
    return ret;
}

static gboolean
ipcam_event_cover_msg_handler_update_param(IpcamEventCoverMsgHandler *handler, const gchar *name, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    GList *members, *item;
    Schedules *sche = g_new(Schedules, 1);
    Rect *rect = g_new(Rect, 1);
    GVariant *value = NULL;
    gboolean ret = FALSE;

    members = json_object_get_members(value_obj);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *sub_name = item->data;
        if (g_str_equal(sub_name, "enable"))
        {
            value = g_variant_new_boolean(json_object_get_boolean_member(value_obj, sub_name));
        }
        else if (g_str_equal(sub_name, "sensitivity"))
        {
            value = g_variant_new_uint32(json_object_get_int_member(value_obj, sub_name));
        }
        else if (g_str_equal(sub_name, "rect"))
        {
            JsonObject *rect_obj = json_object_get_object_member(value_obj, sub_name);
            rect->x = json_object_get_int_member(rect_obj, "left");
            rect->y = json_object_get_int_member(rect_obj, "top");
            rect->width = json_object_get_int_member(rect_obj, "width");
            rect->height = json_object_get_int_member(rect_obj, "height");
            if (IS_64BIT_MACHINE)
            {
                value = g_variant_new_uint64(GPOINTER_TO_SIZE(rect));
            }
            else
            {
                value = g_variant_new_uint32(GPOINTER_TO_SIZE(rect));
            }
        }
        else if (g_str_equal(sub_name, "schedules"))
        {
            gint i = 0;
            JsonObject *sub_obj = json_object_get_object_member(value_obj, sub_name);
            for (i = ENUM_MON; i < ENUM_WEEKDAY_LAST; i++)
            {
                if (json_object_has_member(sub_obj, weekday_name[i]))
                {
                    sche->schedule[i] = (gchar *)json_object_get_string_member(sub_obj, weekday_name[i]);
                }
                else
                {
                    sche->schedule[i] = g_malloc0(1);
                }
            }
            if (IS_64BIT_MACHINE)
            {
                value = g_variant_new_uint64(GPOINTER_TO_SIZE(sche));
            }
            else
            {
                value = g_variant_new_uint32(GPOINTER_TO_SIZE(sche));
            }
        }
        else
        {
            g_warn_if_reached();
        }

        if (value)
        {
            ret = ipcam_iconfig_update(iconfig, IPCAM_EVENT_COVER_TYPE, name, sub_name, value);
            g_variant_unref(value);
            value = NULL;
        }
    }
    g_free(rect);
    g_free(sche);
    g_list_free(members);

    return ret;
}

static gboolean
ipcam_event_cover_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
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
        if (g_str_equal(name, "region1") ||
            g_str_equal(name, "region2"))
        {
            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            ipcam_event_cover_msg_handler_read_param(IPCAM_EVENT_COVER_MSG_HANDLER(handler), builder, name);
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
ipcam_event_cover_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonObject *req_obj;
    GList *members, *item;

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");
    members = json_object_get_members(req_obj);

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *name = item->data;
        if (g_str_equal(name, "region1") ||
            g_str_equal(name, "region2"))
        {
            JsonObject *value_obj = json_object_get_object_member(req_obj, name);
            ipcam_event_cover_msg_handler_update_param(IPCAM_EVENT_COVER_MSG_HANDLER(handler), name, value_obj);
            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            ipcam_event_cover_msg_handler_read_param(IPCAM_EVENT_COVER_MSG_HANDLER(handler), builder, name);
            json_builder_end_object(builder);
        }
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    g_list_free(members);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_event_cover_msg_handler_class_init (IpcamEventCoverMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_event_cover_msg_handler_finalize;

    parent_class->get_action = ipcam_event_cover_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_event_cover_msg_handler_put_action_impl;
}


