#ifndef __EVENT_OUTPUT_H__
#define __EVENT_OUTPUT_H__

#include <gom/gom.h>

#define IPCAM_EVENT_OUTPUT_TYPE (ipcam_event_output_get_type())
#define IPCAM_EVENT_OUTPUT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_EVENT_OUTPUT_TYPE, IpcamEventOutput))
#define IPCAM_EVENT_OUTPUT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_EVENT_OUTPUT_TYPE, IpcamEventOutputClass))
#define IPCAM_IS_EVENT_OUTPUT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_EVENT_OUTPUT_TYPE))
#define IPCAM_IS_EVENT_OUTPUT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_EVENT_OUTPUT_TYPE))
#define IPCAM_EVENT_OUTPUT_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_EVENT_OUTPUT_TYPE, IpcamEventOutputClass))

typedef struct _IpcamEventOutput IpcamEventOutput;
typedef struct _IpcamEventOutputClass IpcamEventOutputClass;

struct _IpcamEventOutput
{
    GomResource parent;
};

struct _IpcamEventOutputClass
{
    GomResourceClass parent_class;
};

GType ipcam_event_output_get_type(void);

#endif /* __EVENT_OUTPUT_H__ */
