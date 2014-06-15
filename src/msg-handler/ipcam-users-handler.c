/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-users-handler.c
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
#include "ipcam-users-handler.h"
#include "iconfig.h"

G_DEFINE_TYPE (IpcamUsersMsgHandler, ipcam_users_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_users_msg_handler_init (IpcamUsersMsgHandler *ipcam_users_msg_handler)
{
}

static void
ipcam_users_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_users_msg_handler_parent_class)->finalize (object);
}

static gboolean
ipcam_users_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;
    gboolean need_password = FALSE, need_privilege = FALSE;

    req_array = json_object_get_array_member (json_node_get_object(request), "items");
    for (i = 0; i < json_array_get_length (req_array); i++) {
        gchar *item = json_array_get_string_element (req_array, i);
        if (g_strcmp0(item, "password") == 0)
            need_password = TRUE;
        else if (g_strcmp0(item, "privilege") == 0)
            need_privilege = TRUE;
    }

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_array(builder);

    GList *users = ipcam_iconfig_get_users(iconfig);
    GList *u;
    for (u = g_list_first(users); u; u = g_list_next(u))
    {
        gchar *username = u->data;
        json_builder_begin_object(builder);
        json_builder_set_member_name(builder, "username");
        json_builder_add_string_value(builder, username);
        if (need_password)
        {
            gchar *passwd = ipcam_iconfig_get_user_password(iconfig, username);
            json_builder_set_member_name(builder, "password");
            json_builder_add_string_value(builder, passwd);
            g_free(passwd);
        }
        if (need_privilege)
        {
            gint privilege = ipcam_iconfig_get_user_privilege(iconfig, username);
            json_builder_set_member_name(builder, "privilege");
            json_builder_add_int_value(builder, privilege);
        }
        json_builder_end_object(builder);
    }
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);
    g_list_free_full(users, g_free);

    return TRUE;
}

static gboolean
ipcam_users_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    json_builder_begin_object(builder);

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        JsonObject *user_obj = json_array_get_object_element(req_array, i);
        gchar *username = json_object_get_string_member(user_obj, "username");
        gchar *password = json_object_get_string_member(user_obj, "password");
        gint privilege = json_object_get_int_member(user_obj, "privilege");

        ipcam_iconfig_set_user_password (iconfig, username, password);
        ipcam_iconfig_set_user_privilege (iconfig, username, privilege);
    }
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_users_msg_handler_post_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    json_builder_begin_object(builder);

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        JsonObject *user_obj = json_array_get_object_element(req_array, i);
        gchar *username = json_object_get_string_member(user_obj, "username");
        gchar *password = json_object_get_string_member(user_obj, "password");
        gint privilege = json_object_get_int_member(user_obj, "privilege");

        ipcam_iconfig_set_user_password (iconfig, username, password);
        ipcam_iconfig_set_user_privilege (iconfig, username, privilege);
    }
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_users_msg_handler_delete_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    json_builder_begin_object(builder);

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    for (i = 0; i < json_array_get_length (req_array); i++)
    {
        JsonObject *item_obj = json_array_get_object_element (req_array, i);
        if (json_object_has_member(item_obj, "username"))
        {
            const gchar *username = json_object_get_string_member (item_obj, "username");
            ipcam_iconfig_del_user(iconfig, username);
        }
    }
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_users_msg_handler_class_init (IpcamUsersMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_users_msg_handler_finalize;

    parent_class->get_action = ipcam_users_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_users_msg_handler_put_action_impl;
    parent_class->post_action = ipcam_users_msg_handler_post_action_impl;
    parent_class->delete_action = ipcam_users_msg_handler_delete_action_impl;
}


