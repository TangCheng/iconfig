/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ipcam-message-handler.h
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

#ifndef _IPCAM_MESSAGE_HANDLER_H_
#define _IPCAM_MESSAGE_HANDLER_H_

#include <glib-object.h>
#include <json-glib/json-glib.h>

G_BEGIN_DECLS

#define IPCAM_TYPE_MESSAGE_HANDLER             (ipcam_message_handler_get_type ())
#define IPCAM_MESSAGE_HANDLER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), IPCAM_TYPE_MESSAGE_HANDLER, IpcamMessageHandler))
#define IPCAM_MESSAGE_HANDLER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), IPCAM_TYPE_MESSAGE_HANDLER, IpcamMessageHandlerClass))
#define IPCAM_IS_MESSAGE_HANDLER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IPCAM_TYPE_MESSAGE_HANDLER))
#define IPCAM_IS_MESSAGE_HANDLER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), IPCAM_TYPE_MESSAGE_HANDLER))
#define IPCAM_MESSAGE_HANDLER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), IPCAM_TYPE_MESSAGE_HANDLER, IpcamMessageHandlerClass))

typedef struct _IpcamMessageHandlerClass IpcamMessageHandlerClass;
typedef struct _IpcamMessageHandler IpcamMessageHandler;
typedef struct _IpcamMessageHandlerPrivate IpcamMessageHandlerPrivate;


struct _IpcamMessageHandlerClass
{
    GObjectClass parent_class;
    gboolean (*get_action)(IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
    gboolean (*put_action)(IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
    gboolean (*post_action)(IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
    gboolean (*delete_action)(IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
};

struct _IpcamMessageHandler
{
    GObject parent_instance;

    IpcamMessageHandlerPrivate *priv;
};

GType ipcam_message_handler_get_type (void) G_GNUC_CONST;
gboolean ipcam_message_handler_do_get (IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
gboolean ipcam_message_handler_do_put (IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
gboolean ipcam_message_handler_do_post (IpcamMessageHandler *self, JsonNode *request, JsonNode **response);
gboolean ipcam_message_handler_do_delete (IpcamMessageHandler *self, JsonNode *request, JsonNode **response);

G_END_DECLS

#endif /* _IPCAM_MESSAGE_HANDLER_H_ */

