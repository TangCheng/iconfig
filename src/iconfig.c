
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <json-glib/json-glib.h>
#include "iconfig.h"
#include "database/database.h"
#include "ajax/ajax.h"
#include "action-handler/generic_action_handler.h"
#include "common.h"

typedef struct _IpcamIConfigPrivate
{
    IpcamDatabase *database;
    IpcamAjax *ajax;
} IpcamIConfigPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamIConfig, ipcam_iconfig, IPCAM_BASE_APP_TYPE);

static void ipcam_iconfig_before_start(IpcamBaseService *base_service);
static void ipcam_iconfig_in_loop(IpcamBaseService *base_service);

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
    base_service_class->before = ipcam_iconfig_before_start;
    base_service_class->in_loop = ipcam_iconfig_in_loop;
}

static void ipcam_iconfig_before_start(IpcamBaseService *base_service)
{
    IpcamIConfig *iconfig = IPCAM_ICONFIG(base_service);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);
    const gchar *ajax_addr = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "ajax:address");
    const gchar *port = ipcam_base_app_get_config(IPCAM_BASE_APP(iconfig), "ajax:port");
    if (ajax_addr != NULL && port != NULL)
    {
        priv->ajax = g_object_new(IPCAM_AJAX_TYPE,
                                  "app", iconfig,
                                  "address", ajax_addr,
                                  "port", strtol(port, NULL, 10),
                                  NULL);
    }

    /* Message Handler */
    const gchar *name[] =
    {
        "get_misc",
        "set_misc",    
        "get_base_info",
        "set_base_info",
        "get_users",
        "set_users",
        "add_users",
        "del_users",
        "get_datetime",
        "set_datetime",
        "get_video",
        "set_video",
        "get_image",
        "set_image",
        "get_privacy_block",
        "set_privacy_block",
        "get_day_night_mode",
        "set_day_night_mode",
        "get_osd",
        "set_osd",
        "get_szyc",
        "set_szyc",
        "get_network",
        "set_network",
        "get_event_input",
        "set_event_input",
        "get_event_output",
        "set_event_output"
    };

    gint i = 0;
    for (i = 0; i < ARRAY_SIZE(name); i++)
    {
        ipcam_base_app_register_request_handler(IPCAM_BASE_APP(iconfig), name[i], IPCAM_GENERIC_ACTION_HANDLER_TYPE);
    }
}

static void ipcam_iconfig_in_loop(IpcamBaseService *base_service)
{
}

gchar *ipcam_iconfig_get_base_info(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_baseinfo(priv->database, (gchar *)name);
}

void ipcam_iconfig_set_base_info(IpcamIConfig *iconfig, const char *name, const char *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_baseinfo(priv->database, (gchar *)name, (gchar *)value);
}

gboolean ipcam_iconfig_get_osd(IpcamIConfig *iconfig,
                               const gchar *name,
                               gboolean *isshow,
                               guint *size,
                               guint *left,
                               guint *top,
                               guint *color)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), FALSE);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_osd(priv->database, (gchar *)name, isshow, size, left, top, color);
}

void ipcam_iconfig_set_osd(IpcamIConfig *iconfig,
                            const gchar *name,
                            gboolean isshow,
                            guint size,
                            guint left,
                            guint top,
                            guint color)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_osd(priv->database, (gchar *)name, isshow, size, left, top, color);
}

GVariant *ipcam_iconfig_get_video(IpcamIConfig *iconfig,
                                  const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    GVariant *ret = ipcam_database_get_video(priv->database, name);
	return ret;
}

void ipcam_iconfig_set_video(IpcamIConfig *iconfig,
                             const gchar *name,
                             GVariant *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);
    
	ipcam_database_set_video(priv->database, name, value);
}

GVariant *ipcam_iconfig_get_image(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_image(priv->database, (gchar *)name);
}

void ipcam_iconfig_set_image(IpcamIConfig *iconfig, const gchar *name, GVariant *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

	ipcam_database_set_image(priv->database, (gchar *)name, value);
}

gchar *ipcam_iconfig_get_network(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), -1);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_network(priv->database, name);
}

void ipcam_iconfig_set_network(IpcamIConfig *iconfig, const gchar *name, const gchar *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_network(priv->database, name, value);
}

gchar *ipcam_iconfig_get_network_static(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_network_static(priv->database, name);
}

void ipcam_iconfig_set_network_static(IpcamIConfig *iconfig, const gchar *name, gchar *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_network_static(priv->database, name, value);
}

gchar *ipcam_iconfig_get_network_pppoe(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_network_pppoe(priv->database, name);
}

void ipcam_iconfig_set_network_pppoe(IpcamIConfig *iconfig, const gchar *name, gchar *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_network_pppoe(priv->database, name, value);
}

gint ipcam_iconfig_get_network_port(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), -1);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_network_port(priv->database, name);
}

void ipcam_iconfig_set_network_port(IpcamIConfig *iconfig, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_network_port(priv->database, name, value);
}

void ipcam_iconfig_set_datetime(IpcamIConfig *iconfig, const gchar *name, const GVariant *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_datetime (priv->database, name, value);
}

GVariant *ipcam_iconfig_get_datetime(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_datetime (priv->database, name);
}

GList *ipcam_iconfig_get_users(IpcamIConfig *iconfig)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_users (priv->database);
}

void ipcam_iconfig_set_user_password(IpcamIConfig *iconfig, const gchar *username, gchar *password)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_user_password (priv->database, username, password);
}

gchar *ipcam_iconfig_get_user_password(IpcamIConfig *iconfig, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_user_password (priv->database, username);
}

void ipcam_iconfig_set_user_role(IpcamIConfig *iconfig, const gchar *username, const gchar *role)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

	ipcam_database_set_user_role (priv->database, username, role);
}

gchar *ipcam_iconfig_get_user_role(IpcamIConfig *iconfig, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_user_role (priv->database, username);
}

gboolean ipcam_iconfig_add_user(IpcamIConfig *iconfig, const gchar *username,
                                const gchar *password, const gchar *role)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), FALSE);
    g_return_val_if_fail(username, FALSE);
    g_return_val_if_fail(password, FALSE);
    g_return_val_if_fail(role, FALSE);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_add_user(priv->database, username, password, role);
}

gboolean ipcam_iconfig_del_user(IpcamIConfig *iconfig, const gchar *username)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_del_user (priv->database, username);
}

void ipcam_iconfig_set_misc(IpcamIConfig *iconfig, const gchar *name, const GVariant *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_misc (priv->database, name, value);
}

GVariant *ipcam_iconfig_get_misc(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_misc (priv->database, name);
}

gboolean ipcam_iconfig_get_privacy_block(IpcamIConfig *iconfig, const char *name, gboolean *enabled,
                                         guint *left, guint *top, guint *width, guint *height, guint *color)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), FALSE);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_privacy_block(priv->database, (gchar *)name, enabled,
                                            left, top, width, height, color);
}

void ipcam_iconfig_set_privacy_block(IpcamIConfig *iconfig, const gchar *name, gboolean enabled,
                                     guint left, guint top, guint width, guint height, guint color)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_privacy_block(priv->database, (gchar *)name, enabled,
                                     left, top, width, height, color);
}

gint ipcam_iconfig_get_day_night_mode(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), -1);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_day_night_mode(priv->database, name);
}

void ipcam_iconfig_set_day_night_mode(IpcamIConfig *iconfig, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_set_day_night_mode(priv->database, name, value);
}

gchar *ipcam_iconfig_get_szyc(IpcamIConfig *iconfig, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), -1);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_get_szyc(priv->database, name);
}

void ipcam_iconfig_set_szyc(IpcamIConfig *iconfig, const gchar *name, const gchar *value)
{
    g_return_if_fail(IPCAM_IS_ICONFIG(iconfig));
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    ipcam_database_set_szyc(priv->database, name, value);
}
GVariant *ipcam_iconfig_read(IpcamIConfig *iconfig, GType table, const gchar *name, const gchar *sub_name)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), NULL);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_read(priv->database, table, name, sub_name);
}

gboolean ipcam_iconfig_update(IpcamIConfig *iconfig, GType table, const gchar *name, const gchar *sub_name,  GVariant *value)
{
    g_return_val_if_fail(IPCAM_IS_ICONFIG(iconfig), FALSE);
    IpcamIConfigPrivate *priv = ipcam_iconfig_get_instance_private(iconfig);

    return ipcam_database_update(priv->database, table, name, sub_name, value);
}

