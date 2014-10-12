#ifndef __EVENT_PROC_H__
#define __EVENT_PROC_H__

#include <gom/gom.h>

#define IPCAM_EVENT_PROC_TYPE (ipcam_event_proc_get_type())
#define IPCAM_EVENT_PROC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_EVENT_PROC_TYPE, IpcamEventProc))
#define IPCAM_EVENT_PROC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_EVENT_PROC_TYPE, IpcamEventProcClass))
#define IPCAM_IS_EVENT_PROC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_EVENT_PROC_TYPE))
#define IPCAM_IS_EVENT_PROC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_EVENT_PROC_TYPE))
#define IPCAM_EVENT_PROC_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_EVENT_PROC_TYPE, IpcamEventProcClass))

typedef struct _IpcamEventProc IpcamEventProc;
typedef struct _IpcamEventProcClass IpcamEventProcClass;

struct _IpcamEventProc
{
    GomResource parent;
};

struct _IpcamEventProcClass
{
    GomResourceClass parent_class;
};

GType ipcam_event_proc_get_type(void);

#endif /* __EVENT_PROC_H__ */
