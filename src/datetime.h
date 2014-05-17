#ifndef __DATETIME_H__
#define __DATETIME_H__

#include <gom/gom.h>

#define IPCAM_DATETIME_TYPE (ipcam_datetime_get_type())
#define IPCAM_DATETIME(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_DATETIME_TYPE, IpcamDatetime))
#define IPCAM_DATETIME_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_DATETIME_TYPE, IpcamDatetimeClass))
#define IPCAM_IS_DATETIME(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_DATETIME_TYPE))
#define IPCAM_IS_DATETIME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_DATETIME_TYPE))
#define IPCAM_DATETIME_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_DATETIME_TYPE, IpcamDatetimeClass))

typedef struct _IpcamDatetime IpcamDatetime;
typedef struct _IpcamDatetimeClass IpcamDatetimeClass;

struct _IpcamDatetime
{
    GomResource parent;
};

struct _IpcamDatetimeClass
{
    GomResourceClass parent_class;
};

GType ipcam_datetime_get_type(void);

#endif /* __DATETIME_H__ */
