/************************************************
 * misc table                                   *
 ************************************************/
CREATE TABLE IF NOT EXISTS misc (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT,
        vtype    TEXT NOT NULL
        );
INSERT INTO misc (name, value, vtype) 
        VALUES ('language', 'Chinese', 'STRING');
INSERT INTO misc (name, value, vtype) 
        VALUES ('rtsp_auth', '0', 'BOOLEAN');
/************************************************
 * base_info table                              *
 ************************************************/
CREATE TABLE IF NOT EXISTS base_info (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT NOT NULL,
        rw       INTEGER
        );
INSERT INTO base_info (name, value, rw) 
        VALUES ('device_name', 'ipcam', 1);
INSERT INTO base_info (name, value, rw) 
        VALUES ('location', 'China', 1);
INSERT INTO base_info (name, value, rw) 
        VALUES ('comment', '', 1);
INSERT INTO base_info (name, value, rw) 
        VALUES ('manufacturer', 'SZYC', 0);
INSERT INTO base_info (name, value, rw) 
        VALUES ('model', 'CS-001', 0);
INSERT INTO base_info (name, value, rw) 
        VALUES ('firmware', '1.0.0', 0);
INSERT INTO base_info (name, value, rw) 
        VALUES ('serial', 'NCD1081A16000001', 0);
INSERT INTO base_info (name, value, rw) 
        VALUES ('hardware', 'Rev.1', 0);
INSERT INTO base_info (name, value, rw) 
        VALUES ('device_type', '1', 0);

/************************************************
 * users table                                  *
 ************************************************/
CREATE TABLE IF NOT EXISTS users (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        password TEXT NOT NULL,
        role     TEXT NOT NULL
        );
INSERT INTO users (name, password, role) 
        VALUES ('admin', 'admin', 'administrator');
INSERT INTO users (name, password, role) 
        VALUES ('operator', '123456', 'operator');
INSERT INTO users (name, password, role) 
        VALUES ('user', '123456', 'user');
/************************************************
 * datetime table                               *
 ************************************************/
CREATE TABLE IF NOT EXISTS datetime (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT,
        vtype    TEXT NOT NULL
        );
INSERT INTO datetime (name, value, vtype) 
        VALUES ('timezone', 'ChinaStandardTime-8', 'STRING');
INSERT INTO datetime (name, value, vtype) 
        VALUES ('use_ntp', '0', 'BOOLEAN');
INSERT INTO datetime (name, value, vtype) 
        VALUES ('ntp_server', 'pool.ntp.org', 'STRING');
/************************************************
 * video table                                  *
 ************************************************/
CREATE TABLE IF NOT EXISTS video (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT,
        vtype    TEXT NOT NULL
        );
INSERT INTO video (name, value, vtype) 
        VALUES ('flip', '1', 'BOOLEAN');
INSERT INTO video (name, value, vtype) 
        VALUES ('mirror', '1', 'BOOLEAN');
INSERT INTO video (name, value, vtype) 
        VALUES ('profile', 'baseline', 'STRING');
/* Main profile */
INSERT INTO video (name, value, vtype) 
        VALUES ('master:frame_rate', '25', 'INTEGER');
INSERT INTO video (name, value, vtype) 
        VALUES ('master:bit_rate', 'CBR', 'STRING');
INSERT INTO video (name, value, vtype) 
        VALUES ('master:bit_rate_value', '2048', 'INTEGER');
INSERT INTO video (name, value, vtype) 
        VALUES ('master:resolution', '720P', 'STRING');
INSERT INTO video (name, value, vtype) 
        VALUES ('master:stream_path', '1/<resolution>', 'STRING');
/* Sub profile */
INSERT INTO video (name, value, vtype) 
        VALUES ('slave:frame_rate', '25', 'INTEGER');
INSERT INTO video (name, value, vtype) 
        VALUES ('slave:bit_rate', 'CBR', 'STRING');
INSERT INTO video (name, value, vtype) 
        VALUES ('slave:bit_rate_value', '1024', 'INTEGER');
INSERT INTO video (name, value, vtype) 
        VALUES ('slave:resolution', 'D1', 'STRING');
INSERT INTO video (name, value, vtype) 
        VALUES ('slave:stream_path', '1/<resolution>', 'STRING');
/************************************************
 * image table                                  *
 ************************************************/
CREATE TABLE IF NOT EXISTS image (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT,
        vtype    TEXT NOT NULL
        );
INSERT INTO image (name, value, vtype) 
        VALUES ('watermark', '0', 'BOOLEAN');
INSERT INTO image (name, value, vtype) 
        VALUES ('3ddnr', '0', 'BOOLEAN');
INSERT INTO image (name, value, vtype) 
        VALUES ('brightness', '128', 'INTEGER');
INSERT INTO image (name, value, vtype) 
        VALUES ('chrominance', '128', 'INTEGER');
INSERT INTO image (name, value, vtype) 
        VALUES ('contrast', '128', 'INTEGER');
INSERT INTO image (name, value, vtype) 
        VALUES ('saturation', '128', 'INTEGER');
INSERT INTO image (name, value, vtype) 
        VALUES ('scenario', '50Hz', 'STRING');
/************************************************
 * privacy_block table                          *
 ************************************************/
CREATE TABLE IF NOT EXISTS privacy_block (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        enable   INTEGER NOT NULL,
        left     INTEGER NOT NULL,
        top      INTEGER NOT NULL,
        width    INTEGER NOT NULL,
        height   INTEGER NOT NULL,
        color    INTEGER NOT NULL
        );
INSERT INTO privacy_block (name, enable, left, top, width, height, color) 
        VALUES ('region1', 0, 0, 0, 67, 90, 0);
INSERT INTO privacy_block (name, enable, left, top, width, height, color) 
        VALUES ('region2', 0, 0, 0, 67, 90, 0);
/************************************************
 * day_night_mode table                         *
 ************************************************/
CREATE TABLE IF NOT EXISTS day_night_mode (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    INTEGER NOT NULL
        );
INSERT INTO day_night_mode (name, value) 
        VALUES ('force_night_mode', 0);
INSERT INTO day_night_mode (name, value) 
        VALUES ('night_mode_threshold', 30);
INSERT INTO day_night_mode (name, value) 
        VALUES ('ir_intensity', 50);
/************************************************
 * osd table                                    *
 ************************************************/
CREATE TABLE IF NOT EXISTS osd (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        isshow   BOOLEAN,
        size     INTEGER,
        left     INTEGER,
        top      INTEGER,
        color    INTEGER
        );
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('master:datetime', 1, 28, 10, 10, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('master:device_name', 1, 28, 10, 940, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('master:comment', 0, 28, 10, 42, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('master:frame_rate', 1, 28, 10, 50, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('master:bit_rate', 1, 28, 85, 50, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('slave:datetime', 1, 24, 10, 10, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('slave:device_name', 1, 24, 10, 940, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('slave:comment', 0, 24, 10, 42, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('slave:frame_rate', 1, 24, 10, 55, 0);
INSERT INTO osd (name, isshow, size, left, top, color) 
        VALUES ('slave:bit_rate', 1, 24, 130, 55, 0);
/************************************************
 * szyc table                               *
 ************************************************/
CREATE TABLE IF NOT EXISTS szyc (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT
        );
INSERT INTO szyc (name, value) 
        VALUES ('train_num', '000000');
INSERT INTO szyc (name, value) 
        VALUES ('carriage_num', '1');
INSERT INTO szyc (name, value) 
        VALUES ('position_num', '1');
/************************************************
 * network table                                *
 ************************************************/
CREATE TABLE IF NOT EXISTS network (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT NOT NULL
        );
INSERT INTO network (name, value) 
        VALUES ('method', 'static');
/************************************************
 * network_static table                         *
 ************************************************/
CREATE TABLE IF NOT EXISTS network_static (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT
        );
INSERT INTO network_static (name, value) 
        VALUES ('ipaddr', '192.168.1.217');
INSERT INTO network_static (name, value) 
        VALUES ('netmask', '255.255.255.0');
INSERT INTO network_static (name, value) 
        VALUES ('gateway', '192.168.1.1');
INSERT INTO network_static (name, value) 
        VALUES ('dns1', '192.168.1.1');
INSERT INTO network_static (name, value) 
        VALUES ('dns2', '');
/************************************************
 * network_pppoe table                          *
 ************************************************/
CREATE TABLE IF NOT EXISTS network_pppoe (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    TEXT
        );
INSERT INTO network_pppoe (name, value) 
        VALUES ('username', '');
INSERT INTO network_pppoe (name, value) 
        VALUES ('password', '');
/************************************************
 * network_port table                           *
 ************************************************/
CREATE TABLE IF NOT EXISTS network_port (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        value    INTEGER UNIQUE NOT NULL
        );
INSERT INTO network_port (name, value) 
        VALUES ('http', 80);
INSERT INTO network_port (name, value) 
        VALUES ('ftp', 21);
INSERT INTO network_port (name, value) 
        VALUES ('rtsp', 554);
/************************************************
 * event_input table                            *
 ************************************************/
CREATE TABLE IF NOT EXISTS event_input (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        enable   INTEGER NOT NULL,
        mon      TEXT NOT NULL,
        tue      TEXT NOT NULL,
        wed      TEXT NOT NULL,
        thu      TEXT NOT NULL,
        fri      TEXT NOT NULL,
        sat      TEXT NOT NULL,
        sun      TEXT NOT NULL
        );
INSERT INTO event_input (name, enable, mon, tue, wed, thu, fri, sat, sun) 
        VALUES ('input1', 0, '', '', '', '', '', '', '');
/************************************************
 * event_output table                           *
 ************************************************/
CREATE TABLE IF NOT EXISTS event_output (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        name     TEXT UNIQUE NOT NULL,
        normal   TEXT NOT NULL,
        period   INTEGER NOT NULL
        );
INSERT INTO event_output (name, normal, period) 
        VALUES ('output1', 'open', 10);
/************************************************
 * event_motion table                           *
 ************************************************/
CREATE TABLE IF NOT EXISTS event_motion (
        id            INTEGER PRIMARY KEY AUTOINCREMENT,
        name          TEXT UNIQUE NOT NULL,
        enable        INTEGER NOT NULL,
        sensitivity   INTEGER NOT NULL,
        left          INTEGER NOT NULL,
        top           INTEGER NOT NULL,
        width         INTEGER NOT NULL,
        height        INTEGER NOT NULL,
        mon           TEXT NOT NULL,
        tue           TEXT NOT NULL,
        wed           TEXT NOT NULL,
        thu           TEXT NOT NULL,
        fri           TEXT NOT NULL,
        sat           TEXT NOT NULL,
        sun           TEXT NOT NULL
        );
INSERT INTO event_motion (name, enable, sensitivity, left, top, width, height, mon, tue, wed, thu, fri, sat, sun) 
        VALUES ('region1', 0, 50, 0, 0, 67, 90, '', '', '', '', '', '', '');
INSERT INTO event_motion (name, enable, sensitivity, left, top, width, height, mon, tue, wed, thu, fri, sat, sun) 
        VALUES ('region2', 0, 50, 0, 0, 67, 90, '', '', '', '', '', '', '');
/************************************************
 * event_cover table                            *
 ************************************************/
CREATE TABLE IF NOT EXISTS event_cover (
        id            INTEGER PRIMARY KEY AUTOINCREMENT,
        name          TEXT UNIQUE NOT NULL,
        enable        INTEGER NOT NULL,
        sensitivity   INTEGER NOT NULL,
        left          INTEGER NOT NULL,
        top           INTEGER NOT NULL,
        width         INTEGER NOT NULL,
        height        INTEGER NOT NULL,
        mon           TEXT NOT NULL,
        tue           TEXT NOT NULL,
        wed           TEXT NOT NULL,
        thu           TEXT NOT NULL,
        fri           TEXT NOT NULL,
        sat           TEXT NOT NULL,
        sun           TEXT NOT NULL
        );
INSERT INTO event_cover (name, enable, sensitivity, left, top, width, height, mon, tue, wed, thu, fri, sat, sun) 
        VALUES ('region1', 1, 60, 0, 0, 1000, 1000, '', '', '', '', '', '', '');
INSERT INTO event_cover (name, enable, sensitivity, left, top, width, height, mon, tue, wed, thu, fri, sat, sun) 
        VALUES ('region2', 0, 50, 0, 0, 67, 90, '', '', '', '', '', '', '');
/************************************************
 * event_proc table                             *
 ************************************************/
CREATE TABLE IF NOT EXISTS event_proc (
        id            INTEGER PRIMARY KEY AUTOINCREMENT,
        name          TEXT UNIQUE NOT NULL,
        record        INTEGER NOT NULL,
        sound         INTEGER NOT NULL,
        output1       INTEGER NOT NULL
        );
INSERT INTO event_proc (name, record, sound, output1) 
        VALUES ('input1', 0, 0, 0);
INSERT INTO event_proc (name, record, sound, output1) 
        VALUES ('motion', 0, 0, 0);
INSERT INTO event_proc (name, record, sound, output1) 
        VALUES ('cover', 0, 0, 0);
/************************************************
 * record_schedule table                        *
 ************************************************/
CREATE TABLE IF NOT EXISTS record_schedule (
        id            INTEGER PRIMARY KEY AUTOINCREMENT,
        mon           TEXT NOT NULL,
        tue           TEXT NOT NULL,
        wed           TEXT NOT NULL,
        thu           TEXT NOT NULL,
        fri           TEXT NOT NULL,
        sat           TEXT NOT NULL,
        sun           TEXT NOT NULL
        );
INSERT INTO record_schedule (mon, tue, wed, thu, fri, sat, sun) 
        VALUES ('', '', '', '', '', '', '');
/************************************************
 * record_strategy table                        *
 ************************************************/
CREATE TABLE IF NOT EXISTS record_strategy (
        id            INTEGER PRIMARY KEY AUTOINCREMENT,
        name          TEXT UNIQUE NOT NULL,
        value         TEXT,
        vtype         TEXT NOT NULL
        );
INSERT INTO record_strategy (name, value, vtype) 
        VALUES ('nr_file_switch', 'size', 'STRING');
INSERT INTO record_strategy (name, value, vtype) 
        VALUES ('nr_file_size', '50', 'INTEGER');
INSERT INTO record_strategy (name, value, vtype) 
        VALUES ('nr_file_period', '10', 'INTEGER');
INSERT INTO record_strategy (name, value, vtype) 
        VALUES ('er_file_period', '10', 'INTEGER');
INSERT INTO record_strategy (name, value, vtype) 
        VALUES ('storage_full', 'stop', 'STRING');
