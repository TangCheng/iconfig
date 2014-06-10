#include "http_video_handler.h"
#include "http_request.h"
#include "http_response.h"
#include "http_query_string_parser.h"
#include "iconfig.h"

G_DEFINE_TYPE(IpcamHttpVideoParamHandler, ipcam_http_video_param_handler, IPCAM_HTTP_REQUEST_HANDLER_TYPE)

static void destroy_data(gpointer data);

START_HANDLER(get_video_param, HTTP_GET, "/api/1.0/video_param.json", http_request, http_response)
{
    IpcamIConfig *iconfig;
    IpcamHttpQueryStringParser *parser;
    gchar *query_string = NULL;
    GList *param_list = NULL;
    GHashTable *query_hash = NULL;
    gboolean success = FALSE;
    
    g_object_get(get_video_param, "app", &iconfig, NULL);
    g_object_get(http_request, "query-string", &query_string, NULL);
    if (query_string) 
    {
        parser = g_object_new(IPCAM_HTTP_QUERY_STRING_PARSER_TYPE, NULL);
        query_hash = ipcam_http_query_string_parser_get(parser, query_string);
        if (query_hash)
        {
            param_list = g_hash_table_lookup(query_hash, "params[]");
            if (param_list)
            {
                gchar *infos = ipcam_iconfig_get_video_param(iconfig, param_list);
                g_object_set(http_response, "body", infos, NULL);
                g_free(infos);
                g_object_set(http_response,
                             "status", 200,
                             NULL);
                success = TRUE;
            }
        }
        g_free(query_string);
        g_clear_object(&parser);
    }
    if (!success)
    {
        ipcam_http_response_success(http_response, success);
    }
    ret = TRUE;
}
END_HANDLER

START_HANDLER(put_video_param, HTTP_PUT, "/api/1.0/video_param.json", http_request, http_response)
{
    gchar *body = NULL;
    IpcamIConfig *iconfig;
    GHashTable *infos_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, destroy_data);
    gboolean success = FALSE;

    g_object_get(put_video_param, "app", &iconfig, NULL);
    g_object_get(http_request, "body", &body, NULL);
    if (body)
    {
        JsonParser *parser = json_parser_new();
		GError *err = NULL;
        if (json_parser_load_from_data(parser, body, -1, &err))
        {
            JsonNode *body_node;
            JsonObject *param_obj;
            GList *m;
			body_node = json_parser_get_root(parser);
            param_obj = json_object_get_object_member(json_node_get_object(body_node),
                                                      "video_param");
            m = json_object_get_members(param_obj);
            while(m)
            {
                gchar *pname = m->data;
                if (pname)
                {
                    gint val = json_object_get_int_member(param_obj, pname);
                    ipcam_iconfig_set_video_param(iconfig, pname, val);
                }
                m = m->next;
            }
            g_list_free(m);

            g_object_set(http_response,
                         "status", 200,
                         NULL);
            success = TRUE;
        }
        g_object_unref(parser);
		g_free(body);
    }
    ipcam_http_response_success(http_response, success);
    g_hash_table_destroy(infos_hash);
    ret = TRUE;
}
END_HANDLER

static void ipcam_http_video_param_handler_init(IpcamHttpVideoParamHandler *self)
{
    ipcam_http_request_handler_register(IPCAM_HTTP_REQUEST_HANDLER(self), get_video_param);
    ipcam_http_request_handler_register(IPCAM_HTTP_REQUEST_HANDLER(self), put_video_param);
}

static void ipcam_http_video_param_handler_class_init(IpcamHttpVideoParamHandlerClass *klass)
{
}

static void destroy_data(gpointer data)
{
    g_free(data);
}
