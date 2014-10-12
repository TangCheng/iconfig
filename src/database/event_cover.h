#ifndef __EVENT_COVER_H__
#define __EVENT_COVER_H__

#include <gom/gom.h>

#define IPCAM_EVENT_COVER_TYPE (ipcam_event_cover_get_type())
#define IPCAM_EVENT_COVER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_EVENT_COVER_TYPE, IpcamEventCover))
#define IPCAM_EVENT_COVER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_EVENT_COVER_TYPE, IpcamEventCoverClass))
#define IPCAM_IS_EVENT_COVER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_EVENT_COVER_TYPE))
#define IPCAM_IS_EVENT_COVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_EVENT_COVER_TYPE))
#define IPCAM_EVENT_COVER_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_EVENT_COVER_TYPE, IpcamEventCoverClass))

typedef struct _IpcamEventCover IpcamEventCover;
typedef struct _IpcamEventCoverClass IpcamEventCoverClass;

struct _IpcamEventCover
{
    GomResource parent;
};

struct _IpcamEventCoverClass
{
    GomResourceClass parent_class;
};

GType ipcam_event_cover_get_type(void);

#endif /* __EVENT_COVER_H__ */
