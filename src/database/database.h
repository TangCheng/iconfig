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
GList *ipcam_database_get_users(IpcamDatabase *database);
void ipcam_database_set_user_password(IpcamDatabase *database, const gchar *username, gchar *password);
gchar *ipcam_database_get_user_password(IpcamDatabase *database, const gchar *username);
void ipcam_database_set_user_role(IpcamDatabase *database, const gchar *username, const gchar *role);
gchar *ipcam_database_get_user_role(IpcamDatabase *database, const gchar *username);
gboolean ipcam_database_add_user(IpcamDatabase *database, const gchar *username,
                                 const gchar *password, const gchar *role);
gboolean ipcam_database_del_user(IpcamDatabase *database, const gchar *username);
gboolean ipcam_database_update(IpcamDatabase *database, GType table, const gchar *name, const gchar *sub_name, GVariant *value);
GVariant *ipcam_database_read(IpcamDatabase *database, GType table, const gchar *name, const gchar *sub_name);

#endif /* __DATABASE_H__ */
