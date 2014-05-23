#ifndef __OSD_H__
#define __OSD_H__

#include <gom/gom.h>

#define IPCAM_OSD_TYPE (ipcam_osd_get_type())
#define IPCAM_OSD(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_OSD_TYPE, IpcamOsd))
#define IPCAM_OSD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_OSD_TYPE, IpcamOsdClass))
#define IPCAM_IS_OSD(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_OSD_TYPE))
#define IPCAM_IS_OSD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_OSD_TYPE))
#define IPCAM_OSD_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_OSD_TYPE, IpcamOsdClass))

typedef struct _IpcamOsd IpcamOsd;
typedef struct _IpcamOsdClass IpcamOsdClass;

struct _IpcamOsd
{
    GomResource parent;
};

struct _IpcamOsdClass
{
    GomResourceClass parent_class;
};

GType ipcam_osd_get_type(void);

#endif /* __OSD_H__ */
