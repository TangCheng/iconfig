#include "iconfig.h"

G_DEFINE_TYPE(IpcamIConfig, ipcam_iconfig, IPCAM_BASE_APP_TYPE);

static void ipcam_iconfig_before_start(IpcamIConfig *iconfig);
static void ipcam_iconfig_in_loop(IpcamIConfig *iconfig);

static void ipcam_iconfig_init(IpcamIConfig *self)
{
}
static void ipcam_iconfig_class_init(IpcamIConfigClass *klass)
{
    IpcamBaseServiceClass *base_service_class = IPCAM_BASE_SERVICE_CLASS(klass);
    base_service_class->before = &ipcam_iconfig_before_start;
    base_service_class->in_loop = &ipcam_iconfig_in_loop;
}
static void ipcam_iconfig_before_start(IpcamIConfig *iconfig)
{
}
static void ipcam_iconfig_in_loop(IpcamIConfig *iconfig)
{
}
