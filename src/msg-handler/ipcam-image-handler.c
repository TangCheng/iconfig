/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-image-handler.c
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
#include "ipcam-image-handler.h"
#include "iconfig.h"

G_DEFINE_TYPE (IpcamImageMsgHandler, ipcam_image_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_image_msg_handler_init (IpcamImageMsgHandler *ipcam_image_msg_handler)
{
}

static void
ipcam_image_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_image_msg_handler_parent_class)->finalize (object);
}

static gboolean
ipcam_image_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
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
        const gchar *name = json_array_get_string_element(req_array, i);
        //GVariant *value = ipcam_iconfig_get_image(iconfig, name);
        guint *data = g_new(guint, 1);
        *data = 128;
        GBytes *value = g_bytes_new(&data, sizeof(guint));
        ipcam_iconfig_set_image(iconfig, name, value);

        json_builder_set_member_name(builder, name);
        if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING))
        {
            json_builder_add_string_value(builder, g_variant_get_string(value, NULL));
        }
        else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32))
        {
            json_builder_add_int_value(builder, g_variant_get_uint32(value));
        }
        else
        {
            g_warning("NOT excepted data type of value: %s\n", g_variant_get_type_string(value));
        }
        
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_image_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

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
        GVariant *value = NULL;
        JsonNode *node = json_object_get_member(req_obj, name);
        
        if (g_type_is_a(json_node_get_value_type(node), G_TYPE_STRING))
        {
            value = g_variant_new_string(json_node_get_string(node));
        }
        else
        {
            value = g_variant_new_uint32(json_node_get_int(node));
        }
        ipcam_iconfig_set_image(iconfig, name, value);

        json_builder_set_member_name(builder, name);
        if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING))
        {
            json_builder_add_string_value(builder, g_variant_get_string(value, NULL));
        }
        else
        {
            json_builder_add_int_value(builder, g_variant_get_uint32(value));
        }

        g_object_unref(value);
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    g_list_free(members);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_image_msg_handler_class_init (IpcamImageMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_image_msg_handler_finalize;

    parent_class->get_action = ipcam_image_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_image_msg_handler_put_action_impl;
}

