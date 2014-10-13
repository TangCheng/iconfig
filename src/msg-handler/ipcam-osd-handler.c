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
#include "database/osd.h"

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
    Color color;
    GVariant *value = NULL;
    const gchar *key[] = 
    {
        "isshow",
        "size",
        "left",
        "top",
        "color"
    };
    gint i = 0;

    gchar **osd_name = g_strsplit(name, ":", -1);
    json_builder_set_member_name(builder, osd_name[1]);
    g_strfreev(osd_name);
    json_builder_begin_object(builder);

    for (i = 0; i < ARRAY_SIZE(key); i++)
    {
        value = ipcam_iconfig_read(iconfig, IPCAM_OSD_TYPE, name, key[i]);
        if (value)
        {
            if (g_variant_is_of_type(value, G_VARIANT_TYPE_BOOLEAN))
            {
                json_builder_set_member_name(builder, key[i]);
                json_builder_add_boolean_value(builder, g_variant_get_boolean(value));
            }
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32))
            {
                if (g_str_equal(key[i], "color"))
                {
                    color.value = g_variant_get_uint32(value);
                    json_builder_set_member_name(builder, key[i]);
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
                }
                else
                {
                    json_builder_set_member_name(builder, key[i]);
                    json_builder_add_int_value(builder, g_variant_get_uint32(value));
                }
            }
            else
            {
                g_warn_if_reached();
            }
            g_variant_unref(value);
        }
    }
    json_builder_end_object(builder);

    return TRUE;
}

static gboolean
ipcam_osd_msg_handler_update_param(IpcamOsdMsgHandler *handler, const gchar *name, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    Color color;
    JsonObject *color_obj;
    GVariant *value = NULL;
    GList *members, *item;
    
    members = json_object_get_members(value_obj);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *sub_name = item->data;
        if (g_str_equal(sub_name, "isshow"))
        {
            value = g_variant_new_boolean(json_object_get_boolean_member(value_obj, sub_name));
        }
        else if (g_str_equal(sub_name, "color"))
        {
            color_obj = json_object_get_object_member(value_obj, sub_name);
            color.red = json_object_get_int_member(color_obj, "red");
            color.green = json_object_get_int_member(color_obj, "green");
            color.blue = json_object_get_int_member(color_obj, "blue");
            color.alpha = json_object_get_int_member(color_obj, "alpha");
            value = g_variant_new_uint32(color.value);
        }
        else
        {
            value = g_variant_new_uint32(json_object_get_int_member(value_obj, sub_name));
        }
        if (value)
        {
            ipcam_iconfig_update(iconfig, IPCAM_OSD_TYPE, name, sub_name, value);
            g_variant_unref(value);
        }
    }
    g_list_free(members);
        
    return TRUE;
}

static gboolean
ipcam_osd_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;
    gboolean in_object = FALSE;
    gchar first_char = '\0';

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
            if (in_object)
            {
                json_builder_end_object(builder);
                in_object = FALSE;
            }

            if (g_str_has_prefix(name, "master"))
            {
                json_builder_set_member_name(builder, "master");
            }
            else
            {
                json_builder_set_member_name(builder, "slave");
            }
            json_builder_begin_object(builder);
            in_object = TRUE;
        }
        ipcam_osd_msg_handler_read_param(IPCAM_OSD_MSG_HANDLER(handler), builder, name);
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_osd_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonObject *req_obj;
    GList *members, *item;
    gboolean in_object = FALSE;
    gchar first_char = '\0';

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");
    members = json_object_get_members(req_obj);

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);
    gchar *key = g_malloc0(32);
    for(item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *name = item->data;
        JsonObject *sub_obj = json_object_get_object_member(req_obj, name);
        GList *sub_members, *sub_item;
        sub_members = json_object_get_members(sub_obj);
        for (sub_item = g_list_first(sub_members); sub_item; sub_item = g_list_next(sub_item))
        {
            const gchar *sub_name = sub_item->data;
            JsonObject *value_obj = json_object_get_object_member(sub_obj, sub_name);
            g_snprintf(key, 32, "%s:%s", name, sub_name);
            ipcam_osd_msg_handler_update_param(IPCAM_OSD_MSG_HANDLER(handler), key, value_obj);
            if (first_char != name[0])
            {
                first_char = name[0];
                if (in_object)
                {
                    json_builder_end_object(builder);
                    in_object = FALSE;
                }

                json_builder_set_member_name(builder, name);
                json_builder_begin_object(builder);
                in_object = TRUE;
            }
            ipcam_osd_msg_handler_read_param(IPCAM_OSD_MSG_HANDLER(handler), builder, key);
        }
        g_list_free(sub_members);
        json_builder_end_object(builder);
        in_object = FALSE;
    }
    g_free(key);
    g_list_free(members);
    json_builder_end_object(builder);
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


