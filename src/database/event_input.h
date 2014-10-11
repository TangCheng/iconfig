#ifndef __EVENT_INPUT_H__
#define __EVENT_INPUT_H__

#include <gom/gom.h>

#define IPCAM_EVENT_INPUT_TYPE (ipcam_event_input_get_type())
#define IPCAM_EVENT_INPUT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_EVENT_INPUT_TYPE, IpcamEventInput))
#define IPCAM_EVENT_INPUT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_EVENT_INPUT_TYPE, IpcamEventInputClass))
#define IPCAM_IS_EVENT_INPUT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_EVENT_INPUT_TYPE))
#define IPCAM_IS_EVENT_INPUT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_EVENT_INPUT_TYPE))
#define IPCAM_EVENT_INPUT_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_EVENT_INPUT_TYPE, IpcamEventInputClass))

typedef struct _IpcamEventInput IpcamEventInput;
typedef struct _IpcamEventInputClass IpcamEventInputClass;

struct _IpcamEventInput
{
    GomResource parent;
};

struct _IpcamEventInputClass
{
    GomResourceClass parent_class;
};

GType ipcam_event_input_get_type(void);

#endif /* __EVENT_INPUT_H__ */
