#include <gio/gio.h>
#include <string.h>
#include "ajax.h"
#include "iconfig.h"
#include "http_parser.h"
#include "http_request.h"
#include "http_proc.h"
#include "http_response.h"

enum {
  PROP_0,
  PROP_APP,
  PROP_ADDRESS,
  PROP_PORT,
  N_PROPERTIES
};

typedef struct _IPcamAjaxPrivate
{
    IpcamIConfig *iconfig;
    gchar *address;
    guint port;
    GThread *thread;
    volatile gboolean terminated;
} IpcamAjaxPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamAjax, ipcam_ajax, G_TYPE_OBJECT)

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static gpointer ajax_worker(gpointer data);
static gpointer request_proc(gpointer data);

static void ipcam_ajax_finalize(GObject *object)
{
    IpcamAjax *ajax = IPCAM_AJAX(object);
    IpcamAjaxPrivate *priv = ipcam_ajax_get_instance_private(ajax);
    priv->terminated = TRUE;
    g_thread_join(priv->thread);
    g_clear_pointer(&priv->thread, g_thread_unref);
    g_free(priv->address);
    G_OBJECT_CLASS(ipcam_ajax_parent_class)->finalize(object);
}
static void ipcam_ajax_init(IpcamAjax *ajax)
{
    IpcamAjaxPrivate *priv = ipcam_ajax_get_instance_private(ajax);
    priv->terminated = FALSE;
    priv->thread = g_thread_new("ajax-worker", ajax_worker, ajax);
}
static void ipcam_ajax_get_property(GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
    IpcamAjax *self = IPCAM_AJAX(object);
    IpcamAjaxPrivate *priv = ipcam_ajax_get_instance_private(self);
    switch(property_id)
    {
    case PROP_APP:
        {
            g_value_set_object(value, priv->iconfig);
        }
        break;
    case PROP_ADDRESS:
        {
            g_value_set_string(value, priv->address);
        }
        break;
    case PROP_PORT:
        {
            g_value_set_int(value, priv->port);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_ajax_set_property(GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    IpcamAjax *self = IPCAM_AJAX(object);
    IpcamAjaxPrivate *priv = ipcam_ajax_get_instance_private(self);
    switch(property_id)
    {
    case PROP_APP:
        {
            priv->iconfig = g_value_get_object(value);
        }
        break;
    case PROP_ADDRESS:
        {
            g_free(priv->address);
            priv->address = g_value_dup_string(value);
        }
        break;
    case PROP_PORT:
        {
            priv->port = g_value_get_int(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_ajax_class_init(IpcamAjaxClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = &ipcam_ajax_finalize;
    object_class->get_property = &ipcam_ajax_get_property;
    object_class->set_property = &ipcam_ajax_set_property;

    obj_properties[PROP_APP] =
        g_param_spec_object("app",
                            "Application",
                            "The parent application of the ajax thread.",
                            IPCAM_ICONFIG_TYPE, // default value
                            G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    obj_properties[PROP_ADDRESS] =
        g_param_spec_string("address",
                            "Web service listen address.",
                            "Web service listen address.",
                            NULL, // default value
                            G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    obj_properties[PROP_PORT] =
        g_param_spec_int("port",
                         "Web service listen port.",
                         "Web service listen port.",
                         1024,
                         65535,
                         1025, // default value
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}
gboolean ipcam_ajax_get_terminated(IpcamAjax *ajax)
{
    g_return_val_if_fail(IPCAM_IS_AJAX(ajax), FALSE);
    IpcamAjaxPrivate *priv = ipcam_ajax_get_instance_private(ajax);
    return priv->terminated;
}
static gpointer ajax_worker(gpointer data)
{
    IpcamAjax *ajax = data;
    gchar *address;
    guint port;
    g_object_get(ajax, "address", &address, "port", &port, NULL);
    GSocketAddress *socket_address = g_inet_socket_address_new_from_string(address, port);
    g_free(address);
    GSocket *server = g_socket_new(G_SOCKET_FAMILY_IPV4,
                                   G_SOCKET_TYPE_STREAM,
                                   G_SOCKET_PROTOCOL_TCP,
                                   NULL);
    g_socket_set_blocking(server, FALSE);
    g_socket_bind(server, socket_address, TRUE, NULL);
    g_socket_listen(server, NULL);
    GMutex mutex;
    g_mutex_init(&mutex);
    
    while (!ipcam_ajax_get_terminated(ajax))
    {
        GSocket *worker = g_socket_accept(server, NULL, NULL);
        if (worker)
        {
            g_socket_set_blocking(worker, TRUE);
            GObject **data = g_new(GObject *, 3);
            g_object_get(ajax, "app", &data[0], NULL);
            data[1] = G_OBJECT(worker);
            data[2] = &mutex;
            GThread *thread =g_thread_new("request-proc", request_proc, data);
            g_thread_unref(thread);
        }
        else
        {
            usleep(500000);
        }
    }

    g_mutex_clear(&mutex);
    g_socket_close(server, NULL);
    g_object_unref(server);
    g_thread_exit(0);
    return NULL;
}
static gpointer request_proc(gpointer data)
{
    GObject **params = data;
    IpcamIConfig *app = IPCAM_ICONFIG(params[0]);
    GSocket *worker = G_SOCKET(params[1]);
    gchar *buffer = g_new(gchar, 2048);
    gssize len;
    GMutex *mutex = params[2];

    len = g_socket_receive(worker, buffer, 2047, NULL, NULL);
    if (len > 0)
    {
        IpcamHttpParser *parser = g_object_new(IPCAM_HTTP_PARSER_TYPE, NULL);
        IpcamHttpRequest *request = ipcam_http_parser_get_request(parser, buffer, len);
        IpcamHttpProc *proc = g_object_new(IPCAM_HTTP_PROC_TYPE, "app", app, NULL);
        IpcamHttpResponse *response = ipcam_http_proc_get_response(proc, request);
        if (response)
        {
            g_mutex_lock(mutex);
            ipcam_http_response_write_string(response, worker);
            g_mutex_unlock(mutex);
        }
        g_clear_object(&response);
        g_clear_object(&proc);
        g_clear_object(&request);
        g_clear_object(&parser);
    }

    g_free(buffer);
    g_socket_close(worker, NULL);
    g_clear_object(&worker);
    g_free(params);
    g_thread_exit(0);
    return NULL;
}
