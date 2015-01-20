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

#include <stdlib.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <base_app.h>
#include <notice_message.h>
#include "ipcam-network-handler.h"
#include "iconfig.h"
#include "sysutils.h"
#include "common.h"
#include "database/network.h"
#include "database/network_static.h"
#include "database/network_pppoe.h"
#include "database/network_port.h"

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

static void
ipcam_network_msg_handler_read_method(IpcamNetworkMsgHandler *handler, JsonBuilder *builder)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    GVariant *value;

    value = ipcam_iconfig_read(iconfig, IPCAM_NETWORK_TYPE, "method", "value");
    if (value)
    {                
        json_builder_set_member_name(builder, "method");
        json_builder_add_string_value(builder, g_variant_get_string(value, NULL));
        g_variant_unref(value);
    }
}

static void
ipcam_network_msg_handler_read_address(IpcamNetworkMsgHandler *handler, JsonBuilder *builder)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    gchar *hwaddr = NULL, *ipaddr = NULL, *netmask = NULL, *gateway = NULL;
    char *dns[2] = { [0 ... (ARRAY_SIZE(dns) - 1)] = NULL };
    int nr_dns = 2;
    struct __key_val {
        gchar *key;
        gchar **pval;
    } kv[] = {
        { "hwaddr",  &hwaddr },
        { "ipaddr",  &ipaddr },
        { "netmask", &netmask },
        { "gateway", &gateway },
        { "dns1",    &dns[0] },
        { "dns2",    &dns[1] }
    };
    const gchar *netif = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "netif");

    if (sysutils_network_get_hwaddr(netif, &hwaddr) != 0)
    {
        perror("error get mac address: ");
    }
    if (sysutils_network_get_address(netif, &ipaddr, &netmask, NULL) != 0)
    {
        ipaddr = g_strdup(ipcam_iconfig_read(iconfig, IPCAM_NETWORK_STATIC_TYPE, "ipaddr", "value"));
        if (!ipaddr)
             perror("error get network address: ");
    }
    if (sysutils_network_get_gateway(netif, &gateway) != 0)
    {
        gateway = g_strdup(ipcam_iconfig_read(iconfig, IPCAM_NETWORK_STATIC_TYPE, "gateway", "value"));
        if (!gateway)
            perror("error get gateway: ");
    }
    if (sysutils_network_get_dns(netif, dns, &nr_dns) != 0)
    {
        perror("error get dns: ");
    }

    int i;
    for (i = 0; i < ARRAY_SIZE(kv); i++)
    {
        if (*kv[i].pval)
        {
            json_builder_set_member_name(builder, kv[i].key);
            json_builder_add_string_value(builder, *kv[i].pval);

            g_free(*kv[i].pval);
        }
        else
        {
            json_builder_set_member_name(builder, kv[i].key);
            json_builder_add_string_value(builder, "");
        }
    }
}

static void
ipcam_network_msg_handler_read_pppoe(IpcamNetworkMsgHandler *handler, JsonBuilder *builder)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    static gchar *subitems[] = { "username", "password" };
    gint i;
    GVariant *value = NULL;

    for (i = 0; i < ARRAY_SIZE(subitems); i++)
    {
        value = ipcam_iconfig_read(iconfig, IPCAM_NETWORK_PPPOE_TYPE, subitems[i], "value");
        if (value)
        {
            json_builder_set_member_name(builder, subitems[i]);
            json_builder_add_string_value(builder, g_variant_get_string(value, NULL));
            g_variant_unref(value);
        }
    }
}

static void
ipcam_network_msg_handler_read_port(IpcamNetworkMsgHandler *handler, JsonBuilder *builder)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    static gchar *subitems[] = { "http", "ftp", "rtsp" };
    gint i;
    GVariant *value = NULL;

    for (i = 0; i < ARRAY_SIZE(subitems); i++)
    {
        value = ipcam_iconfig_read(iconfig, IPCAM_NETWORK_PORT_TYPE, subitems[i], "value");

        if (value)
        {
            json_builder_set_member_name(builder, subitems[i]);
            json_builder_add_int_value(builder, g_variant_get_uint32(value));
            g_variant_unref(value);
        }
    }
}

static gboolean
ipcam_network_msg_handler_read_param(IpcamNetworkMsgHandler *handler, JsonBuilder *builder, const gchar *name)
{
    if (g_str_equal(name, "method"))
    {
        ipcam_network_msg_handler_read_method(handler, builder);
    }
    else if (g_str_equal(name, "hostname"))
    {
        const char *hostname = NULL;
        if (sysutils_network_get_hostname(&hostname) == 0)
        {
            json_builder_set_member_name(builder, "hostname");
            json_builder_add_string_value(builder, hostname);

            free(hostname);
        }
    }
    else if (g_str_equal(name, "address"))
    {
        json_builder_set_member_name(builder, "address");
        json_builder_begin_object(builder);
        ipcam_network_msg_handler_read_address(handler, builder);
        json_builder_end_object(builder);
    }
    else if (g_str_equal(name, "pppoe"))
    {
        json_builder_set_member_name(builder, name);
        json_builder_begin_object(builder);
        ipcam_network_msg_handler_read_pppoe(handler, builder);
        json_builder_end_object(builder);
    }
    else if (g_strcmp0(name, "port") == 0)
    {
        json_builder_set_member_name(builder, name);
        json_builder_begin_object(builder);
        ipcam_network_msg_handler_read_port(handler, builder);
        json_builder_end_object(builder);
    }

    return TRUE;
}

static void
ipcam_network_msg_handler_update_address(IpcamNetworkMsgHandler *handler, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    
    const gchar *ipaddr = NULL;
    const gchar *netmask = NULL;
    const gchar *gateway = NULL;
    const gchar *dns1 = NULL;
    const gchar *dns2 = NULL;

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
    GVariant *value = NULL;
    for (i = 0; i < ARRAY_SIZE(kv); i++)
    {
        if (json_object_has_member(value_obj, kv[i].key))
        {
            *kv[i].pval = json_object_get_string_member(value_obj, kv[i].key);
            value = g_variant_new_string(*kv[i].pval);
            ipcam_iconfig_update(iconfig, IPCAM_NETWORK_STATIC_TYPE, kv[i].key, "value", value);
            g_variant_unref(value);
        }
    }

    /* Send SZYC notifications */
    if (ipaddr) {
        IpcamMessage *notice_msg;
        JsonBuilder *builder;
        JsonNode *notice_body;
        guint32 carriage_num, position_num;

        if (sscanf(ipaddr, "%*u.%*u.%u.%u", &carriage_num, &position_num) == 2) {
            carriage_num = carriage_num > 100 ? carriage_num - 100 : 0;
            position_num = position_num > 70 ? position_num - 70 : 0;
            char buf[16];
            builder = json_builder_new();
            json_builder_begin_object(builder);
            json_builder_set_member_name(builder, "items");
            json_builder_begin_object(builder);
            json_builder_set_member_name(builder, "carriage_num");
            snprintf(buf, sizeof(buf), "%u", carriage_num);
            json_builder_add_string_value(builder, buf);
            json_builder_set_member_name(builder, "position_num");
            snprintf(buf, sizeof(buf), "%u", position_num);
            json_builder_add_string_value(builder, buf);
            json_builder_end_object(builder);
            json_builder_end_object(builder);

            notice_body = json_builder_get_root(builder);
            g_object_unref(builder);

            notice_msg = g_object_new(IPCAM_NOTICE_MESSAGE_TYPE,
                                      "event", "set_szyc",
                                      "body", notice_body, NULL);
            ipcam_base_app_broadcast_notice_message(IPCAM_BASE_APP(iconfig),
                                                    notice_msg, "iconfig_token");

            g_object_unref(notice_msg);
        }
    }

#if 0
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
#endif
}

static void
ipcam_network_msg_handler_update_pppoe(IpcamNetworkMsgHandler *handler, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    
    GList *items = json_object_get_members(value_obj);
    GList *l;
    for (l = g_list_first(items); l; l = g_list_next(l))
    {
        gchar *name = l->data;
        gchar *value = (gchar *)json_object_get_string_member(value_obj, name);
        GVariant *val = NULL;

        val = g_variant_new_string(value);
        ipcam_iconfig_update(iconfig, IPCAM_NETWORK_PPPOE_TYPE, name, "value", val);
        g_variant_unref(val);
    }
    g_list_free(items);
}

static void
ipcam_network_msg_handler_update_port(IpcamNetworkMsgHandler *handler, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    GList *items = json_object_get_members(value_obj);
    GList *l;
    for (l = g_list_first(items); l; l = g_list_next(l))
    {
        gchar *name = l->data;
        gint value = json_object_get_int_member(value_obj, name);
        GVariant *val = NULL;

        val = g_variant_new_uint32(value);
        ipcam_iconfig_update (iconfig, IPCAM_NETWORK_PORT_TYPE, name, "value", val);
        g_variant_unref(val);
    }
    g_list_free(items);
}

static gboolean
ipcam_network_msg_handler_update_param(IpcamNetworkMsgHandler *handler, const gchar *name, JsonObject *value_obj)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    
    if (g_str_equal(name, "method"))
    {
        const gchar *method = json_object_get_string_member(value_obj, "method");
        GVariant *value = g_variant_new_string(method);
        ipcam_iconfig_update(iconfig, IPCAM_NETWORK_TYPE, "method", "value", value);
        g_variant_unref(value);
    }
    else if (g_str_equal(name, "hostname")) {
        const char *hostname = json_object_get_string_member(value_obj, "hostname");
        sysutils_network_set_hostname(hostname);
    }
    else if (g_str_equal(name, "address"))
    {
        ipcam_network_msg_handler_update_address(handler, value_obj);
    }
    else if (g_str_equal(name, "pppoe"))
    {
        ipcam_network_msg_handler_update_pppoe(handler, value_obj);
    }
    else if (g_str_equal(name, "port"))
    {
        ipcam_network_msg_handler_update_port(handler, value_obj);
    }
    else
    {
        g_warn_if_reached();
    }
    return TRUE;
}

static gboolean
ipcam_network_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
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
        ipcam_network_msg_handler_read_param(IPCAM_NETWORK_MSG_HANDLER(handler), builder, name);
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
        JsonObject *value_obj = NULL;
        if (json_node_get_node_type(json_object_get_member(req_obj, name))
            == JSON_NODE_OBJECT)
        {
            value_obj = json_object_get_object_member(req_obj, name);
        }
        else
        {
            value_obj = req_obj;
        }
        ipcam_network_msg_handler_update_param(IPCAM_NETWORK_MSG_HANDLER(handler), name, value_obj);
        ipcam_network_msg_handler_read_param(IPCAM_NETWORK_MSG_HANDLER(handler), builder, name);
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);
    g_list_free(members);

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
