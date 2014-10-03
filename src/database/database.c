#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gom/gom.h>
#include <sysutils.h>
#include "database.h"
#include "base_info.h"
#include "users.h"
#include "datetime.h"
#include "video.h"
#include "image.h"
#include "privacy_block.h"
#include "day_night_mode.h"
#include "osd.h"
#include "szyc.h"
#include "network.h"
#include "network_static.h"
#include "network_pppoe.h"
#include "network_port.h"

#define DATABASE_PATH "/data"
#define DATABASE_NAME "configuration.sqlite3"
#define DATABASE_PATH_NAME DATABASE_PATH "/" DATABASE_NAME

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
        EXEC_OR_FAIL("CREATE   TABLE IF NOT EXISTS base_info ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT NOT NULL,"
                     "rw       INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('device_name', 'ipcam', 1);");
		EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
		             "VALUES ('location', 'China', 1);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('comment', '', 1);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('manufacturer', 'IPNC', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('model', 'IPCAM-100', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('firmware', 'V1.0.0', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('serial', '0123456789', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('hardware', '', 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS users ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "password TEXT NOT NULL,"
                     "role     TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO users (name, password, role) "
                     "VALUES ('admin', 'admin', 'administrator');");
        EXEC_OR_FAIL("INSERT INTO users (name, password, role) "
                     "VALUES ('operator', 'operator', 'operator');");
        EXEC_OR_FAIL("INSERT INTO users (name, password, role) "
                     "VALUES ('user', 'user', 'user');");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS datetime ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value"
                     ");");
        EXEC_OR_FAIL("INSERT INTO datetime (name, value) "
                     "VALUES ('timezone', 0);");
        EXEC_OR_FAIL("INSERT INTO datetime (name, value) "
                     "VALUES ('use_ntp', 0);");
        EXEC_OR_FAIL("INSERT INTO datetime (name, value) "
                     "VALUES ('ntp_server', 'pool.ntp.org');");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS video ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value"
                     ");");
        /* Main profile */
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:flip', 0);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:quanlity', 'baseline');");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:frame_rate', 25);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:bit_rate', 'CBR');");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:bit_rate_value', 4096);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:resolution', 'UXGA');");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('main_profile:stream_path', 'main_stream');");
        /* Sub profile */
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:flip', 0);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:quanlity', 'baseline');");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:frame_rate', 25);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:bit_rate', 'CBR');");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:bit_rate_value', 1024);");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:resolution', 'D1');");
        EXEC_OR_FAIL("INSERT INTO video (name, value) "
                     "VALUES ('sub_profile:stream_path', 'sub_stream');");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS image ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value"
                     ");");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('watermark', 0);");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('3ddnr', 0);");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('brightness', 128);");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('chrominance', 128);");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('contrast', 128);");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('saturation', 128);");
        EXEC_OR_FAIL("INSERT INTO image (name, value) "
                     "VALUES ('scenario', '50Hz');");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS privacy_block ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "enable   INTEGER NOT NULL,"
                     "left     INTEGER NOT NULL,"
                     "top      INTEGER NOT NULL,"
                     "width    INTEGER NOT NULL,"
                     "height   INTEGER NOT NULL,"
                     "color    INTEGER NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO privacy_block (name, enable, left, top, width, height, color) "
                     "VALUES ('region1', 0, 0, 0, 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO privacy_block (name, enable, left, top, width, height, color) "
                     "VALUES ('region2', 0, 0, 0, 0, 0, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS day_night_mode ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    INTEGER NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO day_night_mode (name, value) "
                     "VALUES ('force_night_mode', 0);");
        EXEC_OR_FAIL("INSERT INTO day_night_mode (name, value) "
                     "VALUES ('night_mode_threshold', 50);");
        EXEC_OR_FAIL("INSERT INTO day_night_mode (name, value) "
                     "VALUES ('ir_intensity', 80);");
        
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS osd ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT NOT NULL,"
                     "stream   TEXT NOT NULL,"
                     "isshow   BOOLEAN,"
                     "size     INTEGER,"
                     "x        INTEGER,"
                     "y        INTEGER,"
                     "color    INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('datetime', 'master', 1, 20, 10, 35, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('device_name', 'master', 1, 20, 10, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('comment', 'master', 1, 20, 800, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('frame_rate', 'master', 1, 20, 10, 945, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('bit_rate', 'master', 1, 20, 10, 970, 0);");
        
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('datetime', 'slave', 1, 20, 10, 35, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('device_name', 'slave', 1, 20, 10, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('comment', 'slave', 1, 20, 800, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('frame_rate', 'slave', 1, 20, 10, 945, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, stream, isshow, size, x, y, color) "
                     "VALUES ('bit_rate', 'slave', 1, 20, 10, 970, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS szyc ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO szyc (name, value) "
                     "VALUES ('train_num', '');");
        EXEC_OR_FAIL("INSERT INTO szyc (name, value) "
                     "VALUES ('trunk_num', '');");
        EXEC_OR_FAIL("INSERT INTO szyc (name, value) "
                     "VALUES ('position_num', '');");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network (name, value) "
                     "VALUES ('method', 'static');");
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_static ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('hwaddr', '00:00:12:23:34:45');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('ipaddr', '192.168.1.217');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('netmask', '255.255.255.0');");
        EXEC_OR_FAIL("INSERT INTO network_static (name, value) "
                     "VALUES ('gateway', '192.168.1.1');");
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
                     "VALUES ('ftp', 21);");
        EXEC_OR_FAIL("INSERT INTO network_port (name, value) "
                     "VALUES ('rtsp', 554);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_input ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "enable   INTEGER NOT NULL,"
                     "mon      BLOB,"
                     "tue      BLOB,"
                     "wed      BLOB,"
                     "thu      BLOB,"
                     "fri      BLOB,"
                     "sat      BLOB,"
                     "sun      BLOB"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_input (name, enable) "
                     "VALUES ('intput1', 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_output ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "normal   TEXT NOT NULL,"
                     "period   INTEGER NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_output (name, normal, period) "
                     "VALUES ('output1', 'open', 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_motion ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "enable        INTEGER NOT NULL,"
                     "sensitivity   INTEGER NOT NULL,"
                     "left          INTEGER NOT NULL,"
                     "top           INTEGER NOT NULL,"
                     "width         INTEGER NOT NULL,"
                     "height        INTEGER NOT NULL,"
                     "mon           BLOB,"
                     "tue           BLOB,"
                     "wed           BLOB,"
                     "thu           BLOB,"
                     "fri           BLOB,"
                     "sat           BLOB,"
                     "sun           BLOB"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_motion (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region1', 0, 50, 0, 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_motion (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region2', 0, 50, 0, 0, 0, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_cover ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "enable        INTEGER NOT NULL,"
                     "sensitivity   INTEGER NOT NULL,"
                     "left          INTEGER NOT NULL,"
                     "top           INTEGER NOT NULL,"
                     "width         INTEGER NOT NULL,"
                     "height        INTEGER NOT NULL,"
                     "mon           BLOB,"
                     "tue           BLOB,"
                     "wed           BLOB,"
                     "thu           BLOB,"
                     "fri           BLOB,"
                     "sat           BLOB,"
                     "sun           BLOB"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_cover (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region1', 0, 50, 0, 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_cover (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region2', 0, 50, 0, 0, 0, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_proc ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "record        INTEGER NOT NULL,"
                     "sound         INTEGER NOT NULL,"
                     "output1       INTEGER NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_proc (name, record, sound, output1) "
                     "VALUES ('intput1', 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_proc (name, record, sound, output1) "
                     "VALUES ('motion', 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_proc (name, record, sound, output1) "
                     "VALUES ('cover', 0, 0, 0);");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS record_schedule ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "mon           BLOB,"
                     "tue           BLOB,"
                     "wed           BLOB,"
                     "thu           BLOB,"
                     "fri           BLOB,"
                     "sat           BLOB,"
                     "sun           BLOB"
                     ");");

        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS record_strategy ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "value"
                     ");");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value) "
                     "VALUES ('nr_file_switch', 'size');");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value) "
                     "VALUES ('nr_file_size', 50);");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value) "
                     "VALUES ('nr_file_period', 10);");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value) "
                     "VALUES ('er_file_period', 10);");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value) "
                     "VALUES ('storage_full', 'stop');");
        
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
	struct stat st;
    
    priv->adapter = gom_adapter_new();
	if (stat(DATABASE_PATH, &st) < 0) {
		char *path = strdup(DATABASE_PATH);
		sysutils_make_directory(path, 0775);
		free(path);
	}
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
        guint rw;
        g_object_get(resource, "rw", &rw, NULL);
        if (rw)
        {
            g_object_set(resource, "value", value, NULL);
            gom_resource_save_sync(resource, &error);
        }
        else
        {
            g_warning("Attempt to set read-only property '%s'\n", name);
        }
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
void ipcam_database_set_user_privilege(IpcamDatabase *database, const gchar *username, guint privilege)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_set(resource, "privilege", privilege, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("save users privilege error: %s\n", error->message);
        g_error_free(error);
    }
}
guint ipcam_database_get_user_privilege(IpcamDatabase *database, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    guint privilege = 0;

    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_get(resource, "privilege", &privilege, NULL);
        g_object_unref(resource);
    }
    
    return privilege;
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

void ipcam_database_set_video(IpcamDatabase *database, const gchar *name, GVariant *value)
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

GVariant *ipcam_database_get_video(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    GVariant *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_VIDEO_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "intval", &value, NULL);
        g_object_unref(resource);
    }

    return value;
}

void ipcam_database_set_image(IpcamDatabase *database, const gchar *name, GVariant *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_IMAGE_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set image record failed: %s\n", error->message);
        g_error_free(error);
    }
}
GVariant *ipcam_database_get_image(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    GVariant *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_IMAGE_TYPE, name);
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
