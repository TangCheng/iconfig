#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <gom/gom.h>

#define IPCAM_IMAGE_TYPE (ipcam_image_get_type())
#define IPCAM_IMAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_IMAGE_TYPE, IpcamImage))
#define IPCAM_IMAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_IMAGE_TYPE, IpcamImageClass))
#define IPCAM_IS_IMAGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_IMAGE_TYPE))
#define IPCAM_IS_IMAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_IMAGE_TYPE))
#define IPCAM_IMAGE_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_IMAGE_TYPE, IpcamImageClass))

typedef struct _IpcamImage IpcamImage;
typedef struct _IpcamImageClass IpcamImageClass;

struct _IpcamImage
{
    GomResource parent;
};

struct _IpcamImageClass
{
    GomResourceClass parent_class;
};

GType ipcam_image_get_type(void);

#endif /* __IMAGE_H__ */
