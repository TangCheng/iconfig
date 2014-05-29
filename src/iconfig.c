#include <stdlib.h>
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
}
static void ipcam_iconfig_in_loop(IpcamIConfig *iconfig)
{
}
