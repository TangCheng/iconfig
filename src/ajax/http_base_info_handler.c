#include "http_base_info_handler.h"
#include "http_request.h"
#include "http_response.h"
#include "iconfig.h"

G_DEFINE_TYPE(IpcamHttpBaseInfoHandler, ipcam_http_base_info_handler, IPCAM_HTTP_REQUEST_HANDLER_TYPE)

START_HANDLER(get_base_info, HTTP_GET, "/api/1.0/base_info.json", http_request, http_response)
{
    guint major, minor;
    g_object_get(http_request, "http-major", &major, "http-minor", &minor, NULL);
    g_object_set(http_response,
                 "http-major", major,
                 "http-minor", minor,
                 "status", 200,
                  NULL);
    IpcamIConfig *iconfig;
    g_object_get(get_base_info, "app", &iconfig, NULL);
    GList *infos_list = NULL;
    infos_list = g_list_append(infos_list, "device_name");
    infos_list = g_list_append(infos_list, "comment");
    gchar *infos = ipcam_iconfig_get_base_info(iconfig, infos_list);
    g_object_set(http_response, "body", infos, NULL);
    g_free(infos);
    g_list_free(infos_list);
    ret = TRUE;
}
END_HANDLER

START_HANDLER(put_base_info, HTTP_PUT, "/api/1.0/base_info.json", http_request, http_response)
{
    guint major, minor;
    g_object_get(http_request, "http-major", &major, "http-minor", &minor, NULL);
    g_object_set(http_response,
                 "http-major", major,
                 "http-minor", minor,
                 "status", 200,
                 NULL);
    ret = TRUE;
}
END_HANDLER

static void ipcam_http_base_info_handler_init(IpcamHttpBaseInfoHandler *self)
{
    ipcam_http_request_handler_register(IPCAM_HTTP_REQUEST_HANDLER(self), get_base_info);
}
static void ipcam_http_base_info_handler_class_init(IpcamHttpBaseInfoHandlerClass *klass)
{
}
