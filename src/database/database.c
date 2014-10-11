#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gom/gom.h>
#include <sysutils.h>
#include "database.h"
#include "common.h"
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
#include "misc.h"
#include "event_input.h"

#define DATABASE_PATH "/data"
#define DATABASE_NAME "configuration.sqlite3"
#define DATABASE_PATH_NAME DATABASE_PATH "/" DATABASE_NAME

typedef struct _IpcamDatabasePrivate
{
    GomRepository *repository;
    GomAdapter *adapter;
    GomResource *resource;
} IpcamDatabasePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamDatabase, ipcam_database, G_TYPE_OBJECT);

static void ipcam_database_open(IpcamDatabase *database);

static void ipcam_database_finalize (GObject *object)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(IPCAM_DATABASE(object));
    g_object_unref(priv->repository);
    g_object_unref(priv->adapter);
    if (priv->resource)
    {
        g_object_unref(priv->resource);
    }
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
        /************************************************
         * misc table                                   *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS misc ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT,"
                     "vtype    TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO misc (name, value, vtype) "
                     "VALUES ('language', '中文', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO misc (name, value, vtype) "
                     "VALUES ('rtsp_auth', '0', 'BOOLEAN');");
        /************************************************
         * base_info table                              *
         ************************************************/
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
                     "VALUES ('manufacturer', 'YXG Electronic Ltd.', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('model', 'NCD108-1-L', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('firmware', 'V1.0.0', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('serial', 'NCD1081A16000001', 0);");
        EXEC_OR_FAIL("INSERT INTO base_info (name, value, rw) "
                     "VALUES ('hardware', 'Rev.1', 0);");
        /************************************************
         * users table                                  *
         ************************************************/
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
        /************************************************
         * datetime table                               *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS datetime ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT,"
                     "vtype    TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO datetime (name, value, vtype) "
                     "VALUES ('timezone', '(GMT+08:00) Beijing', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO datetime (name, value, vtype) "
                     "VALUES ('use_ntp', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO datetime (name, value, vtype) "
                     "VALUES ('ntp_server', 'pool.ntp.org', 'STRING');");
        /************************************************
         * video table                                  *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS video ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT,"
                     "vtype    TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('flip', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('mirror', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('profile', 'baseline', 'STRING');");
        /* Main profile */
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('master:frame_rate', '25', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('master:bit_rate', 'CBR', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('master:bit_rate_value', '4096', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('master:resolution', 'UXGA', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('master:stream_path', 'main_stream', 'STRING');");
        /* Sub profile */
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('slave:frame_rate', '25', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('slave:bit_rate', 'CBR', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('slave:bit_rate_value', '1024', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('slave:resolution', 'D1', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO video (name, value, vtype) "
                     "VALUES ('slave:stream_path', 'sub_stream', 'STRING');");
        /************************************************
         * image table                                  *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS image ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT,"
                     "vtype    TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('watermark', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('3ddnr', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('brightness', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('chrominance', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('contrast', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('saturation', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO image (name, value, vtype) "
                     "VALUES ('scenario', '50Hz', 'STRING');");
        /************************************************
         * privacy_block table                          *
         ************************************************/
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
        /************************************************
         * day_night_mode table                         *
         ************************************************/
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
        /************************************************
         * osd table                                    *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS osd ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "isshow   BOOLEAN,"
                     "size     INTEGER,"
                     "left     INTEGER,"
                     "top      INTEGER,"
                     "color    INTEGER"
                     ");");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('master:datetime', 1, 20, 10, 35, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('master:device_name', 1, 20, 10, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('master:comment', 1, 20, 800, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('master:frame_rate', 1, 20, 10, 945, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('master:bit_rate', 1, 20, 10, 970, 0);");
        
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('slave:datetime', 1, 20, 10, 35, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('slave:device_name', 1, 20, 10, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('slave:comment', 1, 20, 800, 10, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('slave:frame_rate', 1, 20, 10, 945, 0);");
        EXEC_OR_FAIL("INSERT INTO osd (name, isshow, size, left, top, color) "
                     "VALUES ('slave:bit_rate', 1, 20, 10, 970, 0);");
        /************************************************
         * szyc table                               *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS szyc ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO szyc (name, value) "
                     "VALUES ('train_num', '');");
        EXEC_OR_FAIL("INSERT INTO szyc (name, value) "
                     "VALUES ('carriage_num', '');");
        EXEC_OR_FAIL("INSERT INTO szyc (name, value) "
                     "VALUES ('position_num', '');");
        /************************************************
         * network table                                *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network (name, value) "
                     "VALUES ('method', 'static');");
        /************************************************
         * network_static table                         *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_static ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
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
        /************************************************
         * network_pppoe table                          *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS network_pppoe ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO network_pppoe (name, value) "
                     "VALUES ('username', '');");
        EXEC_OR_FAIL("INSERT INTO network_pppoe (name, value) "
                     "VALUES ('password', '');");
        /************************************************
         * network_port table                           *
         ************************************************/
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
        /************************************************
         * event_input table                            *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_input ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "enable   INTEGER NOT NULL,"
                     "mon      TEXT NOT NULL,"
                     "tue      TEXT NOT NULL,"
                     "wed      TEXT NOT NULL,"
                     "thu      TEXT NOT NULL,"
                     "fri      TEXT NOT NULL,"
                     "sat      TEXT NOT NULL,"
                     "sun      TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_input (name, enable, mon, tue, wed, thu, fri, sat, sun) "
                     "VALUES ('input1', 0, '', '', '', '', '', '', '');");
        /************************************************
         * event_output table                           *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_output ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "normal   TEXT NOT NULL,"
                     "period   INTEGER NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_output (name, normal, period) "
                     "VALUES ('output1', 'open', 0);");
        /************************************************
         * event_motion table                           *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_motion ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "enable        INTEGER NOT NULL,"
                     "sensitivity   INTEGER NOT NULL,"
                     "left          INTEGER NOT NULL,"
                     "top           INTEGER NOT NULL,"
                     "width         INTEGER NOT NULL,"
                     "height        INTEGER NOT NULL,"
                     "mon           TEXT,"
                     "tue           TEXT,"
                     "wed           TEXT,"
                     "thu           TEXT,"
                     "fri           TEXT,"
                     "sat           TEXT,"
                     "sun           TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_motion (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region1', 0, 50, 0, 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_motion (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region2', 0, 50, 0, 0, 0, 0);");
        /************************************************
         * event_cover table                            *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_cover ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "enable        INTEGER NOT NULL,"
                     "sensitivity   INTEGER NOT NULL,"
                     "left          INTEGER NOT NULL,"
                     "top           INTEGER NOT NULL,"
                     "width         INTEGER NOT NULL,"
                     "height        INTEGER NOT NULL,"
                     "mon           TEXT,"
                     "tue           TEXT,"
                     "wed           TEXT,"
                     "thu           TEXT,"
                     "fri           TEXT,"
                     "sat           TEXT,"
                     "sun           TEXT"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_cover (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region1', 0, 50, 0, 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_cover (name, enable, sensitivity, left, top, width, height) "
                     "VALUES ('region2', 0, 50, 0, 0, 0, 0);");
        /************************************************
         * event_proc table                             *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS event_proc ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "record        INTEGER NOT NULL,"
                     "sound         INTEGER NOT NULL,"
                     "output1       INTEGER NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO event_proc (name, record, sound, output1) "
                     "VALUES ('input1', 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_proc (name, record, sound, output1) "
                     "VALUES ('motion', 0, 0, 0);");
        EXEC_OR_FAIL("INSERT INTO event_proc (name, record, sound, output1) "
                     "VALUES ('cover', 0, 0, 0);");
        /************************************************
         * record_schedule table                        *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS record_schedule ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "mon           TEXT,"
                     "tue           TEXT,"
                     "wed           TEXT,"
                     "thu           TEXT,"
                     "fri           TEXT,"
                     "sat           TEXT,"
                     "sun           TEXT"
                     ");");
        /************************************************
         * record_strategy table                        *
         ************************************************/
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS record_strategy ("
                     "id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name          TEXT UNIQUE NOT NULL,"
                     "value         TEXT,"
                     "vtype         TEXT NOT NULL"
                     ");");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value, vtype) "
                     "VALUES ('nr_file_switch', 'size', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value, vtype) "
                     "VALUES ('nr_file_size', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value, vtype) "
                     "VALUES ('nr_file_period', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value, vtype) "
                     "VALUES ('er_file_period', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO record_strategy (name, value, vtype) "
                     "VALUES ('storage_full', 'stop', 'STRING');");
        
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
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
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
static GomResource *ipcam_database_add_resource(IpcamDatabase *database, GType resource_type)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GomResource *resource = NULL;

    resource = g_object_new(resource_type, "repository", priv->repository, NULL);

    return resource;
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
void ipcam_database_set_user_role(IpcamDatabase *database, const gchar *username, const gchar *role)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_set(resource, "role", role, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("save users role error: %s\n", error->message);
        g_error_free(error);
    }
}
gchar *ipcam_database_get_user_role(IpcamDatabase *database, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *role = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        g_object_get(resource, "role", &role, NULL);
        g_object_unref(resource);
    }
    
    return role;
}
gboolean ipcam_database_add_user(IpcamDatabase *database, const gchar *username,
                                 const gchar *password, const gchar *role)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_add_resource(database, IPCAM_USERS_TYPE);
    if (resource)
    {
        g_object_set(resource,
                     "name", username,
                     "password", password,
                     "role", role,
                     NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }
    else
    {
        return FALSE;
    }

    if (error)
    {
        g_print("add user error: %s\n", error->message);
        g_error_free(error);
        return FALSE;
    }
    return TRUE;
}
gboolean ipcam_database_del_user(IpcamDatabase *database, const gchar *username)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    GError *error = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_USERS_TYPE, username);
    if (resource)
    {
        gom_resource_delete_sync(resource, &error);
        g_object_unref(resource);
    }
    else
    {
        return FALSE;
    }

    if (error)
    {
        g_print("delete user error: %s\n", error->message);
        g_error_free(error);
        return FALSE;
    }
    return TRUE;
}
void ipcam_database_set_osd(IpcamDatabase *database,
                            const gchar *name,
                            gboolean isshow,
                            guint size,
                            guint left,
                            guint top,
                            guint color)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_OSD_TYPE, name);
    if (resource)
    {
        g_object_set(resource,
                     "isshow", isshow,
                     "size", size,
                     "left", left,
                     "top", top,
                     "color", color,
                     NULL);
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
                                guint *left,
                                guint *top,
                                guint *color)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    gboolean ret = FALSE;
    
    resource = ipcam_database_get_resource(database, IPCAM_OSD_TYPE, name);
    if (resource)
    {
        g_object_get(resource,
                     "isshow", isshow,
                     "size", size,
                     "left", left,
                     "top", top,
                     "color", color,
                     NULL);
        g_object_unref(resource);
        ret = TRUE;
    }
    
    return ret;
}

void ipcam_database_set_video(IpcamDatabase *database, const gchar *name, const GVariant *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;
    gchar *temp_value = NULL;

    if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_STRING))
    {
        temp_value = g_variant_dup_string((GVariant *)value, NULL);
    }
    else if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_BOOLEAN))
    {
        gboolean b = g_variant_get_boolean((GVariant *)value);
        temp_value = g_malloc0(8);
        g_snprintf(temp_value, 8, "%u", b);
    }
    else if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_UINT32))
    {
        guint i = g_variant_get_uint32((GVariant *)value);
        temp_value = g_malloc0(8);
        g_snprintf(temp_value, 8, "%u", i);
    }
    else
    {
        g_warn_if_reached();
    }

    if (temp_value)
    {    
        resource = ipcam_database_get_resource(database, IPCAM_VIDEO_TYPE, name);
        if (resource)
        {
            g_object_set(resource, "value", temp_value, NULL);
            gom_resource_save_sync(resource, &error);
            g_object_unref(resource);
        }
        g_free(temp_value);
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
    gchar *temp_value = NULL;
    gchar *vtype = NULL;
    GVariant *value = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_VIDEO_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &temp_value, "vtype", &vtype, NULL);
        g_object_unref(resource);
        if (vtype && g_str_equal(vtype, "STRING"))
        {
            value = g_variant_new_string(temp_value);
        }
        else if (vtype && g_str_equal(vtype, "BOOLEAN"))
        {
            value = g_variant_new_boolean(g_ascii_strtoull(temp_value, NULL, 10));
        }
        else if (vtype && g_str_equal(vtype, "INTEGER"))
        {
            value = g_variant_new_uint32(g_ascii_strtoull(temp_value, NULL, 10));
        }
        else
        {
            g_warn_if_reached();
        }
        g_free(temp_value);
        g_free(vtype);
    }

    return value;
}

void ipcam_database_set_image(IpcamDatabase *database, const gchar *name, const GVariant *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;
    gchar *vtype = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_IMAGE_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "vtype", &vtype, NULL);
        if (vtype && g_str_equal(vtype, "INTEGER"))
        {
            gchar *temp_value = g_malloc0(16);
            g_snprintf(temp_value, 16, "%u", g_variant_get_uint32((GVariant *)value));
            g_object_set(resource, "value", temp_value, NULL);
            g_free(temp_value);
        }
        else if (vtype && g_str_equal(vtype, "BOOLEAN"))
        {
            gchar *temp_value = g_malloc0(8);
            g_snprintf(temp_value, 16, "%u", g_variant_get_boolean((GVariant *)value));
            g_object_set(resource, "value", temp_value, NULL);
            g_free(temp_value);
        }
        else if (vtype && g_str_equal(vtype, "STRING"))
        {
            g_object_set(resource, "value", g_variant_get_string((GVariant *)value, NULL), NULL);
        }
        else
        {
            g_warn_if_reached();
        }
        g_free(vtype);
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
    gchar *vtype = NULL;
    gchar *temp_value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_IMAGE_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &temp_value, "vtype", &vtype, NULL);
        g_object_unref(resource);
    }

    if (temp_value)
    {
        if (vtype && g_str_equal(vtype, "INTEGER"))
        {
            value = g_variant_new_uint32(g_ascii_strtoull(temp_value, NULL, 10));
        }
        else if (vtype && g_str_equal(vtype, "BOOLEAN"))
        {
            value = g_variant_new_boolean(g_ascii_strtoull(temp_value, NULL, 10));
        }
        else if (vtype && g_str_equal(vtype, "STRING"))
        {
            value = g_variant_new_string(temp_value);
        }
        else
        {
            g_warn_if_reached();
        }
        
        g_free(temp_value);
        g_free(vtype);
    }
    
    return value;
}
void ipcam_database_set_network(IpcamDatabase *database, const gchar *name, const gchar *value)
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
gchar *ipcam_database_get_network(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *value = NULL;
    
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
void ipcam_database_set_datetime(IpcamDatabase *database, const gchar *name, const GVariant *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_DATETIME_TYPE, name);
    if (resource)
    {
        gchar *temp_value = NULL;
        if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_STRING))
        {
            temp_value = g_strdup(g_variant_get_string((GVariant *)value, NULL));
        }
        else if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_BOOLEAN))
        {
            temp_value = g_malloc0(8);
            g_snprintf(temp_value, 8, "%u", g_variant_get_boolean((GVariant *)value));
        }
        else
        {
            g_warn_if_reached();
        }

        if (temp_value)
        {
            g_object_set(resource, "value", temp_value, NULL);
            gom_resource_save_sync(resource, &error);
        }
        
        g_object_unref(resource);
        g_free(temp_value);
    }

    if (error)
    {
        g_print("set datetime record failed: %s\n", error->message);
        g_error_free(error);
    }
}
GVariant *ipcam_database_get_datetime(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *temp_value = NULL;
    gchar *vtype = NULL;
    GVariant *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_DATETIME_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &temp_value, "vtype", &vtype, NULL);
        g_object_unref(resource);
    }

    if (vtype && g_str_equal(vtype, "STRING"))
    {
        value = g_variant_new_string(temp_value);
    }
    else if (vtype && g_str_equal(vtype, "BOOLEAN"))
    {
        value = g_variant_new_boolean(g_ascii_strtoull(temp_value, NULL, 10));
    }
    else
    {
        g_warn_if_reached();
    }

    g_free(temp_value);
    g_free(vtype);
    return value;
}
void ipcam_database_set_misc(IpcamDatabase *database, const gchar *name, const GVariant *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_MISC_TYPE, name);
    if (resource)
    {
        gchar *temp_value = NULL;
        if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_STRING))
        {
            temp_value = g_strdup(g_variant_get_string((GVariant *)value, NULL));
        }
        else if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_BOOLEAN))
        {
            temp_value = g_malloc0(8);
            g_snprintf(temp_value, 8, "%u", g_variant_get_boolean((GVariant *)value));
        }
        else
        {
            g_warn_if_reached();
        }

        if (temp_value)
        {
            g_object_set(resource, "value", temp_value, NULL);
            gom_resource_save_sync(resource, &error);
        }
        
        g_object_unref(resource);
        g_free(temp_value);
    }

    if (error)
    {
        g_print("set misc record failed: %s\n", error->message);
        g_error_free(error);
    }
}
GVariant *ipcam_database_get_misc(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *temp_value = NULL;
    gchar *vtype = NULL;
    GVariant *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_MISC_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &temp_value, "vtype", &vtype, NULL);
        g_object_unref(resource);
    }

    if (vtype && g_str_equal(vtype, "STRING"))
    {
        value = g_variant_new_string(temp_value);
    }
    else if (vtype && g_str_equal(vtype, "BOOLEAN"))
    {
        value = g_variant_new_boolean(g_ascii_strtoull(temp_value, NULL, 10));
    }
    else
    {
        g_warn_if_reached();
    }

    g_free(temp_value);
    g_free(vtype);
    return value;
}
void ipcam_database_set_privacy_block(IpcamDatabase *database,
                                      const gchar *name,
                                      gboolean enabled,
                                      guint left,
                                      guint top,
                                      guint width,
                                      guint height,
                                      guint color)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_PRIVACY_BLOCK_TYPE, name);
    if (resource)
    {
        g_object_set(resource,
                     "enable", enabled,
                     "left", left,
                     "top", top,
                     "width", width,
                     "height", height,
                     "color", color,
                     NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set osd record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gboolean ipcam_database_get_privacy_block(IpcamDatabase *database,
                                          const gchar *name,
                                          gboolean *enabled,
                                          guint *left,
                                          guint *top,
                                          guint *width,
                                          guint *height,
                                          guint *color)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    GomResource *resource = NULL;
    gboolean ret = FALSE;
    
    resource = ipcam_database_get_resource(database, IPCAM_PRIVACY_BLOCK_TYPE, name);
    if (resource)
    {
        g_object_get(resource,
                     "enable", enabled,
                     "left", left,
                     "top", top,
                     "width", width,
                     "height", height,
                     "color", color,
                     NULL);
        g_object_unref(resource);
        ret = TRUE;
    }
    
    return ret;
}
void ipcam_database_set_day_night_mode(IpcamDatabase *database, const gchar *name, guint value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_DAY_NIGHT_MODE_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set day night mode record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gint ipcam_database_get_day_night_mode(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), -1);
    GomResource *resource = NULL;
    gint value = -1;
    
    resource = ipcam_database_get_resource(database, IPCAM_DAY_NIGHT_MODE_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_szyc(IpcamDatabase *database, const gchar *name, const gchar *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_SZYC_TYPE, name);
    if (resource)
    {
        g_object_set(resource, "value", value, NULL);
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set szyc record failed: %s\n", error->message);
        g_error_free(error);
    }
}
gchar *ipcam_database_get_szyc(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    gchar *value = NULL;
    
    resource = ipcam_database_get_resource(database, IPCAM_SZYC_TYPE, name);
    if (resource)
    {
        g_object_get(resource, "value", &value, NULL);
        g_object_unref(resource);
    }
    
    return value;
}
void ipcam_database_set_event_input(IpcamDatabase *database, const gchar *name, GVariant *value)
{
    g_return_if_fail(IPCAM_IS_DATABASE(database));
    GomResource *resource = NULL;
    GError *error = NULL;

    resource = ipcam_database_get_resource(database, IPCAM_SZYC_TYPE, name);
    if (resource)
    {
        gom_resource_save_sync(resource, &error);
        g_object_unref(resource);
    }

    if (error)
    {
        g_print("set szyc record failed: %s\n", error->message);
        g_error_free(error);
    }
}
GVariant *ipcam_database_get_event_input(IpcamDatabase *database, const gchar *name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    GomResource *resource = NULL;
    GVariant *value;

    resource = ipcam_database_get_resource(database, IPCAM_SZYC_TYPE, name);
    if (resource)
    {
        g_object_unref(resource);
    }
    
    return value;
}
static void ipcam_database_set_schedules(IpcamDatabase *database, GVariant *value)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    Schedules *sche = NULL;
    if (IS_64BIT_MACHINE)
    {
        sche = GSIZE_TO_POINTER(g_variant_get_uint64(value));
    }
    else
    {
        sche = GSIZE_TO_POINTER(g_variant_get_uint32(value));
    }
    g_object_set(priv->resource,
                 "mon", sche->schedule[ENUM_MON],
                 "tue", sche->schedule[ENUM_TUE],
                 "wed", sche->schedule[ENUM_WED],
                 "thu", sche->schedule[ENUM_THU],
                 "fri", sche->schedule[ENUM_FRI],
                 "sat", sche->schedule[ENUM_SAT],
                 "sun", sche->schedule[ENUM_SUN],
                 NULL);
}
static gboolean ipcam_database_update_value(IpcamDatabase *database, const gchar *sub_name, GVariant *value)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GError *error = NULL;
    gboolean ret = TRUE;

    if (g_str_equal(sub_name, "schedules"))
    {
        ipcam_database_set_schedules(database, value);
    }
    else
    {
        if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING))
        {
            g_object_set(priv->resource, sub_name, g_variant_get_string(value, NULL), NULL);
        }
        else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BOOLEAN))
        {
            g_object_set(priv->resource, sub_name, g_variant_get_boolean(value), NULL);
        }
        else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32))
        {
            g_object_set(priv->resource, sub_name, g_variant_get_uint32(value), NULL);
        }
        else
        {
            g_warn_if_reached();
            ret = FALSE;
        }
    }
    
    gom_resource_save_sync(priv->resource, &error);

    if (error)
    {
        g_print("set record failed: %s\n", error->message);
        g_error_free(error);
        ret = FALSE;
    }

    return ret;
}
gboolean ipcam_database_update(IpcamDatabase *database, GType table, const gchar *name, const gchar *sub_name, GVariant *value)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), FALSE);
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    gboolean ret = FALSE;

    priv->resource = ipcam_database_get_resource(database, table, name);
    if (priv->resource)
    {
        ret = ipcam_database_update_value(database, sub_name, value);
        
        g_object_unref(priv->resource);
        priv->resource = NULL;
    }
    return ret;
}
GVariant *ipcam_database_get_schedules(IpcamDatabase *database)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GVariant *value;

    Schedules *sche = g_new0(Schedules, 1);
    g_object_get(priv->resource,
                 "mon", &sche->schedule[ENUM_MON],
                 "tue", &sche->schedule[ENUM_TUE],
                 "wed", &sche->schedule[ENUM_WED],
                 "thu", &sche->schedule[ENUM_THU],
                 "fri", &sche->schedule[ENUM_FRI],
                 "sat", &sche->schedule[ENUM_SAT],
                 "sun", &sche->schedule[ENUM_SUN],
                 NULL);
    if (IS_64BIT_MACHINE)
    {
        value = g_variant_new_uint64(GPOINTER_TO_SIZE(sche));
    }
    else
    {
        value = g_variant_new_uint32(GPOINTER_TO_SIZE(sche));
    }
    return value;
}
GVariant *ipcam_database_read_value(IpcamDatabase *database, const gchar *sub_name)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GParamSpec *param_spec = NULL;
    GVariant *value = NULL;
    guint ival;
    gchar *sval;
    gboolean bval;

    if (g_str_equal(sub_name, "schedules"))
    {
        value = ipcam_database_get_schedules(database);
    }
    else
    {
        param_spec = g_object_class_find_property(G_OBJECT_GET_CLASS(priv->resource), sub_name);
        switch (G_PARAM_SPEC_VALUE_TYPE(param_spec))
        {
        case G_TYPE_UINT:
            g_object_get(priv->resource, sub_name, &ival, NULL);
            value = g_variant_new_uint32(ival);
            break;
        case G_TYPE_STRING:
            g_object_get(priv->resource, sub_name, &sval, NULL);
            value = g_variant_new_string(sval);
            g_free(sval);
            break;
        case G_TYPE_BOOLEAN:
            g_object_get(priv->resource, sub_name, &bval, NULL);
            value = g_variant_new_boolean(bval);
            break;
        default:
            g_warn_if_reached();
            break;
        }
    }    

    return value;
}
GVariant *ipcam_database_read(IpcamDatabase *database, GType table, const gchar *name, const gchar *sub_name)
{
    g_return_val_if_fail(IPCAM_IS_DATABASE(database), NULL);
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GVariant *value = NULL;

    priv->resource = ipcam_database_get_resource(database, table, name);
    if (priv->resource)
    {
        value = ipcam_database_read_value(database, sub_name);
        g_object_unref(priv->resource);
        priv->resource = NULL;
    }

    return value;
}

