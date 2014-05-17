#include <gom/gom.h>
#include "database.h"

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
                     "_id    INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name   TEXT UNIQUE NOT NULL,"
                     "value  TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value) "
                     "VALUES ('device_name', 'ipcam');");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value) "
                     "VALUES ('comment', '');");
        
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS users ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "password  TEXT NOT NULL,"
                     "is_admin  BOOLEAN"
                     ");");
        EXEC_OR_FAIL("INSERT INTO users (name, password, is_admin) "
                     "VALUES ('admin', 'admin', 'true');");
        
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS osd ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "is_show   BOOLEAN,"
                     "size      INTEGER,"
                     "x         INTEGER,"
                     "y         INTEGER,"
                     "color     INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO osd (name, is_show, size, x, y, color) "
                     "VALUES ('datetime', 'true', 5, 10, 20, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, is_show, size, x, y, color) "
                     "VALUES ('device_name', 'true', 5, 10, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, is_show, size, x, y, color) "
                     "VALUES ('comment', 'true', 5, 70, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, is_show, size, x, y, color) "
                     "VALUES ('frame_rate', 'true', 5, 10, 80, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, is_show, size, x, y, color) "
                     "VALUES ('bit_rate', 'true', 5, 10, 90, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS video ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "value     INTEGER"
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
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "value     INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO scene (name, value) "
                     "VALUES ('scenario', 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "value     INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network (name, value) "
                     "VALUES ('method', 0);");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_static ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "value     TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('address', '192.168.0.100');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('netmask', '255.255.255.0');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('gateway', '192.168.0.1');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('dns1', '');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('dns2', '');");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_pppoe ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "value     TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_pppoe (name, value) "
                     "VALUES ('username', '');");
        EXEC_OR_FAIL("INSERT INTO network_pppoe (name, value) "
                     "VALUES ('password', '');");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_port ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "value     INTEGER UNIQUE NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_port (name, value) "
                     "VALUES ('http', 80);");
        EXEC_OR_FAIL("INSERT INTO network_port (name, value) "
                     "VALUES ('rtsp', 554);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS datetime ("
                     "_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name      TEXT UNIQUE NOT NULL,"
                     "int_value INTEGER,"
                     "str_value TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO datetime (name, int_value, str_value) "
                     "VALUES ('timezone', 0, '');");
        EXEC_OR_FAIL("INSERT INTO datetime (name, int_value, str_value) "
                     "VALUES ('use_ntp', 0, '');");
        EXEC_OR_FAIL("INSERT INTO datetime (name, int_value, str_value) "
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
