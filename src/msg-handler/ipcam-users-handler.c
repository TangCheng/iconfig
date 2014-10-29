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
ipcam_users_msg_handler_read_param(IpcamUsersMsgHandler *handler,
                                   JsonBuilder *builder,
                                   const gchar *username,
                                   gboolean read_password,
                                   gboolean read_role)
{
    g_return_val_if_fail(username, FALSE);
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    json_builder_set_member_name(builder, "username");
    json_builder_add_string_value(builder, username);
    if (read_password)
    {
        gchar *passwd = ipcam_iconfig_get_user_password(iconfig, username);
        json_builder_set_member_name(builder, "password");
        json_builder_add_string_value(builder, passwd);
        g_free(passwd);
    }
    if (read_role)
    {
        gchar *role = ipcam_iconfig_get_user_role(iconfig, username);
        json_builder_set_member_name(builder, "role");
        json_builder_add_string_value(builder, role);
        g_free(role);
    }

    return TRUE;
}

static gboolean
ipcam_users_msg_handler_update_param(IpcamUsersMsgHandler *handler,
                                     const gchar *username,
                                     const gchar *param_name,
                                     const gchar *value)
{
    g_return_val_if_fail(username, FALSE);
    g_return_val_if_fail(param_name, FALSE);
    g_return_val_if_fail(value, FALSE);
    
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    if (g_str_equal(param_name, "password"))
    {
        ipcam_iconfig_set_user_password (iconfig, username, (gchar *)value);
    }
    else if (g_str_equal(param_name, "role"))
    {
        if (g_str_equal(value, "administrator") ||
            g_str_equal(value, "operator") ||
            g_str_equal(value, "user"))
        {
            ipcam_iconfig_set_user_role (iconfig, username, (gchar *)value);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        g_warn_if_reached();
    }

    return TRUE;
}

static gboolean
ipcam_users_msg_handler_create_param(IpcamUsersMsgHandler *handler,
                                     const gchar *username,
                                     const gchar *password,
                                     const gchar *role)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    return ipcam_iconfig_add_user(iconfig, username, password, role);
}

static gboolean
ipcam_users_msg_handler_delete_param(IpcamUsersMsgHandler *handler,
                                     const gchar *username)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    
    return ipcam_iconfig_del_user(iconfig, username);
}

static gboolean
ipcam_users_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;
    gboolean need_password = FALSE, need_role = FALSE;

    if (request)
    {   
        req_array = json_object_get_array_member (json_node_get_object(request), "items");
        for (i = 0; i < json_array_get_length (req_array); i++)
        {
            const gchar *item = json_array_get_string_element (req_array, i);
            if (g_str_equal(item, "password"))
            {
                need_password = TRUE;
            }
            else if (g_str_equal(item, "role"))
            {
                need_role = TRUE;
            }
            else
            {
                g_warn_if_reached();
            }
        }
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
        ipcam_users_msg_handler_read_param(IPCAM_USERS_MSG_HANDLER(handler),
                                           builder, (const gchar *)username,
                                           need_password, need_role);
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
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_array(builder);
    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        JsonObject *user_obj = json_array_get_object_element(req_array, i);
        const gchar *username = json_object_get_string_member(user_obj, "username");
        const gchar *password = json_object_get_string_member(user_obj, "password");
        const gchar *role = json_object_get_string_member(user_obj, "role");

        if (password)
        {
            ipcam_users_msg_handler_update_param(IPCAM_USERS_MSG_HANDLER(handler),
                                                 username, "password", password);
        }
        if (role)
        {
            ipcam_users_msg_handler_update_param(IPCAM_USERS_MSG_HANDLER(handler),
                                                 username, "role", role);
        }

        json_builder_begin_object(builder);
        ipcam_users_msg_handler_read_param(IPCAM_USERS_MSG_HANDLER(handler), builder,
                                           username, password != NULL, role != NULL);
        json_builder_end_object(builder);
    }
    json_builder_end_array(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static gboolean
ipcam_users_msg_handler_post_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_array(builder);
    for (i = 0; i < json_array_get_length(req_array); i++)
    {
        JsonObject *user_obj = json_array_get_object_element(req_array, i);
        const gchar *username = json_object_get_string_member(user_obj, "username");
        const gchar *password = json_object_get_string_member(user_obj, "password");
        const gchar *role = json_object_get_string_member(user_obj, "role");

        if (ipcam_users_msg_handler_create_param(IPCAM_USERS_MSG_HANDLER(handler),
                                                 username, password, role))
        {
            json_builder_begin_object(builder);
            ipcam_users_msg_handler_read_param(IPCAM_USERS_MSG_HANDLER(handler), builder,
                                               username, TRUE, TRUE);
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
ipcam_users_msg_handler_delete_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int i;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name (builder, "items");
    json_builder_begin_array (builder);
    for (i = 0; i < json_array_get_length (req_array); i++)
    {
        JsonObject *item_obj = json_array_get_object_element (req_array, i);
        if (json_object_has_member(item_obj, "username"))
        {
            const gchar *username = json_object_get_string_member (item_obj, "username");
            if (!g_str_equal(username, "admin"))
            {
                if (ipcam_users_msg_handler_delete_param(IPCAM_USERS_MSG_HANDLER(handler), username))
                {
                    json_builder_begin_object(builder);
                    json_builder_set_member_name(builder, "username");
                    json_builder_add_string_value (builder, username);
                    json_builder_end_object(builder);
                }
            }
        }
    }
    json_builder_end_array (builder);
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


