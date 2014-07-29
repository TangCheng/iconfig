/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-video-handler.c
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
#include "ipcam-video-handler.h"
#include "iconfig.h"

G_DEFINE_TYPE (IpcamVideoMsgHandler, ipcam_video_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_video_msg_handler_init (IpcamVideoMsgHandler *ipcam_video_msg_handler)
{
}

static void
ipcam_video_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_video_msg_handler_parent_class)->finalize (object);
}

#define ARRAY_SIZE(x)       (sizeof(x) / sizeof(x[0]))

static gboolean
ipcam_video_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);
    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        const gchar *profile = json_array_get_string_element(req_array, i);
        gint intval;
        gchar *strval;
        int i;
        static struct {
            const gchar *name;
            gboolean is_strval;
        } kv[] = {
            { "flip", 0 },
            { "quanlity", 0 },
            { "frame_rate", 0 },
            { "bit_rate", 0 },
            { "bit_rate_value", 0 },
            { "resolution", 0 },
            { "stream_path", 1 }
        };

        json_builder_set_member_name(builder, profile);
        json_builder_begin_object(builder);
        for (i = 0; i < ARRAY_SIZE(kv); i++) {
            json_builder_set_member_name(builder, kv[i].name);
            if (kv[i].is_strval) {
                strval = ipcam_iconfig_get_video_string(iconfig, profile, kv[i].name);
                json_builder_add_string_value(builder, strval);
                g_free(strval);
            }
            else {
                intval = ipcam_iconfig_get_video_int(iconfig, profile, kv[i].name);
                json_builder_add_int_value(builder, intval);
            }
        }
        json_builder_end_object(builder);
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_video_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    JsonObject *req_obj;
    GList *members, *item;

    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);

    members = json_object_get_members(req_obj);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        gchar *profile = item->data;
        JsonObject *p_obj;
        GList *p_members, *p_item;

        p_obj = json_object_get_object_member(req_obj, profile);
        p_members = json_object_get_members(p_obj);

        json_builder_set_member_name(builder, profile);
        json_builder_begin_object(builder);
        for (p_item = g_list_first(p_members); p_item; p_item = g_list_next(p_item))
        {
            const gchar *name = p_item->data;
            JsonNode *node = json_object_get_member(p_obj, name);

            json_builder_set_member_name(builder, name);
            if (g_type_is_a(json_node_get_value_type(node), G_TYPE_STRING)) {
                const gchar *strval = json_node_get_string(node);
                ipcam_iconfig_set_video_string(iconfig, profile, name, strval);
                json_builder_add_string_value(builder, strval);
            }
            else {
                gint intval = json_node_get_int(node);
                ipcam_iconfig_set_video_int(iconfig, profile, name, intval);
                intval = ipcam_iconfig_get_video_int(iconfig, profile, name);
                json_builder_add_int_value(builder, (gint64)intval);
            }
        }
        g_list_free(p_members);
        json_builder_end_object(builder);
    }
    g_list_free(members);

    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);
    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_video_msg_handler_class_init (IpcamVideoMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_video_msg_handler_finalize;

    parent_class->get_action = ipcam_video_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_video_msg_handler_put_action_impl;
}


