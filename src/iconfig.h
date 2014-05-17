#ifndef __ICONFIG_H__
#define __ICONFIG_H__

#include <base_app.h>

#define IPCAM_ICONFIG_TYPE (ipcam_iconfig_get_type())
#define IPCAM_ICONFIG(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_ICONFIG_TYPE, IpcamIConfig))
#define IPCAM_ICONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_ICONFIG_TYPE, IpcamIConfigClass))
#define IPCAM_IS_ICONFIG(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_ICONFIG_TYPE))
#define IPCAM_IS_ICONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_ICONFIG_TYPE))
#define IPCAM_ICONFIG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_ICONFIG_TYPE, IpcamIConfigClass))

typedef struct _IpcamIConfig IpcamIConfig;
typedef struct _IpcamIConfigClass IpcamIConfigClass;

struct _IpcamIConfig
{
    IpcamBaseApp parent;
};

struct _IpcamIConfigClass
{
    IpcamBaseAppClass parent_class;
};

GType ipcam_iconfig_get_type(void);

#endif /* __ICONFIG_H__ */
