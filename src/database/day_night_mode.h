#ifndef __DAY_NIGHT_MODE_H__
#define __DAY_NIGHT_MODE_H__

#include <gom/gom.h>

#define IPCAM_DAY_NIGHT_MODE_TYPE (ipcam_day_night_mode_get_type())
#define IPCAM_DAY_NIGHT_MODE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_DAY_NIGHT_MODE_TYPE, IpcamDayNightMode))
#define IPCAM_DAY_NIGHT_MODE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_DAY_NIGHT_MODE_TYPE, IpcamDayNightModeClass))
#define IPCAM_IS_DAY_NIGHT_MODE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_DAY_NIGHT_MODE_TYPE))
#define IPCAM_IS_DAY_NIGHT_MODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_DAY_NIGHT_MODE_TYPE))
#define IPCAM_DAY_NIGHT_MODE_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_DAY_NIGHT_MODE_TYPE, IpcamDayNightModeClass))

typedef struct _IpcamDayNightMode IpcamDayNightMode;
typedef struct _IpcamDayNightModeClass IpcamDayNightModeClass;

struct _IpcamDayNightMode
{
    GomResource parent;
};

struct _IpcamDayNightModeClass
{
    GomResourceClass parent_class;
};

GType ipcam_day_night_mode_get_type(void);

#endif /* __DAY_NIGHT_MODE_H__ */
