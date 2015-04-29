/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-szyc-handler.c
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

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <base_app.h>
#include "ipcam-network-handler.h"
#include "ipcam-szyc-handler.h"
#include "iconfig.h"
#include "sysutils.h"
#include "database/szyc.h"
#include "database/network.h"
#include "database/network_static.h"
#include "database/network_pppoe.h"
#include "database/network_port.h"

G_DEFINE_TYPE (IpcamSzycMsgHandler, ipcam_szyc_msg_handler, IPCAM_TYPE_MESSAGE_HANDLER);

static void
ipcam_szyc_msg_handler_init (IpcamSzycMsgHandler *ipcam_szyc_msg_handler)
{
}

static void
ipcam_szyc_msg_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_szyc_msg_handler_parent_class)->finalize (object);
}

static gboolean
ipcam_szyc_msg_handler_read_param(IpcamSzycMsgHandler *handler, JsonBuilder *builder, const gchar *name)
{
    IpcamIConfig *iconfig;
    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
    gboolean ret = FALSE;

    if (g_str_equal(name, "carriage_num") || g_str_equal(name, "position_num")) {
        gchar *ipaddr = NULL, *netmask = NULL;
        char buf[16];
        guint32 carriage_num = 0;
        guint32 position_num = 0;
        const gchar *netif = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "netif");

        if (sysutils_network_get_address(netif, &ipaddr, &netmask, NULL) == 0) {
            if (sscanf(ipaddr, "%*u.%*u.%u.%u", &carriage_num, &position_num) == 2) {
                json_builder_set_member_name(builder, name);
                if (g_str_equal(name, "carriage_num")) {
                    carriage_num = carriage_num > 100 ? carriage_num - 100 : 0;
                    snprintf(buf, sizeof(buf), "%d", carriage_num);
                }
                else if(g_str_equal(name, "position_num")) {
                    position_num = position_num > 70 ? position_num - 70 : 0;
                    snprintf(buf, sizeof(buf), "%d", position_num);
                }
                json_builder_add_string_value(builder, buf);
            }
            g_free(ipaddr);
            g_free(netmask);
        }
    }
    else {
        GVariant *value = ipcam_iconfig_read(iconfig, IPCAM_SZYC_TYPE, name, "value");
        if (value)
        {
            json_builder_set_member_name(builder, name);
            json_builder_add_string_value(builder, g_variant_get_string(value, NULL));
            g_variant_unref(value);
            ret = TRUE;
        }
    }

    return ret;
}

static gboolean
ipcam_szyc_msg_handler_update_param(IpcamSzycMsgHandler *handler, const gchar *name, const gchar *value)
{
    IpcamIConfig *iconfig;

    if (g_str_equal(name, "train_num")) {
        g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);
        GVariant *val = g_variant_new_string(value);
        if (val)
        {
            ipcam_iconfig_update(iconfig, IPCAM_SZYC_TYPE, name, "value", val);
            g_variant_unref(val);
        }
    }

    return TRUE;
}

static gboolean
ipcam_szyc_msg_handler_get_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
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
        ipcam_szyc_msg_handler_read_param(IPCAM_SZYC_MSG_HANDLER(handler), builder, name);
    }
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static int
ipcam_szyc_msg_handler_get_ipaddr(IpcamMessageHandler *handler)
{
    IpcamIConfig *iconfig;
    const gchar *netif = "eth0";
    const gchar *saddr = NULL;
    GVariant *value;

    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    netif = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "netif");

    value = ipcam_iconfig_read(iconfig,
                               IPCAM_NETWORK_STATIC_TYPE,
                               "ipaddr",
                               "value");
    if (value) {
        saddr = g_variant_get_string(value, NULL);
        if (saddr) {
            return inet_addr(saddr);
        }
    }

    return 0;
}

static gboolean
ipcam_szyc_msg_handler_apply_network_change(IpcamMessageHandler *handler, JsonNode *request)
{
    IpcamIConfig *iconfig;
    JsonObject *req_obj;
    JsonNode *net_req, *net_resp;
    const gchar *carriage_str = NULL, *position_str = NULL;
    guint8 ipaddr3 = 110, ipaddr4 = 71;
    JsonBuilder *builder;

    g_object_get(G_OBJECT(handler), "app", &iconfig, NULL);

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");

    if (json_object_has_member (req_obj, "carriage_num")) {
        carriage_str = json_object_get_string_member (req_obj, "carriage_num");
        ipaddr3 = (strtoul(carriage_str, NULL, 0) & 0xff) + 100;
    }
    if (json_object_has_member (req_obj, "position_num")) {
        position_str = json_object_get_string_member (req_obj, "position_num");
        ipaddr4 = (strtoul(position_str, NULL, 0) & 0xff) + 70;
    }

    if (!carriage_str && !position_str)
        return FALSE;

    if (!carriage_str || !position_str) {
        int addr = ipcam_szyc_msg_handler_get_ipaddr(handler);
        if (addr) {
            if (!carriage_str)
                ipaddr3 = (addr >> 16) & 0xff;
            if (!position_str)
                ipaddr4 = (addr >> 24) & 0xff;
        }
        else {
            g_return_val_if_reached(FALSE);
        }
    }

    gchar new_ipaddr[32];
    snprintf(new_ipaddr, sizeof(new_ipaddr),
             "%d.%d.%d.%d", 192, 168, ipaddr3, ipaddr4);

    builder = json_builder_new();
    json_builder_begin_object(builder);
    json_builder_set_member_name (builder, "items");
    json_builder_begin_object (builder);
    json_builder_set_member_name (builder, "method");
    json_builder_add_string_value(builder, "static");
    json_builder_set_member_name (builder, "address");
    json_builder_begin_object (builder);
    json_builder_set_member_name (builder, "ipaddr");
    json_builder_add_string_value (builder, new_ipaddr);
    json_builder_set_member_name (builder, "netmask");
    json_builder_add_string_value (builder, "255.255.0.0");
    json_builder_end_object (builder);
    json_builder_end_object (builder);
    json_builder_end_object (builder);

    net_req = json_builder_get_root(builder);

    g_object_unref(builder);

    IpcamMessageHandler *net_handler = g_object_new(IPCAM_TYPE_NETWORK_MSG_HANDLER, "app", iconfig, NULL);
    if (net_handler) {
        ipcam_message_handler_do_put(net_handler, "set_network", net_req, &net_resp);

        if (net_resp)
            json_node_free(net_resp);
        g_object_unref(net_handler);

        return TRUE;
    }
    return FALSE;
}

static gboolean
ipcam_szyc_msg_handler_put_action_impl(IpcamMessageHandler *handler, JsonNode *request, JsonNode **response)
{
    JsonBuilder *builder = json_builder_new();
    JsonObject *req_obj;
    GList *members, *item;

    ipcam_szyc_msg_handler_apply_network_change (handler, request);

    req_obj = json_object_get_object_member(json_node_get_object(request), "items");
    members = json_object_get_members(req_obj);

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_object(builder);
    for (item = g_list_first(members); item; item = g_list_next(item))
    {
        const gchar *name = item->data;
        const gchar *value = json_object_get_string_member(req_obj, name);
        ipcam_szyc_msg_handler_update_param(IPCAM_SZYC_MSG_HANDLER(handler), name, value);
        ipcam_szyc_msg_handler_read_param(IPCAM_SZYC_MSG_HANDLER(handler), builder, name);
    }
    g_list_free(members);
    json_builder_end_object(builder);
    json_builder_end_object(builder);

    *response = json_builder_get_root(builder);

    g_object_unref(builder);

    return TRUE;
}

static void
ipcam_szyc_msg_handler_class_init (IpcamSzycMsgHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    IpcamMessageHandlerClass *parent_class = IPCAM_MESSAGE_HANDLER_CLASS(klass);

    object_class->finalize = ipcam_szyc_msg_handler_finalize;

    parent_class->get_action = ipcam_szyc_msg_handler_get_action_impl;
    parent_class->put_action = ipcam_szyc_msg_handler_put_action_impl;
}
