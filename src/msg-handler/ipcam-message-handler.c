/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-message-handler.c
 * Copyright (C) 2014 Watson Xu <watson@watsons-pc>
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
#include "ipcam-message-handler.h"
#include "iconfig.h"

struct _IpcamMessageHandlerPrivate
{
    IpcamIConfig *iconfig;
};


enum
{
    PROP_0,

    PROP_APP
};



G_DEFINE_TYPE_WITH_PRIVATE (IpcamMessageHandler, ipcam_message_handler, G_TYPE_OBJECT);

static void
ipcam_message_handler_init (IpcamMessageHandler *ipcam_message_handler)
{
}

static void
ipcam_message_handler_finalize (GObject *object)
{
    G_OBJECT_CLASS (ipcam_message_handler_parent_class)->finalize (object);
}

static void
ipcam_message_handler_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    IpcamMessageHandler *self;
    IpcamMessageHandlerPrivate *priv;

    g_return_if_fail (IPCAM_IS_MESSAGE_HANDLER (object));

    self = IPCAM_MESSAGE_HANDLER(object);
    priv = ipcam_message_handler_get_instance_private(self);

    switch (prop_id)
    {
    case PROP_APP:
        priv->iconfig = g_value_get_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
ipcam_message_handler_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    IpcamMessageHandler *self;
    IpcamMessageHandlerPrivate *priv;

    g_return_if_fail (IPCAM_IS_MESSAGE_HANDLER (object));

    self = IPCAM_MESSAGE_HANDLER(object);
    priv = ipcam_message_handler_get_instance_private(self);

    switch (prop_id)
    {
    case PROP_APP:
        g_value_set_object(value, priv->iconfig);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static gboolean
ipcam_message_handler_get_action_impl(IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_printf("ipcam_message_handler_get_action() should be overridden.\n");
    return FALSE;
}

static gboolean
ipcam_message_handler_put_action_impl(IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_printf("ipcam_message_handler_put_action() should be overridden.\n");
    return FALSE;
}

static gboolean
ipcam_message_handler_post_action_impl(IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_printf("ipcam_message_handler_post_action() should be overridden.\n");
    return FALSE;
}

static gboolean
ipcam_message_handler_delete_action_impl(IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_printf("ipcam_message_handler_delete_action() should be overridden.\n");
    return FALSE;
}

static void
ipcam_message_handler_class_init (IpcamMessageHandlerClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = ipcam_message_handler_finalize;
    object_class->set_property = ipcam_message_handler_set_property;
    object_class->get_property = ipcam_message_handler_get_property;

    g_object_class_install_property (object_class,
                                     PROP_APP,
                                     g_param_spec_object ("app",
                                                          "Application",
                                                          "Application",
                                                          IPCAM_ICONFIG_TYPE,
                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
    klass->get_action = ipcam_message_handler_get_action_impl;
    klass->get_action = ipcam_message_handler_put_action_impl;
    klass->get_action = ipcam_message_handler_post_action_impl;
    klass->get_action = ipcam_message_handler_delete_action_impl;
}

gboolean
ipcam_message_handler_do_get (IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_return_if_fail (IPCAM_IS_MESSAGE_HANDLER(self));

    IPCAM_MESSAGE_HANDLER_GET_CLASS(self)->get_action(self, request, response);
}

gboolean
ipcam_message_handler_do_put (IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_return_if_fail (IPCAM_IS_MESSAGE_HANDLER(self));

    IPCAM_MESSAGE_HANDLER_GET_CLASS(self)->put_action(self, request, response);
}

gboolean
ipcam_message_handler_do_post (IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_return_if_fail (IPCAM_IS_MESSAGE_HANDLER(self));

    IPCAM_MESSAGE_HANDLER_GET_CLASS(self)->post_action(self, request, response);
}

gboolean
ipcam_message_handler_do_delete (IpcamMessageHandler *self, JsonNode *request, JsonNode **response)
{
    g_return_if_fail (IPCAM_IS_MESSAGE_HANDLER(self));

    IPCAM_MESSAGE_HANDLER_GET_CLASS(self)->delete_action(self, request, response);
}