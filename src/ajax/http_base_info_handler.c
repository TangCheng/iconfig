#include "http_base_info_handler.h"
#include "http_request.h"
#include "http_response.h"

G_DEFINE_TYPE(IpcamHttpBaseInfoHandler, ipcam_http_base_info_handler, IPCAM_HTTP_REQUEST_HANDLER_TYPE)

START_HANDLER(get_base_info, HTTP_GET, "/api/1.0/base_info.json", http_request, http_response, 0, matches)
{

}
END_HANDLER


static void ipcam_http_base_info_handler_init(IpcamHttpBaseInfoHandler *self)
{
    ipcam_http_request_handler_register(IPCAM_HTTP_REQUEST_HANDLER(self), get_base_info);
}
static void ipcam_http_base_info_handler_class_init(IpcamHttpBaseInfoHandlerClass *klass)
{
}
