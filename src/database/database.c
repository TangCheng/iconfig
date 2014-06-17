#include <gom/gom.h>
#include "database.h"
#include "base_info.h"
#include "users.h"
#include "osd.h"
#include "video.h"
#include "scene.h"
#include "network.h"
#include "network_static.h"
#include "network_pppoe.h"
#include "network_port.h"
#include "datetime.h"

#define DATABASE_PATH_NAME "/data/configuration.sqlite3"

typedef struct _IpcamDatabasePrivate
{
    GomRepository *repository;
    GomAdapter *adapter;
} IpcamDatabasePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamDatabase, ipcam_database, G_TYPE_OBJECT);

static void ipcam_database_open(IpcamDatabase *database);

static void ipcam_database_finalize (GObject *object)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(IPCAM_DATABASE(object));
    g_object_unref(priv->repository);
    g_object_unref(priv->adapter);
    G_OBJECT_CLASS(ipcam_database_parent_class)->finalize(object);
}
static void ipcam_database_init(IpcamDatabase *self)
{
    ipcam_database_open(self);
}
static void ipcam_database_class_init(IpcamDatabaseClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = &ipcam_database_finalize;
}

static gboolean ipcam_database_migrator(GomRepository  *repository,
                                        GomAdapter     *adapter,
                                        guint           version,
                                        gpointer        user_data,
                                        GError        **error)
{
#define EXEC_OR_FAIL(sql)                                   \
    G_STMT_START {                                          \
        GomCommand *c = g_object_new(GOM_TYPE_COMMAND,      \
                                     "adapter", adapter,    \
                                     "sql", (sql),          \
                                     NULL);                 \
        if (!gom_command_execute(c, NULL, error)) {         \
            g_object_unref(c);                              \
            goto failure;                                   \
        }                                                   \
        g_object_unref(c);                                  \
    } G_STMT_END
    if (version == 1) {
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS base_info ("
                     "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name   TEXT UNIQUE NOT NULL,"
                     "value  TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value) "
                     "VALUES ('device_name', 'ipcam');");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value) "
                     "VALUES ('comment', '');");
        
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS users ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "password TEXT NOT NULL,"
                     "isadmin  BOOLEAN"
                     ");");
        EXEC_OR_FAIL("INSERT INTO users (name, password, isadmin) "
                     "VALUES ('admin', 'admin', 1);");
        
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS osd ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "isshow   BOOLEAN,"
                     "size     INTEGER,"
                     "x        INTEGER,"
                     "y        INTEGER,"
                     "color    INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, x, y, color) "
                     "VALUES ('datetime', 1, 5, 10, 20, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, x, y, color) "
                     "VALUES ('device_name', 1, 5, 10, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, x, y, color) "
                     "VALUES ('comment', 1, 5, 70, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, x, y, color) "
                     "VALUES ('frame_rate', 1, 5, 10, 80, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, x, y, color) "
                     "VALUES ('bit_rate', 1, 5, 10, 90, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS video ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('profile', 0);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('flip', 0);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('quanlity', 0);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('frame_rate', 25);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('bit_rate', 0);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('bit_rate_value', 300);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS scene ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO scene (name, value) "
                     "VALUES ('scenario', 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network (name, value) "
                     "VALUES ('method', 0);");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_static ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('ipaddr', '192.168.0.100');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('netmask', '255.255.255.0');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('gateway', '192.168.0.1');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('dns1', '');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('dns2', '');");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_pppoe ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_pppoe (name, value) "
                     "VALUES ('username', '');");
        EXEC_OR_FAIL("INSERT INTO network_pppoe (name, value) "
                     "VALUES ('password', '');");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_port ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    INTEGER UNIQUE NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_port (name, value) "
                     "VALUES ('http', 80);");
        EXEC_OR_FAIL("INSERT INTO network_port (name, value) "
                     "VALUES ('rtsp', 554);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS datetime ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "intvalue INTEGER,"
                     "strvalue TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO datetime (name, intvalue, strvalue) "
                     "VALUES ('timezone', 0, '');");
        EXEC_OR_FAIL("INSERT INTO datetime (name, intvalue, strvalue) "
                     "VALUES ('use_ntp', 0, '');");
        EXEC_OR_FAIL("INSERT INTO datetime (name, intvalue, strvalue) "
                     "VALUES ('ntp_server', 0, '');");
        
        return TRUE;
    }

failure:
    return FALSE;
}
static void ipcam_database_migrate(IpcamDatabase *database)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GError *error = NULL;
    gom_repository_migrate_sync(priv->repository, 1, ipcam_database_migrator, NULL, &error);
    if (error != NULL)
    {
        g_print("database migrate error: %s\n", error->message);
        g_error_free(error);
    }
}
static void ipcam_database_open(IpcamDatabase *database)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GError *error = NULL;
    gchar *uri = NULL;
    GFile *db = NULL;
    
    priv->adapter = gom_adapter_new();
    db = g_file_new_for_path(DATABASE_PATH_NAME);
    uri = g_file_get_uri(db);
    gom_adapter_open_sync(priv->adapter, uri, &error);
    g_free(uri);
    g_object_unref(db);

    if (error == NULL) 
    {
        priv->repository = gom_repository_new(priv->adapter);
        ipcam_database_migrate(database);
    }
    else
    {
        g_print("open apater error: %s\n", error->message);
        g_error_free(error);
    }
}

static GomResource *ipcam_database_get_resource(IpcamDatabase *database, GType resource_type, const gchar *name)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GValue value = { 0, };
    GomFilter *filter;
    GomResource *resource = NULL;
    GError *error = NULL;

    g_value_init(&value, G_TYPE_STRING);
    g_value_set_string(&value, name);
    filter = gom_filter_new_eq(resource_type, "name", &value);
    g_value_unset(&value);
    resource = gom_repository_find_one_sync(priv->repository,
                                            resource_type,
                                            filter,
                                            &error);
    if (error != NULL)
    {
        g_print("Get record error: %s\n", error->message);
        g_error_free(error);
    }
    g_object_unref(filter);
    
    return resource;
}
static GomResourceGroup *ipcam_database_get_resource_group(IpcamDatabase *database, GType resource_type)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);;
    GError *error = NULL;
    GomResourceGroup *resource_group = NULL;

    resource_group = gom_repository_find_sync(priv->repository,
                                              resource_type,
                                              NULL,
                                              &error);
    if (error)
    {
        g_print("Get record group error: %s\n", error->message);
        g_error_free(error);
    }
    
    return resource_group;
}
void ipcam_database_set_baseinfo(IpcamDatabase *database, const gchar *name, gchar *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_BASE_INFO_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("save base info error: %s\n", error->message);
        g_error_free(error);
    }
}
gchar *ipcam_database_get_baseinfo(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *value = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_BASE_INFO_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
GList *ipcam_database_get_users(IpcamDatabase *database)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResourceGroup *resource_group = NULL;
    GomResource *resource = NULL;
    guint count = 0;
    guint i = 0;
    GList *users_list = NULL;
    gchar *username;
    GError *error = NULL;

    resource_group = ipcam_database_get_resource_group(database, IPCAM_USERS_TYPE);
    if (resource_group)
    {
        count = gom_resource_group_get_count(resource_group);
        gom_resource_group_fetch_sync(resource_group, 0, count, &error);
        for (i = 0; i < count; i++)
        {
            resource = gom_resource_group_get_index(resource_group, i);
            g_object_get(resource, "name", &username, NULL);
            users_list = g_list_append(users_list, g_strdup(username));
            g_free(username);
        }
        g_object_unref(resource_group);

        if (error)
        {
            g_print("fetch data error: %s\n", error->message);
            g_error_free(error);
        }
    }

    return users_list;
}
void ipcam_database_set_user_password(IpcamDatabase *database, const gchar *username, gchar *password)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_set(resource, "password", password, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }
    else
    {
        resource = g_object_new(IPCAM_USERS_TYPE, "name", username, "password", password, NULL);
        IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
        g_object_set(resource, "repository", priv->repository, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("save users password error: %s\n", error->message);
        g_error_free(error);
    }
}
gchar *ipcam_database_get_user_password(IpcamDatabase *database, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *password = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_get(resource, "password", &password, NULL);
        g_object_unref(resource);
    }
    
    return password;
}
void ipcam_database_set_user_privilege(IpcamDatabase *database, const gchar *username, gboolean isadmin)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_set(resource, "isadmin", isadmin, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("save users privilege error: %s\n", error->message);
        g_error_free(error);
    }
}
gboolean ipcam_database_get_user_privilege(IpcamDatabase *database, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    gboolean isadmin = FALSE;

    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_get(resource, "isadmin", &isadmin, NULL);
        g_object_unref(resource);
    }
    
    return isadmin;
}
void ipcam_database_del_user(IpcamDatabase *database, const gchar *username)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        gom_resource_delete_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("delete user error: %s\n", error->message);
        g_error_free(error);
    }
}
void ipcam_database_set_osd(IpcamDatabase *database,
                            const gchar *name,
                            gboolean isshow,
                            guint size,
                            guint x,
                            guint y,
                            guint color)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_OSD_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "isshow", isshow, "size", size, "x", x, "y", y, "color", color, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set osd record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gboolean ipcam_database_get_osd(IpcamDatabase *database,
                                const gchar *name,
                                gboolean *isshow,
                                guint *size,
                                guint *x,
                                guint *y,
                                guint *color)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    gboolean ret = FALSE;
    
    resource = ipcam_database_get_resource(database, IPCAM_OSD_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "isshow", isshow, "size", size, "x", x, "y", y, "color", color, NULL);
        g_object_unref(resource);
        ret = TRUE;
    }
    
    return ret;
}
void ipcam_database_set_video(IpcamDatabase *database, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_VIDEO_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set video record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gint ipcam_database_get_video(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), -1);
    GomResource *resource = NULL;
    gint value = -1;
    
    resource = ipcam_database_get_resource(database, IPCAM_VIDEO_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_scene(IpcamDatabase *database, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_SCENE_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set scene record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gint ipcam_database_get_scene(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), -1);
    GomResource *resource = NULL;
    gint value = -1;
    
    resource = ipcam_database_get_resource(database, IPCAM_SCENE_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_network(IpcamDatabase *database, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set network record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gint ipcam_database_get_network(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), -1);
    GomResource *resource = NULL;
    gint value = -1;
    
    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_network_static(IpcamDatabase *database, const gchar *name, gchar *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_STATIC_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set network static record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gchar *ipcam_database_get_network_static(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_STATIC_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_network_pppoe(IpcamDatabase *database, const gchar *name, gchar *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_PPPOE_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set network pppoe record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gchar *ipcam_database_get_network_pppoe(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_PPPOE_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_network_port(IpcamDatabase *database, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_PORT_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set network port record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gint ipcam_database_get_network_port(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), -1);
    GomResource *resource = NULL;
    gint value = -1;
    
    resource = ipcam_database_get_resource(database, IPCAM_NETWORK_PORT_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_datetime(IpcamDatabase *database, const gchar *name, guint int_value, gchar *str_value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_DATETIME_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "intvalue", int_value, NULL);
        if (str_value)
        {
            g_object_set(resource, "strvalue", str_value, NULL);
        }
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set datetime record failed: %s\n", error->message);
        g_error_free(error);
    }
}
void ipcam_database_get_datetime(IpcamDatabase *database, const gchar *name, guint *int_value, gchar **str_value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_DATETIME_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "intvalue", int_value, "strvalue", str_value, NULL);
        g_object_unref(resource);
    }
}
