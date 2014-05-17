#ifndef __BASE_INFO_H__
#define __BASE_INFO_H__

#include <gom/gom.h>

#define IPCAM_BASE_INFO_TYPE (ipcam_base_info_get_type())
#define IPCAM_BASE_INFO(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_BASE_INFO_TYPE, IpcamBaseInfo))
#define IPCAM_BASE_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_BASE_INFO_TYPE, IpcamBaseInfoClass))
#define IPCAM_IS_BASE_INFO(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_BASE_INFO_TYPE))
#define IPCAM_IS_BASE_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_BASE_INFO_TYPE))
#define IPCAM_BASE_INFO_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((ojb), IPCAM_BASE_INFO_TYPE, IpcamBaseInfoClass))

typedef struct _IpcamBaseInfo IpcamBaseInfo;
typedef struct _IpcamBaseInfoClass IpcamBaseInfoClass;

struct _IpcamBaseInfo
{
    GomResource parent;
};

struct _IpcamBaseInfoClass
{
    GomResourceClass parent_class;
};

GType ipcam_base_info_get_type(void);

#endif /* __BASE_INFO_H__ */
