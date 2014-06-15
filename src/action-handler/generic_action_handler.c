#include "generic_action_handler.h"
#include "messages.h"
#include "iconfig.h"
#include "../msg-handler/ipcam-base-info-handler.h"
#include "../msg-handler/ipcam-osd-handler.h"
#include "../msg-handler/ipcam-video-handler.h"
#include "../msg-handler/ipcam-scene-handler.h"
#include "../msg-handler/ipcam-network-handler.h"
#include "../msg-handler/ipcam-datetime-handler.h"
#include "../msg-handler/ipcam-users-handler.h"

typedef struct _IpcamGenericActionHandlerPrivate
{
    GHashTable *action_hash;
} IpcamGenericActionHandlerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamGenericActionHandler, ipcam_generic_action_handler, IPCAM_ACTION_HANDLER_TYPE);

static void ipcam_generic_action_handler_run_impl(IpcamActionHandler *action_handler,
                                               IpcamMessage *message);

typedef enum _ActionMethod
{
    ACT_GET,
    ACT_SET,
    ACT_ADD,
    ACT_DEL
} ActionMethod;

typedef struct _MsgHandlerHashValue
{
    ActionMethod    method;
    GType           g_type;
} MsgHandlerHashValue;

static MsgHandlerHashValue *msg_handler_hash_value_init(ActionMethod method, GType g_type)
{
    MsgHandlerHashValue *v = g_new0(MsgHandlerHashValue, 1);
    if (v)
    {
        v->method = method;
        v->g_type = g_type;
    }
    return v;
}

static void ipcam_generic_action_handler_init(IpcamGenericActionHandler *self)
{
    IpcamGenericActionHandlerPrivate *priv = ipcam_generic_action_handler_get_instance_private(self);

    priv->action_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);

#define _       msg_handler_hash_value_init
    g_hash_table_insert(priv->action_hash, "get_base_info", 
                        _(ACT_GET, IPCAM_TYPE_BASE_INFO_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_base_info", 
                        _(ACT_SET, IPCAM_TYPE_BASE_INFO_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_osd", 
                        _(ACT_GET, IPCAM_TYPE_OSD_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_osd", 
                        _(ACT_SET, IPCAM_TYPE_OSD_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_video", 
                        _(ACT_GET, IPCAM_TYPE_VIDEO_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_video", 
                        _(ACT_SET, IPCAM_TYPE_VIDEO_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_scene", 
                        _(ACT_GET, IPCAM_TYPE_SCENE_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_scene", 
                        _(ACT_SET, IPCAM_TYPE_SCENE_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_network", 
                        _(ACT_GET, IPCAM_TYPE_NETWORK_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_network", 
                        _(ACT_SET, IPCAM_TYPE_NETWORK_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_datetime", 
                        _(ACT_GET, IPCAM_TYPE_DATETIME_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_datetime", 
                        _(ACT_SET, IPCAM_TYPE_DATETIME_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_users", 
                        _(ACT_GET, IPCAM_TYPE_USERS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_users", 
                        _(ACT_SET, IPCAM_TYPE_USERS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "add_users", 
                        _(ACT_ADD, IPCAM_TYPE_USERS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "del_users", 
                        _(ACT_DEL, IPCAM_TYPE_USERS_MSG_HANDLER));
}

static void ipcam_generic_action_handler_finalize(GObject *gobject)
{
    IpcamGenericActionHandler *self = IPCAM_GENERIC_ACTION_HANDLER(gobject);
    IpcamGenericActionHandlerPrivate *priv = ipcam_generic_action_handler_get_instance_private(self);

    g_hash_table_destroy (priv->action_hash);
}

static void ipcam_generic_action_handler_class_init(IpcamGenericActionHandlerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    IpcamActionHandlerClass *action_handler_class = IPCAM_ACTION_HANDLER_CLASS(klass);

    gobject_class->finalize = ipcam_generic_action_handler_finalize;
    action_handler_class->run = &ipcam_generic_action_handler_run_impl;
}

static void ipcam_generic_action_handler_run_impl(IpcamActionHandler *self,
                                               IpcamMessage *message)
{
    IpcamGenericActionHandlerPrivate *priv = ipcam_generic_action_handler_get_instance_private(self);
    IpcamIConfig *iconfig;
    IpcamRequestMessage *rq_msg;
    IpcamResponseMessage *rp_msg;
    const gchar *action;
    JsonNode *rq_body;
    JsonNode *rp_body;
    MsgHandlerHashValue *hash_val;
    gchar *token;

    g_object_get(G_OBJECT(self), "service", &iconfig, NULL);
    rq_msg = IPCAM_REQUEST_MESSAGE(message);
    g_object_get(G_OBJECT(rq_msg), "action", &action, "body", &rq_body, NULL);
    hash_val = g_hash_table_lookup(priv->action_hash, action);
    if (hash_val)
    {
        IpcamMessageHandler *handler = g_object_new(hash_val->g_type, NULL);
        if (handler)
        {
            gboolean ret = FALSE;
            switch (hash_val->method)
            {
                case ACT_GET:
                    ret = ipcam_message_handler_do_get(handler, rq_body, &rp_body);
                    break;
                case ACT_SET:
                    ret = ipcam_message_handler_do_put(handler, rq_body, &rp_body);
                    break;
                case ACT_ADD:
                    ret = ipcam_message_handler_do_post(handler, rq_body, &rp_body);
                    break;
                case ACT_DEL:
                    ret = ipcam_message_handler_do_delete(handler, rq_body, &rp_body);
                    break;
                 default:
                    break;
            }
            if (ret)
            {
                rp_msg = ipcam_request_message_get_response_message(rq_msg, "0");
                g_object_set(G_OBJECT(rq_msg), "body", rq_body, NULL);
            }
        }
    }
    rp_msg = ipcam_request_message_get_response_message(rq_msg, "1");
    g_object_get(G_OBJECT(rq_msg), "token", &token, NULL);
    ipcam_base_app_send_message(IPCAM_BASE_APP(iconfig), rp_msg, "iconfig", token, NULL, 0);
    g_object_unref(rp_msg);
    g_free(token);
}
