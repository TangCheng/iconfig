#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <glib.h>
#include <glib-object.h>

#define IPCAM_DATABASE_TYPE (ipcam_database_get_type())
#define IPCAM_DATABASE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_DATABASE_TYPE, IpcamDatabase))
#define IPCAM_DATABASE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_DATABASE_TYPE, IpcamDatabaseClass))
#define IPCAM_IS_DATABASE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_DATABASE_TYPE))
#define IPCAM_IS_DATABASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_DATABASE_TYPE))
#define IPCAM_DATABASE_GET_CLASSS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_DATABASE_TYPE, IpcamDatabaseClass))

typedef struct _IpcamDatabase IpcamDatabase;
typedef struct _IpcamDatabaseClass IpcamDatabaseClass;

struct _IpcamDatabase
{
    GObject parent;
};

struct _IpcamDatabaseClass
{
    GObjectClass parent_class;
};

GType ipcam_database_get_type(void);
void ipcam_database_set_baseinfo(IpcamDatabase *database, gchar *name, gchar *value);
gchar *ipcam_database_get_baseinfo(IpcamDatabase *database, gchar *name);
GList *ipcam_database_get_users(IpcamDatabase *database);
void ipcam_database_set_user_password(IpcamDatabase *database, gchar *username, gchar *password);
gchar *ipcam_database_get_user_password(IpcamDatabase *database, gchar *username);
void ipcam_database_set_user_privilege(IpcamDatabase *database, gchar *username, gboolean isadmin);
gboolean ipcam_database_get_user_privilege(IpcamDatabase *database, gchar *username);

#endif /* __DATABASE_H__ */
