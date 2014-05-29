#ifndef __HTTP_REQUEST_HANDLER_H__
#define __HTTP_REQUEST_HANDLER_H__

#include <regex.h>

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
    gboolean (*handler)(IpcamHttpRequestHandler *, IpcamHttpRequest *, IpcamHttpResponse *);
};

typedef struct _IpcamHttpRequestHandlerData
{
    guint method;
    gchar *regex_str;
    regex_t regex;
    gssize n_match;
} IpcamHttpRequestHandlerData;
typedef IpcamHttpRequestHandlerData handler_data;

#define START_HANDLER(NAME, METHOD, REGEX, RESULT, NUM, MATCHES) \
static void ipcam_http_proc_##NAME##_func(IpcamHttpResponse *, regmatch_t[]); \
handler NAME##_data = {ipcam_http_proc_##NAME##_func, METHOD, REGEX, {0}, NUM}; \
handler *NAME = &NAME##_data; \
static void ipcam_http_proc_##NAME##_func(IpcamHttpResponse *RESULT, regmatch_t MATCHES[]) { \
    IpcamHttpResponse *response = RESULT;

#define END_HANDLER \
}

GType ipcam_http_request_handler_get_type(void);
gboolean ipcam_http_request_handler_dispatch(IpcamHttpRequestHandler *http_request_handler,
                                             IpcamHttpRequest *http_request,
                                             IpcamHttpResponse *http_response);
void ipcam_http_request_handler_register(IpcamHttpRequestHandler *http_request_handler,
                                         handler_data *handler);


#endif /* __HTTP_REQUEST_HANDLER_H__ */
