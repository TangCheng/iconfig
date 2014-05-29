#include "http_base_info_handler.h"
#include "http_request.h"
#include "http_response.h"

G_DEFINE_TYPE(IpcamHttpBaseInfoHandler, ipcam_http_base_info_handler, IPCAM_HTTP_REQUEST_HANDLER_TYPE)

static gboolean ipcam_http_base_info_handler_handler_impl(IpcamHttpBaseInfoHandler *handler,
                                                          IpcamHttpRequest *request,
                                                          IpcamHttpResponse *response);

/*
START_HANDLER(base_info, GET, "/api/1.0/base_info.json", http_response, 0, matches) {

} END_HANDLER
*/

static void ipcam_http_base_info_handler_init(IpcamHttpBaseInfoHandler *self)
{
}
static void ipcam_http_base_info_handler_class_init(IpcamHttpBaseInfoHandlerClass *klass)
{
    IpcamHttpRequestHandlerClass *request_handler_class = IPCAM_HTTP_REQUEST_HANDLER_CLASS(klass);
    request_handler_class->handler = &ipcam_http_base_info_handler_handler_impl;
}
static gboolean ipcam_http_base_info_handler_handler_impl(IpcamHttpBaseInfoHandler *handler,
                                                          IpcamHttpRequest *request,
                                                          IpcamHttpResponse *response)
{
    return FALSE;
}
