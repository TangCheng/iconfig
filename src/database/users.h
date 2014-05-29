#ifndef __USERS_H__
#define __USERS_H__

#include <gom/gom.h>

#define IPCAM_USERS_TYPE (ipcam_users_get_type())
#define IPCAM_USERS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_USERS_TYPE, IpcamUsers))
#define IPCAM_USERS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_USERS_TYPE, IpcamUsersClass))
#define IPCAM_IS_USERS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_USERS_TYPE))
#define IPCAM_IS_USERS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_USERS_TYPE))
#define IPCAM_USERS_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_USERS_TYPE, IpcamUsersClass))

typedef struct _IpcamUsers IpcamUsers;
typedef struct _IpcamUsersClass IpcamUsersClass;

struct _IpcamUsers
{
    GomResource parent;
};

struct _IpcamUsersClass
{
    GomResourceClass parent_class;
};

GType ipcam_users_get_type(void);

#endif /* __USERS_H__ */
