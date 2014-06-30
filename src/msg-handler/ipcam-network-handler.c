/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-network-handler.c
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
#include <base_app.h>
#include "ipcam-network-handler.h"
#include "iconfig.h"
#include "sysutils.h"

G_DEFINE_TYPE (IpcamNetworkMsgHandler, ipcam_network_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_network_msg_handler_init (IpcamNetworkMsgHandler *ipcam_network_msg_handler)
{
}

static void
ipcam_network_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_network_msg_handler_parent_class)->finalize (object);
}

#define ARRAY_SIZE(x)       sizeof(x)/sizeof(x[0])

static gboolean
ipcam_network_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonArray *req_array;
    int idx;

    req_array = json_object_get_array_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);

    for (idx = 0; idx < json_array_get_length(req_array); idx++)
    {
        const gchar *name = json_array_get_string_element(req_array, idx);

        if (g_strcmp0(name, "autoconf") == 0) {
            gint value = ipcam_iconfig_get_network(iconfig, "method");

            json_builder_set_member_name(builder, name);
            json_builder_add_int_value(builder, value);
        }
        else if (g_strcmp0(name, "address") == 0)
        {
            gchar *ipaddr = NULL, *netmask = NULL, *gateway = NULL;
            json_builder_set_member_name(builder, "ipaddr");
            json_builder_begin_object(builder);
            struct __key_val {
                gchar *key;
                gchar **pval;
            } kv[] = {
                { "ipaddr", &ipaddr },
                { "netmask", &netmask },
                { "gateway", &gateway }
            };
            const gchar *netif = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "netif");

            if (sysutils_network_get_address(netif, &ipaddr, &netmask, NULL) != 0)
                perror("error get network address: ");
            if (sysutils_network_get_gateway(netif, &gateway) != 0)
                perror("error get gateway: ");

            int i;
            for (i = 0; i < ARRAY_SIZE(kv); i++)
            {
                if (*kv[i].pval)
                {
                    json_builder_set_member_name(builder, kv[i].key);
                    json_builder_add_string_value(builder, *kv[i].pval);

                    g_free(*kv[i].pval);
                }
            }
            json_builder_end_object(builder);
        }
        else if (g_strcmp0(name, "static-address") == 0)
        {
            static gchar *keys[] = { "ipaddr", "netmask", "gateway", "dns1", "dns2" };
            int i;

            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            for (i = 0; i < ARRAY_SIZE(keys); i++)
            {
                gchar *value = ipcam_iconfig_get_network_static(iconfig, keys[i]);

                json_builder_set_member_name(builder, keys[i]);
                json_builder_add_string_value(builder, value);

                g_free(value);
            }
            json_builder_end_object(builder);
        }
        else if (g_strcmp0(name, "pppoe") == 0)
        {
            static gchar *subitems[] = { "username", "password" };
            int i;

            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            for (i = 0; i < ARRAY_SIZE(subitems); i++)
            {
                gchar *value = ipcam_iconfig_get_network_pppoe(iconfig, subitems[i]);

                json_builder_set_member_name(builder, subitems[i]);
                json_builder_add_string_value(builder, value);

                g_free(value);
            }
            json_builder_end_object(builder);
        }
        else if (g_strcmp0(name, "server_port") == 0)
        {
            static gchar *subitems[] = { "http", "rtsp" };
            int i;

            json_builder_set_member_name(builder, name);
            json_builder_begin_object(builder);
            for (i = 0; i < ARRAY_SIZE(subitems); i++)
            {
                gint value = ipcam_iconfig_get_network_port(iconfig, subitems[i]);

                json_builder_set_member_name(builder, subitems[i]);
                json_builder_add_int_value(builder, value);
            }
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
ipcam_network_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    JsonBuilder *builder = json_builder_new();
    JsonObject *req_obj;

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "changed_items");
    json_builder_begin_object(builder);
    if (json_object_has_member(req_obj, "autoconf"))
    {
        gint value = json_object_get_int_member(req_obj, "autoconf");

        ipcam_iconfig_set_network(iconfig, "method", value);
        value = ipcam_iconfig_get_network(iconfig, "method");
        json_builder_set_member_name(builder, "autoconf");
        json_builder_add_int_value(builder, value);
    }
    if (json_object_has_member(req_obj, "address"))
    {
        const gchar *ipaddr = NULL;
        const gchar *netmask = NULL;
        const gchar *gateway = NULL;
        const gchar *dns1 = NULL;
        const gchar *dns2 = NULL;
        JsonObject *addr_obj = json_object_get_object_member(req_obj, "address");

        json_builder_set_member_name(builder, "address");
        json_builder_begin_object(builder);

        struct __key_val
        {
            const char *key;
            const gchar **pval;
        } kv[] = {
            { "ipaddr", &ipaddr },
            { "netmask", &netmask },
            { "gateway", &gateway },
            { "dns1", &dns1 },
            { "dns2", &dns2 }
        };

        int i;
        for (i = 0; i < ARRAY_SIZE(kv); i++)
        {
            if (json_object_has_member(addr_obj, kv[i].key))
            {
                *kv[i].pval = json_object_get_string_member(addr_obj, kv[i].key);
                ipcam_iconfig_set_network_static(iconfig, kv[i].key, (gchar *)*kv[i].pval);
                json_builder_set_member_name(builder, kv[i].key);
                json_builder_add_string_value(builder, *kv[i].pval);
            }
        }

        json_builder_end_object(builder);

        /* Apply the change of network configuration. */
        const gchar *netif = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "netif");
        if (netif)
        {
            if (ipaddr || netmask) {
                if (sysutils_network_set_address(netif, (gchar *)ipaddr, (gchar *)netmask, NULL) != 0)
                    perror("error set network address: ");
            }

            if (gateway) {
                if (sysutils_network_set_gateway(netif, gateway) != 0)
                    perror("error set gateway: ");
            }

            if (dns1 || dns2)
            {
                const char *dns[2] = { dns1, dns2 };
                sysutils_network_set_dns(netif, dns, 2);
            }
        }
    }
    if (json_object_has_member(req_obj, "pppoe"))
    {
        JsonObject *addr_obj = json_object_get_object_member(req_obj, "pppoe");
        GList *items = json_object_get_members(addr_obj);
        GList *l;
        json_builder_set_member_name(builder, "pppoe");
        json_builder_begin_object(builder);
        for (l = g_list_first(items); l; l = g_list_next(l))
        {
            gchar *name = l->data;
            gchar *value = (gchar *)json_object_get_string_member(addr_obj, name);

            ipcam_iconfig_set_network_pppoe(iconfig, name, value);
            value = ipcam_iconfig_get_network_pppoe(iconfig, name);
            json_builder_set_member_name(builder, name);
            json_builder_add_string_value(builder, value);

            g_free(value);
        }
        json_builder_end_object(builder);
        g_list_free(items);
    }
    if (json_object_has_member(req_obj, "server_port"))
    {
        JsonObject *addr_obj = json_object_get_object_member(req_obj, "server_port");
        GList *items = json_object_get_members(addr_obj);
        GList *l;

        json_builder_set_member_name(builder, "server_port");
        json_builder_begin_object(builder);
        for (l = g_list_first(items); l; l = g_list_next(l))
        {
            gchar *name = l->data;
            gint value = json_object_get_int_member(addr_obj, name);

            ipcam_iconfig_set_network_port (iconfig, name, value);
            value = ipcam_iconfig_get_network_port(iconfig, name);
            json_builder_set_member_name(builder, name);
            json_builder_add_int_value(builder, value);
        }
        json_builder_end_object(builder);
        g_list_free(items);
    }

    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_network_msg_handler_class_init (IpcamNetworkMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_network_msg_handler_finalize;

    parent_class->get_action = ipcam_network_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_network_msg_handler_put_action_impl;
}


