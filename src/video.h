#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <gom/gom.h>

#define IPCAM_VIDEO_TYPE (ipcam_video_get_type())
#define IPCAM_VIDEO(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_VIDEO_TYPE, IpcamVideo))
#define IPCAM_VIDEO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_VIDEO_TYPE, IpcamVideoClass))
#define IPCAM_IS_VIDEO(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_VIDEO_TYPE))
#define IPCAM_IS_VIDEO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_VIDEO_TYPE))
#define IPCAM_VIDEO_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_VIDEO_TYPE, IpcamVideoClass))

typedef struct _IpcamVideo IpcamVideo;
typedef struct _IpcamVideoClass IpcamVideoClass;

struct _IpcamVideo
{
    GomResource parent;
};

struct _IpcamVideoClass
{
    GomResourceClass parent_class;
};

GType ipcam_video_get_type(void);

#endif /* __VIDEO_H__ */
