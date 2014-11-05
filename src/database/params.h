#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <gom/gom.h>

#define IPCAM_PARAMS_TYPE (ipcam_params_get_type())
#define IPCAM_PARAMS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_PARAMS_TYPE, IpcamParams))
#define IPCAM_PARAMS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_PARAMS_TYPE, IpcamParamsClass))
#define IPCAM_IS_PARAMS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_PARAMS_TYPE))
#define IPCAM_IS_PARAMS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_PARAMS_TYPE))
#define IPCAM_PARAMS_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_PARAMS_TYPE, IpcamParamsClass))

typedef struct _IpcamParams IpcamParams;
typedef struct _IpcamParamsClass IpcamParamsClass;

struct _IpcamParams
{
    GomResource parent;
};

struct _IpcamParamsClass
{
    GomResourceClass parent_class;
};

GType ipcam_params_get_type(void);

#endif /* __PARAMS_H__ */
