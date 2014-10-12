#ifndef __EVENT_MOTION_H__
#define __EVENT_MOTION_H__

#include <gom/gom.h>

#define IPCAM_EVENT_MOTION_TYPE (ipcam_event_motion_get_type())
#define IPCAM_EVENT_MOTION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_EVENT_MOTION_TYPE, IpcamEventMotion))
#define IPCAM_EVENT_MOTION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_EVENT_MOTION_TYPE, IpcamEventMotionClass))
#define IPCAM_IS_EVENT_MOTION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_EVENT_MOTION_TYPE))
#define IPCAM_IS_EVENT_MOTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_EVENT_MOTION_TYPE))
#define IPCAM_EVENT_MOTION_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_EVENT_MOTION_TYPE, IpcamEventMotionClass))

typedef struct _IpcamEventMotion IpcamEventMotion;
typedef struct _IpcamEventMotionClass IpcamEventMotionClass;

struct _IpcamEventMotion
{
    GomResource parent;
};

struct _IpcamEventMotionClass
{
    GomResourceClass parent_class;
};

GType ipcam_event_motion_get_type(void);

#endif /* __EVENT_MOTION_H__ */
