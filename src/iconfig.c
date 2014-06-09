#include <stdlib.h>
#include <json-glib/json-glib.h>
#include "iconfig.h"
#include "database/database.h"
#include "ajax/ajax.h"

typedef struct _IpcamIConfigPrivate
{
    IpcamDatabase *database;
    IpcamAjax *ajax;
} IpcamIConfigPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamIConfig, ipcam_iconfig, IPCAM_BASE_APP_TYPE);

static void ipcam_iconfig_before_start(IpcamIConfig *iconfig);
static void ipcam_iconfig_in_loop(IpcamIConfig *iconfig);

static void ipcam_iconfig_finalize(GObject *object)
{
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(IPCAM_ICONFIG(object));
    g_clear_object(&priv->database);
    g_clear_object(&priv->ajax);
    G_OBJECT_CLASS(ipcam_iconfig_parent_class)->finalize(object);
}
static void ipcam_iconfig_init(IpcamIConfig *self)
{
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(self);
    priv->database = g_object_new(IPCAM_DATABASE_TYPE, NULL);
}
static void ipcam_iconfig_class_init(IpcamIConfigClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = &ipcam_iconfig_finalize;
    
    IpcamBaseServiceClass *base_service_class = IPCAM_BASE_SERVICE_CLASS(klass);
    base_service_class->before = &ipcam_iconfig_before_start;
    base_service_class->in_loop = &ipcam_iconfig_in_loop;
}
static void ipcam_iconfig_before_start(IpcamIConfig *iconfig)
{
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);
    gchar *ajax_addr = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "ajax:address");
    gchar *port = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "ajax:port");
    if (ajax_addr != NULL && port != NULL)
    {
        priv->ajax = g_object_new(IPCAM_AJAX_TYPE,
                                  "app", iconfig,
                                  "address", ajax_addr,
                                  "port", strtol(port, NULL, 10),
                                  NULL);
    }
    /* SO SURPRISED, it is cause segment fault when called
       ipcam_iconfig_get_base_info in other thread if delete
       code below. */
    GList *infos_list = NULL;
    infos_list = g_list_append(infos_list, "device_name");
    gchar *infos = ipcam_iconfig_get_base_info(iconfig, infos_list);
    g_free(infos);
    g_list_free(infos_list);

	infos_list = NULL;
	infos_list = g_list_append(infos_list, "datetime");
	gchar *osds = ipcam_iconfig_get_osd(iconfig, infos_list);
	g_free(osds);
	g_list_free(infos_list);
}

static void ipcam_iconfig_in_loop(IpcamIConfig *iconfig)
{
}

gchar *ipcam_iconfig_get_base_info(IpcamIConfig *iconfig, GList *infos)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    JsonBuilder *builder = json_builder_new();
    JsonBuilder *infos_builder = json_builder_new();
    JsonGenerator *generator = json_generator_new();
    GList *item = g_list_first(infos);
    gchar *value;

    json_builder_begin_object(builder);
    for (; item; item = g_list_next(item))
    {
        json_builder_set_member_name(builder, item->data);
        value = ipcam_database_get_baseinfo(priv->database, item->data);
        json_builder_add_string_value(builder, value);
        g_free(value);
    }
    json_builder_end_object(builder);

    json_builder_begin_object(infos_builder);
    json_builder_set_member_name(infos_builder, "infos");
    JsonNode *root = json_builder_get_root(builder);
    json_builder_add_value(infos_builder, root);
    json_builder_end_object(infos_builder);
    JsonNode *infos_root = json_builder_get_root(infos_builder);
    json_generator_set_root(generator, infos_root);
    json_generator_set_pretty(generator, FALSE);
    
    const gchar *string = json_generator_to_data(generator, NULL);
    json_node_free(infos_root);
    g_object_unref(generator);
    g_object_unref(infos_builder);
    g_object_unref(builder);
    return string;
}

void ipcam_iconfig_set_base_info(IpcamIConfig *iconfig, GHashTable *infos)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);
    gchar *value = NULL;

    value = g_hash_table_lookup(infos, "device_name");
    if (value)
    {
        ipcam_database_set_baseinfo(priv->database, "device_name", value);
    }

    value = g_hash_table_lookup(infos, "comment");
    if (value)
    {
        ipcam_database_set_baseinfo(priv->database, "comment", value);
    }
}

gchar *ipcam_iconfig_get_osd(IpcamIConfig *iconfig, GList *infos)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    JsonBuilder *builder = json_builder_new();
    JsonBuilder *infos_builder = json_builder_new();
    JsonGenerator *generator = json_generator_new();
    GList *item = g_list_first(infos);

    json_builder_begin_array(builder);
    for (; item; item = g_list_next(item))
    {
		gboolean isshow;
		guint size;
		guint x, y;
		guint color;

        if (ipcam_database_get_osd(priv->database, item->data, &isshow, &size, &x, &y, &color)) {
			json_builder_begin_object (builder);

			json_builder_set_member_name (builder, "name");
			json_builder_add_string_value(builder, item->data);
			json_builder_set_member_name(builder, "isshow");
			json_builder_add_boolean_value(builder, isshow);
			json_builder_set_member_name(builder, "size");
			json_builder_add_int_value (builder, (gint64)size);
			json_builder_set_member_name(builder, "x");
			json_builder_add_int_value (builder, (gint64)x);
			json_builder_set_member_name(builder, "y");
			json_builder_add_int_value (builder, (gint64)y);
			json_builder_set_member_name(builder, "color");
			json_builder_add_int_value (builder, (gint64)color);

			json_builder_end_object (builder);
		}
    }
    json_builder_end_array(builder);

    json_builder_begin_object(infos_builder);
    json_builder_set_member_name(infos_builder, "osds");
    JsonNode *root = json_builder_get_root(builder);
    json_builder_add_value(infos_builder, root);
    json_builder_end_object(infos_builder);
    JsonNode *infos_root = json_builder_get_root(infos_builder);
    json_generator_set_root(generator, infos_root);
    json_generator_set_pretty(generator, FALSE);
    
    const gchar *string = json_generator_to_data(generator, NULL);
    json_node_free(infos_root);
    g_object_unref(generator);
    g_object_unref(infos_builder);
    g_object_unref(builder);
    return string;
}

void ipcam_iconfig_set_osd(IpcamIConfig *iconfig, gchar *name, gboolean isshow,
                           guint size, guint x, guint y, guint color)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);
    gchar *value = NULL;

	ipcam_database_set_osd(priv->database, name, isshow, size, x, y, color);
}
