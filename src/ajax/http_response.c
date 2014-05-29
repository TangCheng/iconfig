#include <time.h>
#include "http_response.h"

enum
{
    PROP_0,
    PROP_HTTP_MAJOR,
    PROP_HTTP_MINOR,
    PROP_STATUS,
    PROP_BODY,
    N_PROPERTIES
};

typedef struct _IpcamHttpResponsePrivate
{
    guint http_major;
    guint http_minor;
    guint status;
    gchar *body;
} IpcamHttpResponsePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamHttpResponse, ipcam_http_response, G_TYPE_OBJECT)

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_http_response_prepare_response(IpcamHttpResponse *http_response);

static void ipcam_http_response_finalize(GObject *object)
{
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(IPCAM_HTTP_RESPONSE(object));
    g_free(priv->body);
    G_OBJECT_CLASS(ipcam_http_response_parent_class)->finalize(object);
}
static void ipcam_http_response_init(IpcamHttpResponse *self)
{
}
static void ipcam_http_response_get_property(GObject    *object,
                                             guint       property_id,
                                             GValue     *value,
                                             GParamSpec *pspec)
{
    IpcamHttpResponse *self = IPCAM_HTTP_RESPONSE(object);
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(self);
    switch(property_id)
    {
    case PROP_HTTP_MAJOR:
        {
            g_value_set_int(value, priv->http_major);
        }
        break;
    case PROP_HTTP_MINOR:
        {
            g_value_set_int(value, priv->http_minor);
        }
        break;
    case PROP_STATUS:
        {
            g_value_set_int(value, priv->status);
        }
        break;
    case PROP_BODY:
        {
            g_value_set_string(value, priv->body);
        }
        break;
   default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_http_response_set_property(GObject      *object,
                                             guint         property_id,
                                             const GValue *value,
                                             GParamSpec   *pspec)
{
    IpcamHttpResponse *self = IPCAM_HTTP_RESPONSE(object);
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(self);
    switch(property_id)
    {
    case PROP_HTTP_MAJOR:
        {
            priv->http_major = g_value_get_int(value);
        }
        break;
    case PROP_HTTP_MINOR:
        {
            priv->http_minor = g_value_get_int(value);
        }
        break;
    case PROP_STATUS:
        {
            priv->status = g_value_get_int(value);
        }
        break;
    case PROP_BODY:
        {
            g_free(priv->body);
            priv->body = g_value_dup_string(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_http_response_class_init(IpcamHttpResponseClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = &ipcam_http_response_finalize;
    object_class->get_property = &ipcam_http_response_get_property;
    object_class->set_property = &ipcam_http_response_set_property;

    obj_properties[PROP_HTTP_MAJOR] =
        g_param_spec_int("http-major",
                         "http major version",
                         "http major version",
                         1,
                         10,
                         1,
                         G_PARAM_READWRITE);
    obj_properties[PROP_HTTP_MINOR] =
        g_param_spec_int("http-minor",
                         "http minor version",
                         "http minor version",
                         0,
                         10,
                         1,
                         G_PARAM_READWRITE);
    obj_properties[PROP_STATUS] =
        g_param_spec_int("status",
                         "http status code",
                         "http status code",
                         0,
                         65535,
                         200,
                         G_PARAM_READWRITE);
    obj_properties[PROP_BODY] =
        g_param_spec_string("body",
                            "html body",
                            "html body",
                            NULL,
                            G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}
gchar *ipcam_http_response_get_string(IpcamHttpResponse *http_response)
{    
/*    time(&now);
    strftime(priv->header[HEADER_DATE][1], 32, "%a, %d %b %Y %T %Z", gmtime(&now));
*/
    return NULL;
}


