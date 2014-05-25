#include <regex.h>
#include <string.h>
#include "http_response.h"
#include "iconfig.h"
#include "http_request.h"

#define GET 1
#define POST 2
#define PUT 3
#define HEAD 4
#define DELETE 5

struct handler
{
    void (*func)(IpcamHttpResponse*, regmatch_t[]);
    int method;
    const char *regex_str;
    regex_t regex;
    size_t nmatch;
};
typedef struct handler handler;

#define START_HANDLER(NAME, METHOD, REGEX, RESULT, NUM, MATCHES) \
static void ipcam_http_response_##NAME##_func(IpcamHttpResponse *, regmatch_t[]); \
handler NAME##_data = {ipcam_http_response_##NAME##_func, METHOD, REGEX, {0}, NUM}; \
handler *NAME = &NAME##_data; \
static void ipcam_http_response_##NAME##_func(IpcamHttpResponse *RESULT, regmatch_t MATCHES[]) { \
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(RESULT); \

#define END_HANDLER \
}

enum
{
    PROP_0,
    PROP_APP,
    PROP_REQUEST,
    N_PROPERTIES
};

typedef struct _IpcamHttpResponsePrivate
{
    IpcamIConfig *iconfig;
    IpcamHttpRequest *request;
    gchar *response;
    GList *handler_list;
} IpcamHttpResponsePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamHttpResponse, ipcam_http_response, G_TYPE_OBJECT)

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };
static void ipcam_http_response_dispatch(IpcamHttpResponse *http_response);
static void ipcam_http_response_add_handler(IpcamHttpResponse *http_response, handler *h);
static void ipcam_http_response_init_handlers(IpcamHttpResponse *http_response);
static void ipcam_http_response_cleanup_handlers(IpcamHttpResponse *http_response);

START_HANDLER(base_info, GET, "/api/1.0/base_info.json", http_response, 0, matches) {
    gchar *query_string;
    g_object_get(priv->request, "query-string", &query_string, NULL);
    g_print("%s\n", query_string);
    g_free(query_string);
    priv->response = g_strdup("HTTP/1.1 200 OK\r\ncontent-type: application/x-javascript; charset=utf-8\r\ncontent-length: 53\r\n\r\n{\"infos\": {\"device_name\": \"ipcam\", \"comment\": \"aaa\"}}\r\n\r\n");
} END_HANDLER

static void ipcam_http_response_finalize(GObject *object)
{
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(IPCAM_HTTP_RESPONSE(object));
    ipcam_http_response_cleanup_handlers(IPCAM_HTTP_RESPONSE(object));
    g_free(priv->response);
    g_list_free(priv->handler_list);
    G_OBJECT_CLASS(ipcam_http_response_parent_class)->finalize(object);
}
static void ipcam_http_response_init(IpcamHttpResponse *self)
{
    ipcam_http_response_add_handler(self, base_info);
    ipcam_http_response_init_handlers(self);
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
    case PROP_APP:
        {
            g_value_set_object(value, priv->iconfig);
        }
        break;
    case PROP_REQUEST:
        {
            g_value_set_object(value, priv->request);
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
    case PROP_APP:
        {
            priv->iconfig = g_value_get_object(value);
        }
        break;
    case PROP_REQUEST:
        {
            priv->request = g_value_get_object(value);
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

    obj_properties[PROP_APP] =
        g_param_spec_object("app",
                            "app",
                            "app",
                            IPCAM_ICONFIG_TYPE,
                            G_PARAM_READWRITE);
    obj_properties[PROP_REQUEST] =
        g_param_spec_object("request",
                            "request",
                            "request",
                            IPCAM_HTTP_REQUEST_TYPE,
                            G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}
gchar *ipcam_http_response_get_result(IpcamHttpResponse *http_response)
{
    g_return_val_if_fail(IPCAM_IS_HTTP_RESPONSE(http_response), NULL);
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(http_response);
    ipcam_http_response_dispatch(http_response);
    return priv->response;
}
static void ipcam_http_response_dispatch(IpcamHttpResponse *http_response)
{
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(http_response);
    
    gchar *path = NULL;
    g_object_get(priv->request, "path", &path, NULL);
    if (path == NULL) {
        return;
    }
    char *method_str = NULL;
    g_object_get(priv->request, "method", &method_str, NULL);
    if (method_str == NULL) {
        g_free(path);
        return;
    }
    int method;
    if (strcmp(method_str, "GET") == 0) {
        method = GET;
    } else if (strcmp(method_str, "POST") == 0) {
        method = POST;
    } else if (strcmp(method_str, "PUT") == 0) {
        method = PUT;
    } else if (strcmp(method_str, "HEAD") == 0) {
        method = HEAD;
    } else if (strcmp(method_str, "DELETE") == 0) {
        method = DELETE;
    } else {
        g_free(path);
        g_free(method_str);
        return;
    }
    g_free(method_str);
    GList *item = g_list_first(priv->handler_list);
    for (; item != NULL; item = g_list_next(item)) {
        handler *cur = item->data;
        if (cur->method == method) {
            regmatch_t *matches = g_new(regmatch_t, cur->nmatch);
            int m = regexec(&cur->regex, path, cur->nmatch, matches, 0);
            if (m == 0) {
                cur->func(http_response, matches);
                g_free(matches);
                g_free(path);
                return;
            }
            g_free(matches);
        }
    }
    g_free(path);
}
static void ipcam_http_response_add_handler(IpcamHttpResponse *http_response, handler *h)
{
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(http_response);
    priv->handler_list = g_list_append(priv->handler_list, h);
}
static void ipcam_http_response_init_handlers(IpcamHttpResponse *http_response)
{
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(http_response);
    GList *item = g_list_first(priv->handler_list);
    for (; item != NULL; item = g_list_next(item)) {
        handler *cur = item->data;
        if (regcomp(&cur->regex, cur->regex_str, 0) != 0) {
            // do nothing
        }
    }
}
static void ipcam_http_response_cleanup_handlers(IpcamHttpResponse *http_response)
{
    IpcamHttpResponsePrivate *priv = ipcam_http_response_get_instance_private(http_response);
    GList *item = g_list_first(priv->handler_list);
    for (; item != NULL; item = g_list_next(item)) {
        handler *cur = item->data;
        regfree(&cur->regex);
    }
}
