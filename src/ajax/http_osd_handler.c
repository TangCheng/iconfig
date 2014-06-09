#include "http_osd_handler.h"
#include "http_request.h"
#include "http_response.h"
#include "http_query_string_parser.h"
#include "iconfig.h"

G_DEFINE_TYPE(IpcamHttpOsdHandler, ipcam_http_osd_handler, IPCAM_HTTP_REQUEST_HANDLER_TYPE)

static gchar *json_get_value(JsonNode *json, const gchar *key);
static void destroy_data(gpointer data);

START_HANDLER(get_osd, HTTP_GET, "/api/1.0/osd.json", http_request, http_response)
{
    IpcamIConfig *iconfig;
    IpcamHttpQueryStringParser *parser;
    gchar *query_string = NULL;
    GList *infos_list = NULL;
    GHashTable *query_hash = NULL;
    gboolean success = FALSE;
    
    g_object_get(get_osd, "app", &iconfig, NULL);
    g_object_get(http_request, "query-string", &query_string, NULL);
    if (query_string) 
    {
        parser = g_object_new(IPCAM_HTTP_QUERY_STRING_PARSER_TYPE, NULL);
        query_hash = ipcam_http_query_string_parser_get(parser, query_string);
        if (query_hash)
        {
            infos_list = g_hash_table_lookup(query_hash, "osd_names[]");
            if (infos_list)
            {
                gchar *infos = ipcam_iconfig_get_osd(iconfig, infos_list);
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

START_HANDLER(put_osd, HTTP_PUT, "/api/1.0/osd.json", http_request, http_response)
{
    gchar *body = NULL;
    IpcamIConfig *iconfig;
    GHashTable *infos_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, destroy_data);
    gboolean success = FALSE;

    g_object_get(put_osd, "app", &iconfig, NULL);
    g_object_get(http_request, "body", &body, NULL);
    if (body)
    {
        JsonParser *parser = json_parser_new();
        JsonNode *jbody;
        gchar *value = NULL;
		GError *err = NULL;
        if (json_parser_load_from_data(parser, body, -1, &err))
        {
            jbody = json_parser_get_root(parser);
            value = json_get_value(jbody, "$.infos.device_name");
            if (value)
            {
                g_hash_table_insert(infos_hash, "device_name", value);
            }
            value = json_get_value(jbody, "$.infos.comment");
            if (value)
            {
                g_hash_table_insert(infos_hash, "comment", value);
            }
			ipcam_iconfig_set_osd(iconfig, infos_hash);
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

static void ipcam_http_osd_handler_init(IpcamHttpOsdHandler *self)
{
    ipcam_http_request_handler_register(IPCAM_HTTP_REQUEST_HANDLER(self), get_osd);
    ipcam_http_request_handler_register(IPCAM_HTTP_REQUEST_HANDLER(self), put_osd);
}

static void ipcam_http_osd_handler_class_init(IpcamHttpOsdHandlerClass *klass)
{
}

static gchar *json_get_value(JsonNode *json, const gchar *key)
{
    gchar *str = NULL;
    JsonNode *jattr = json_path_query(key, json, NULL);
    JsonArray *array = json_node_get_array(jattr);
    if (json_array_get_length(array) > 0)
    {
        str = g_strdup(json_array_get_string_element(array, 0));
    }
    json_node_free(jattr);
    return str;
}
static void destroy_data(gpointer data)
{
    g_free(data);
}
