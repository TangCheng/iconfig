#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gom/gom.h>
#include <sysutils.h>
#include "database.h"
#include "common.h"
#include "users.h"

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
        EXEC_OR_FAIL("CREATE TABLE IF NOT EXISTS params ("
                     "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "name     TEXT UNIQUE NOT NULL,"
                     "value    TEXT NOT NULL DEFAULT '',"
                     "vtype    TEXT NOT NULL DEFAULT 'STRING',"
                     "rw       INTEGER NOT NULL DEFAULT 1"
                     ");");
        /* misc */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('misc:language', '简体中文', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('misc:rtsp_auth', '0', 'BOOLEAN');");
        /* base_info */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('base_info:device_name', 'ipcam', 'STRING');");
		EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
		             "VALUES ('base_info:location', 'China', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('base_info:comment', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype, rw) "
                     "VALUES ('base_info:manufacturer', 'EasyWay', 'STRING', 0);");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype, rw) "
                     "VALUES ('base_info:model', 'NCD108-1-L', 'STRING', 0);");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype, rw) "
                     "VALUES ('base_info:firmware', '1.0.0', 'STRING', 0);");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype, rw) "
                     "VALUES ('base_info:serial', 'NCD1081A16000001', 'STRING', 0);");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype, rw) "
                     "VALUES ('base_info:hardware', 'Rev.1', 'STRING', 0);");
        /* datetime */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('datetime:timezone', '(GMT+08:00) Beijing', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('datetime:use_ntp', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('datetime:ntp_server', 'pool.ntp.org', 'STRING');");
        /* video */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:flip', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:mirror', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:profile', 'baseline', 'STRING');");
        /* Main profile */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:master:frame_rate', '25', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:master:bit_rate', 'CBR', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:master:bit_rate_value', '4096', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:master:resolution', '1080P', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:master:stream_path', 'main_stream', 'STRING');");
        /* Sub profile */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:slave:frame_rate', '25', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:slave:bit_rate', 'CBR', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:slave:bit_rate_value', '1024', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:slave:resolution', 'D1', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('video:slave:stream_path', 'sub_stream', 'STRING');");
        /* image */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:watermark', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:3ddnr', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:brightness', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:chrominance', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:contrast', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:saturation', '128', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('image:scenario', '50Hz', 'STRING');");
        /* privacy_block */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region1:enable', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region1:rect:left', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region1:rect:top', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region1:rect:width', '67', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region1:rect:height', '90', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region1:color', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region2:enable', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region2:rect:left', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region2:rect:top', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region2:rect:width', '67', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region2:rect:height', '90', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('privacy_block:region2:color', '0', 'INTEGER');");
        /* day_night_mode */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('day_night_mode:force_night_mode', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('day_night_mode:night_mode_threshold', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('day_night_mode:ir_intensity', '80', 'INTEGER');");
        /* osd */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:datetime:isshow', '1', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:datetime:size', '20', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:datetime:left', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:datetime:top', '35', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:datetime:color', '0', 'INTEGER');");

        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:device_name:isshow', '1', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:device_name:size', '20', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:device_name:left', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:device_name:top', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:device_name:color', '0', 'INTEGER');");
        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:comment:isshow', '1', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:comment:size', '20', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:comment:left', '800', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:comment:top', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:comment:color', '0', 'INTEGER');");
        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:frame_rate:isshow', '1', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:frame_rate:size', '20', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:frame_rate:left', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:frame_rate:top', '945', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:frame_rate:color', '0', 'INTEGER');");

        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:bit_rate:isshow', '1', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:bit_rate:size', '20', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:bit_rate:left', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:bit_rate:top', '970', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('osd:master:bit_rate:color', '0', 'INTEGER');");
        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:datetime:isshow', '1', 'BOOLEAN');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:datetime:size', '20', 'INTEGER');");            
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:datetime:left', '10', 'INTEGER');");            
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:datetime:top', '35', 'INTEGER');");             
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:datetime:color', '0', 'INTEGER');");           
                                                                                         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:device_name:isshow', '1', 'BOOLEAN');");       
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:device_name:size', '20', 'INTEGER');");        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:device_name:left', '10', 'INTEGER');");        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:device_name:top', '10', 'INTEGER');");         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:device_name:color', '0', 'INTEGER');");        
                                                                                         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:comment:isshow', '1', 'BOOLEAN');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:comment:size', '20', 'INTEGER');");            
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:comment:left', '800', 'INTEGER');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:comment:top', '10', 'INTEGER');");             
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:comment:color', '0', 'INTEGER');");            
                                                                                         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:frame_rate:isshow', '1', 'BOOLEAN');");        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:frame_rate:size', '20', 'INTEGER');");         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:frame_rate:left', '10', 'INTEGER');");         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:frame_rate:top', '945', 'INTEGER');");         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:frame_rate:color', '0', 'INTEGER');");         
                                                                                         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:bit_rate:isshow', '1', 'BOOLEAN');");          
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:bit_rate:size', '20', 'INTEGER');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:bit_rate:left', '10', 'INTEGER');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:bit_rate:top', '970', 'INTEGER');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                          
                     "VALUES ('osd:slave:bit_rate:color', '0', 'INTEGER');");           
        /* szyc */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('szyc:train_num', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('szyc:carriage_num', '1', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('szyc:position_num', '1', 'STRING');");
        /* network */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:method', 'static', 'STRING');");
        /* network_static */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:address:ipaddr', '192.168.1.217, 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:address:netmask', '255.255.255.0', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:address:gateway', '192.168.1.1', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:address:dns1', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:address:dns2', '', 'STRING');");
        /* network_pppoe */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:pppoe:username', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value) "
                     "VALUES ('network:pppoe:password', '', STRING);");
        /* network_port */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:port:http', '80', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:port:ftp', '21', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('network:prot:rtsp', '554', 'INTEGER');");
        /* event_input */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:enable', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:mon', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:tue', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:wed', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:thu', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:fri', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:sat', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_input:input1:schedules:sun', '', 'STRING');");
        /* event_output */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_output:output1:normal', 'open', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_putput:output1:period', '10', 'INTEGER');");
        /* event_motion */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:enable', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:sensitivity', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:rect:left', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:rect:top', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:rect:width', '67', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:rect:height', '90', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:schedules:mon', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:schedules:tue', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:schedules:wed', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region:schedules1:thu', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:schedules:fri', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:schedules:sat', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region1:schedules:sun', '', 'STRING');");

        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:enable', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:sensitivity', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:rect:left', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:rect:top', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:rect:width', '67', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:rect:height', '90', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:mon', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:tue', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:wed', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:thu', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:fri', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:sat', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_motion:region2:schedules:sun', '', 'STRING');");
        /* event_cover */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:enable', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:sensitivity', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:rect:left', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:rect:top', '0', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:rect:width', '67', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:rect:height', '90', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:mon', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:tue', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:wed', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:thu', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:fri', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:sat', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_cover:region1:schedules:sun', '', 'STRING');");
                                                              
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:enable', '0', 'BOOLEAN');");      
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:sensitivity', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:rect:left', '0', 'INTEGER');");        
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:rect:top', '0', 'INTEGER');");         
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:rect:width', '67', 'INTEGER');");      
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:rect:height', '90', 'INTEGER');");     
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:mon', '', 'STRING');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:tue', '', 'STRING');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:wed', '', 'STRING');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:thu', '', 'STRING');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:fri', '', 'STRING');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:sat', '', 'STRING');");           
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "                       
                     "VALUES ('event_cover:region2:schedules:sun', '', 'STRING');");
        /* event_proc */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:input1:record', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:input1:sound', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:input1:output1', '0', 'BOOLEAN');");

        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:motion:record', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:motion:sound', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:motion:output1', '0', 'BOOLEAN');");

        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:cover:record', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:cover:sound', '0', 'BOOLEAN');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('event_proc:cover:output1', '0', 'BOOLEAN');");
        /* record_schedule */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:mon', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:tue', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:wed', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:thu', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:fri', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:sat', '', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_schedules:sun', '', 'STRING');");
        /* record_strategy */
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_strategy:nr_file_switch', 'size', 'STRING');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_strategy:nr_file_size', '50', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_strategy:nr_file_period', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_strategy:er_file_period', '10', 'INTEGER');");
        EXEC_OR_FAIL("INSERT INTO params (name, value, vtype) "
                     "VALUES ('record_strategy:storage_full', 'stop', 'STRING');");
        
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

static gboolean ipcam_database_update_value(IpcamDatabase *database, const gchar *sub_name, GVariant *value)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GError *error = NULL;
    gchar *temp_value = NULL;
    gboolean ret = TRUE;

    if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_STRING))
    {
        temp_value = g_strdup(g_variant_get_string((GVariant *)value, NULL));
    }
    else if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_BOOLEAN))
    {
        temp_value = g_malloc0(8);
        g_snprintf(temp_value, 8, "%u", g_variant_get_boolean((GVariant *)value));
    }
    else if (g_variant_is_of_type((GVariant *)value, G_VARIANT_TYPE_UINT32))
    {
        temp_value = g_malloc0(16);
        g_snprintf(temp_value, 16, "%u", g_variant_get_uint32((GVariant *)value));
    }
    else
    {
        g_warn_if_reached();
        ret = FALSE;
    }

    if (temp_value)
    {
        g_object_set(priv->resource, "value", temp_value, NULL);
        gom_resource_save_sync(priv->resource, &error);
        g_free(temp_value);
    }

    if (error)
    {
        g_print("set datetime record failed: %s\n", error->message);
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
        guint rw;
        g_object_get(priv->resource, "rw", &rw, NULL);
        if (0 == rw)
        {
            g_warning("Attempt to set read-only property '%s'\n", name);
        }
        else
        {
            ret = ipcam_database_update_value(database, sub_name, value);
        }
        
        g_object_unref(priv->resource);
        priv->resource = NULL;
    }
    return ret;
}
GVariant *ipcam_database_read_value(IpcamDatabase *database, const gchar *sub_name)
{
    IpcamDatabasePrivate *priv = ipcam_database_get_instance_private(database);
    GVariant *value = NULL;
    gchar *temp_value = NULL;
    gchar *vtype = NULL;

    g_object_get(priv->resource, sub_name, &temp_value, "vtype", &vtype, NULL);
    
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
