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
#include "common.h"
#include "database/video.h"

struct _IpcamVideoMsgHandlerPrivate
{
    GRegex *regex;
};

G_DEFINE_TYPE (IpcamVideoMsgHandler, ipcam_video_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_video_msg_handler_init (IpcamVideoMsgHandler *ipcam_video_msg_handler)
{
    IpcamVideoMsgHandlerPrivate *priv;

    priv = G_TYPE_INSTANCE_GET_PRIVATE(ipcam_video_msg_handler,
                                       IPCAM_TYPE_VIDEO_MSG_HANDLER,
                                       IpcamVideoMsgHandlerPrivate);
    ipcam_video_msg_handler->priv = priv;

    priv->regex = g_regex_new("<[^>]*>",
                              G_REGEX_EXTENDED | G_REGEX_OPTIMIZE,
                              0, NULL);
}

static void
ipcam_video_msg_handler_finalize (GObject *object)
{
    IpcamVideoMsgHandler *self = IPCAM_VIDEO_MSG_HANDLER(object);
    IpcamVideoMsgHandlerPrivate *priv = self->priv;

    g_regex_unref(priv->regex);

    G_OBJECT_CLASS (ipcam_video_msg_handler_parent_class)->finalize (object);
}

static void
ipcam_video_msg_handler_proc_variant(IpcamVideoMsgHandler *handler, JsonBuilder *builder, GVariant *value)
{
    if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING))
    {
        json_builder_add_string_value(builder, g_variant_get_string(value, NULL));
    }
    else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32))
    {
        json_builder_add_int_value(builder, g_variant_get_uint32(value));
    }
    else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BOOLEAN))
    {
        json_builder_add_boolean_value(builder, g_variant_get_boolean(value));
    }
    else
    {
        g_warn_if_reached();
    }
}

static gboolean
stream_path_replace_cb(const GMatchInfo *info,
                       GString          *res,
                       gpointer         data)
{
    gchar *match;
    GVariant *var;

    match = g_match_info_fetch(info, 0);
    var = g_hash_table_lookup((GHashTable*)data, match);
    if (var) {
        if (g_variant_is_of_type(var, G_VARIANT_TYPE_STRING)) {
            g_string_append(res, g_variant_get_string(var, NULL));
        }
        else if(g_variant_is_of_type(var, G_VARIANT_TYPE_UINT32)) {
            g_string_append_printf(res, "%d", g_variant_get_uint32(var));
        }
        else if(g_variant_is_of_type(var, G_VARIANT_TYPE_INT32)) {
            g_string_append_printf(res, "%d", g_variant_get_int32(var));
        }
    }
    g_free(match);

    return FALSE;
}

static gboolean
ipcam_video_msg_handler_read_param(IpcamVideoMsgHandler *handler, JsonBuilder *builder, const gchar *name)
{
    IpcamVideoMsgHandlerPrivate *priv = handler->priv;
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    if (g_str_equal(name, "main_profile") ||
        g_str_equal(name, "sub_profile"))
    {
        GVariant *frame_rate = NULL;
        GVariant *bit_rate = NULL;
        GVariant *bit_rate_value = NULL;
        GVariant *resolution = NULL;
        GVariant *stream_path = NULL;
        gchar key[64];
        gint i = 0;
        struct {
            const gchar *key;
            GVariant **value;
        } kv[] = {
            { "frame_rate", &frame_rate },
            { "bit_rate", &bit_rate },
            { "bit_rate_value", &bit_rate_value },
            { "resolution", &resolution },
            { "stream_path", &stream_path }
        };

        json_builder_set_member_name(builder, name);
        json_builder_begin_object(builder);

        for (i = 0; i < ARRAY_SIZE(kv); i++)
        {
            g_snprintf(key, 32, "%s:%s", name[0] == 'm' ? "master" : "slave", kv[i].key);
            *(kv[i].value) = ipcam_iconfig_read(iconfig, IPCAM_VIDEO_TYPE, key, "value");
        }

        if (stream_path) {
            const gchar *path = g_variant_get_string(stream_path, NULL);
            GVariant *name_var;
            guint32 channel = g_str_equal(name, "sub_profile") ? 1 : 0;
            GVariant *chn_var;
            GHashTable *h;
            gchar *real_path;

            h = g_hash_table_new(g_str_hash, g_str_equal);

            name_var = g_variant_new_string(name);
            g_hash_table_insert(h, "<name>", name_var);
            chn_var = g_variant_new_uint32(channel);
            g_hash_table_insert(h, "<channel>", chn_var);
            g_hash_table_insert(h, "<resolution>", resolution);
            g_hash_table_insert(h, "<framerate>", frame_rate);

            real_path = g_regex_replace_eval(priv->regex, path, -1, 0, 0, stream_path_replace_cb, h, NULL);

            if (real_path) {
                g_variant_unref(stream_path);
                stream_path = g_variant_new_string(real_path);
                g_free(real_path);
            }

            g_hash_table_destroy(h);
            g_variant_unref(name_var);
            g_variant_unref(chn_var);
        }

        for (i = 0; i < ARRAY_SIZE(kv); i++)
        {
            if (*(kv[i].value))
            {
                json_builder_set_member_name(builder, kv[i].key);
                ipcam_video_msg_handler_proc_variant(handler, builder, *(kv[i].value));
                g_variant_unref(*(kv[i].value));
            }
        }
        json_builder_end_object(builder);        
    }
    else
    {
        GVariant *value;

        value = ipcam_iconfig_read(iconfig, IPCAM_VIDEO_TYPE, name, "value");
        if (value)
        {
            json_builder_set_member_name(builder, name);
            ipcam_video_msg_handler_proc_variant(handler, builder, value);
            g_variant_unref(value);
            value = NULL;
        }
    }

    return TRUE;
}

static gboolean
ipcam_video_msg_handler_update_param(IpcamVideoMsgHandler *handler, const gchar *name, JsonNode *value_node)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    GVariant *value = NULL;
    GList *members = NULL;
    GList *item = NULL;
    JsonObject *stream_obj = NULL;

    switch (json_node_get_value_type(value_node))
    {
    case G_TYPE_BOOLEAN:
        value = g_variant_new_boolean(json_node_get_boolean(value_node));
        break;
    case G_TYPE_STRING:
        value = g_variant_new_string(json_node_get_string(value_node));
        break;
    default:
        if (g_type_is_a(json_node_get_value_type(value_node), G_TYPE_BOXED))
        {      
            stream_obj = json_node_get_object(value_node);
            break;
        }

        g_warn_if_reached();
        break;
    }

    if (value)
    {
        ipcam_iconfig_update(iconfig, IPCAM_VIDEO_TYPE, name, "value", value);
        g_variant_unref(value);
    }

    if (stream_obj)
    {
        members = json_object_get_members(stream_obj);
        for (item = g_list_first(members); item; item = g_list_next(item))
        {
            const gchar *value_name = item->data;
            JsonNode *node = json_object_get_member(stream_obj, value_name);
            gchar key[32];

            if (g_str_equal(value_name, "stream_path")) {
                g_warning("ignore setting %s property\n", value_name);
                continue;
            }

            g_snprintf(key, 32, "%s:%s", name[0] == 'm' ? "master" : "slave", value_name);
            if (g_type_is_a(json_node_get_value_type(node), G_TYPE_STRING))
            {
                const gchar *strval = json_node_get_string(node);
                value = g_variant_new_string(strval);
                ipcam_iconfig_update(iconfig, IPCAM_VIDEO_TYPE, key, "value", value);
                g_variant_unref(value);
            }
            else if (g_type_is_a(json_node_get_value_type(node), G_TYPE_INT64))
            {
                gint64 intval = json_node_get_int(node);
                value = g_variant_new_uint32((gint32)intval);
                ipcam_iconfig_update(iconfig, IPCAM_VIDEO_TYPE, key, "value", value);
                g_variant_unref(value);
            }
        }
        g_list_free(members);
    }
    
    return TRUE;
}

static gboolean
ipcam_video_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
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
        ipcam_video_msg_handler_read_param(IPCAM_VIDEO_MSG_HANDLER(handler), builder, name);
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
    JsonObject *req_obj;
    GList *members, *item;
    JsonBuilder *builder = json_builder_new();

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);

    members = json_object_get_members(req_obj);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        gchar *name = item->data;
        JsonNode *node;

        node = json_object_get_member(req_obj, name);
        ipcam_video_msg_handler_update_param(IPCAM_VIDEO_MSG_HANDLER(handler), name, node);
        ipcam_video_msg_handler_read_param(IPCAM_VIDEO_MSG_HANDLER(handler), builder, name);
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

    g_type_class_add_private(klass, sizeof(IpcamVideoMsgHandlerPrivate));

    object_class->finalize = ipcam_video_msg_handler_finalize;

    parent_class->get_action = ipcam_video_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_video_msg_handler_put_action_impl;
}


