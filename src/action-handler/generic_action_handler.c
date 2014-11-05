#include "generic_action_handler.h"
#include "messages.h"
#include "iconfig.h"
#include "../msg-handler/ipcam-users-handler.h"
#include "../msg-handler/ipcam-params-handler.h"

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

    priv->action_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);

#define _(...)       msg_handler_hash_value_init(__VA_ARGS__)
    g_hash_table_insert(priv->action_hash, "get_params", 
                        _(ACT_GET, IPCAM_TYPE_PARAMS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_params", 
                        _(ACT_SET, IPCAM_TYPE_PARAMS_MSG_HANDLER));

    g_hash_table_insert(priv->action_hash, "get_users", 
                        _(ACT_GET, IPCAM_TYPE_USERS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "set_users", 
                        _(ACT_SET, IPCAM_TYPE_USERS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "add_users", 
                        _(ACT_ADD, IPCAM_TYPE_USERS_MSG_HANDLER));
    g_hash_table_insert(priv->action_hash, "del_users", 
                        _(ACT_DEL, IPCAM_TYPE_USERS_MSG_HANDLER));
#undef _
}

static void ipcam_generic_action_handler_finalize(GObject *gobject)
{
    IpcamGenericActionHandler *self = IPCAM_GENERIC_ACTION_HANDLER(gobject);
    IpcamGenericActionHandlerPrivate *priv = ipcam_generic_action_handler_get_instance_private(self);

    g_hash_table_destroy (priv->action_hash);
    G_OBJECT_CLASS(ipcam_generic_action_handler_parent_class)->finalize(gobject);
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
    IpcamGenericActionHandler *handler = IPCAM_GENERIC_ACTION_HANDLER(self);
    IpcamGenericActionHandlerPrivate *priv = ipcam_generic_action_handler_get_instance_private(handler);
    IpcamIConfig *iconfig;
    IpcamMessage *req_msg;
    IpcamMessage *resp_msg;
    const gchar *action;
    JsonNode *req_body;
    JsonNode *resp_body = NULL;
    MsgHandlerHashValue *hash_val;
    gchar *token;
    gboolean ret = FALSE;

    g_object_get(G_OBJECT(self), "service", &iconfig, NULL);
    req_msg = IPCAM_MESSAGE(message);
    g_object_get(G_OBJECT(req_msg), "action", &action, "body", &req_body, NULL);

    hash_val = g_hash_table_lookup(priv->action_hash, action);
    if (hash_val)
    {
        IpcamMessageHandler *handler = g_object_new(hash_val->g_type, "app", iconfig, NULL);
        if (handler)
        {
            switch (hash_val->method)
            {
                case ACT_GET:
                    ret = ipcam_message_handler_do_get(handler, action, req_body, &resp_body);
                    break;
                case ACT_SET:
                    ret = ipcam_message_handler_do_put(handler, action, req_body, &resp_body);
                    break;
                case ACT_ADD:
                    ret = ipcam_message_handler_do_post(handler, action, req_body, &resp_body);
                    break;
                case ACT_DEL:
                    ret = ipcam_message_handler_do_delete(handler, action, req_body, &resp_body);
                    break;
                 default:
                    break;
            }
            g_object_unref(handler);
        }
    }

    if (resp_body == NULL)
    {
        resp_body = json_node_new(JSON_NODE_OBJECT);
        json_node_take_object(resp_body, json_object_new());
    }

    resp_msg = (IpcamMessage *)ipcam_request_message_get_response_message(IPCAM_REQUEST_MESSAGE(req_msg),
                                                                          ret ? "0" : "1");
    g_object_set(G_OBJECT(resp_msg), "body", resp_body, NULL);

    g_object_get(G_OBJECT(req_msg), "token", &token, NULL);
    ipcam_base_app_send_message(IPCAM_BASE_APP(iconfig), resp_msg, "iconfig", token, NULL, 0);

    g_free((gchar *)action);
    g_object_unref(resp_msg);
    g_free(token);
}
