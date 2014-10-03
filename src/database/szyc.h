#ifndef __SZYC_H__
#define __SZYC_H__

#include <gom/gom.h>

#define IPCAM_SZYC_TYPE (ipcam_szyc_get_type())
#define IPCAM_SZYC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_SZYC_TYPE, IpcamSzyc))
#define IPCAM_SZYC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_SZYC_TYPE, IpcamSzycClass))
#define IPCAM_IS_SZYC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_SZYC_TYPE))
#define IPCAM_IS_SZYC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_SZYC_TYPE))
#define IPCAM_SZYC_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_SZYC_TYPE, IpcamSzycClass))

typedef struct _IpcamSzyc IpcamSzyc;
typedef struct _IpcamSzycClass IpcamSzycClass;

struct _IpcamSzyc
{
    GomResource parent;
};

struct _IpcamSzycClass
{
    GomResourceClass parent_class;
};

GType ipcam_szyc_get_type(void);

#endif /* __SZYC_H__ */
