#ifndef __MISC_H__
#define __MISC_H__

#include <gom/gom.h>

#define IPCAM_MISC_TYPE (ipcam_misc_get_type())
#define IPCAM_MISC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_MISC_TYPE, IpcamMisc))
#define IPCAM_MISC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_MISC_TYPE, IpcamMiscClass))
#define IPCAM_IS_MISC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_MISC_TYPE))
#define IPCAM_IS_MISC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_MISC_TYPE))
#define IPCAM_MISC_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_MISC_TYPE, IpcamMiscClass))

typedef struct _IpcamMisc IpcamMisc;
typedef struct _IpcamMiscClass IpcamMiscClass;

struct _IpcamMisc
{
    GomResource parent;
};

struct _IpcamMiscClass
{
    GomResourceClass parent_class;
};

GType ipcam_misc_get_type(void);

#endif /* __MISC_H__ */
