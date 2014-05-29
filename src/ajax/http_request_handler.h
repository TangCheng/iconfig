#ifndef __HTTP_REQUEST_HANDLER_H__
#define __HTTP_REQUEST_HANDLER_H__

#include <regex.h>
#include <http_parser.h>
#include "http_request.h"
#include "http_response.h"

#define IPCAM_HTTP_REQUEST_HANDLER_TYPE (ipcam_http_request_handler_get_type())
#define IPCAM_HTTP_REQUEST_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_HTTP_REQUEST_HANDLER_TYPE, IpcamHttpRequestHandler))
#define IPCAM_HTTP_REQUEST_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_HTTP_REQUEST_HANDLER_TYPE, IpcamHttpRequestHandlerClass))
#define IPCAM_IS_HTTP_REQUEST_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_HTTP_REQUEST_HANDLER_TYPE))
#define IPCAM_IS_HTTP_REQUEST_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_HTTP_REQUEST_HANDLER_TYPE))
#define IPCAM_HTTP_REQUEST_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_HTTP_REQUEST_HANDLER_TYPE, IpcamHttpRequestHandlerClass))

typedef struct _IpcamHttpRequestHandler IpcamHttpRequestHandler;
typedef struct _IpcamHttpRequestHandlerClass IpcamHttpRequestHandlerClass;

struct _IpcamHttpRequestHandler
{
    GObject parent;
};

struct _IpcamHttpRequestHandlerClass
{
    GObjectClass parent_class;
};

typedef struct _IpcamHttpRequestHandlerData
{
    gboolean (*func)(IpcamHttpRequestHandler *, IpcamHttpRequest *, IpcamHttpResponse *);
    enum http_method method;
    gchar *regex_str;
    regex_t regex;
    gssize n_match;
} IpcamHttpRequestHandlerData;
typedef IpcamHttpRequestHandlerData handler_data;

#define START_HANDLER(NAME, METHOD, REGEX, REQUEST, RESULT, NUM, MATCHES) \
static gboolean ipcam_http_##NAME##_func(IpcamHttpRequestHandler *, IpcamHttpRequest *, IpcamHttpResponse *); \
handler_data NAME##_data = {ipcam_http_##NAME##_func, METHOD, REGEX, {0}, NUM}; \
handler_data *NAME = &NAME##_data; \
static gboolean ipcam_http_##NAME##_func(IpcamHttpRequestHandler *NAME, IpcamHttpRequest *REQUEST, IpcamHttpResponse *RESULT) { \
    gboolean ret = FALSE;


#define END_HANDLER \
    return ret; \
}

GType ipcam_http_request_handler_get_type(void);
gboolean ipcam_http_request_handler_dispatch(IpcamHttpRequestHandler *http_request_handler,
                                             IpcamHttpRequest *http_request,
                                             IpcamHttpResponse *http_response);
void ipcam_http_request_handler_register(IpcamHttpRequestHandler *http_request_handler,
                                         handler_data *handler);


#endif /* __HTTP_REQUEST_HANDLER_H__ */
