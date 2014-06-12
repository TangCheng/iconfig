#ifndef __ICONFIG_H__
#define __ICONFIG_H__

#include <base_app.h>

#define IPCAM_ICONFIG_TYPE (ipcam_iconfig_get_type())
#define IPCAM_ICONFIG(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_ICONFIG_TYPE, IpcamIConfig))
#define IPCAM_ICONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_ICONFIG_TYPE, IpcamIConfigClass))
#define IPCAM_IS_ICONFIG(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_ICONFIG_TYPE))
#define IPCAM_IS_ICONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_ICONFIG_TYPE))
#define IPCAM_ICONFIG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_ICONFIG_TYPE, IpcamIConfigClass))

typedef struct _IpcamIConfig IpcamIConfig;
typedef struct _IpcamIConfigClass IpcamIConfigClass;

struct _IpcamIConfig
{
    IpcamBaseApp parent;
};

struct _IpcamIConfigClass
{
    IpcamBaseAppClass parent_class;
};

GType ipcam_iconfig_get_type(void);
gchar *ipcam_iconfig_get_base_info(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_base_info(IpcamIConfig *iconfig, const char *name, const char *value);
gboolean ipcam_iconfig_get_osd(IpcamIConfig *iconfig, const char *name, gboolean *isshow,
                             guint *size, guint *x, guint *y, guint *color);
void ipcam_iconfig_set_osd(IpcamIConfig *iconfig, const gchar *name, gboolean isshow,
                           guint size, guint x, guint y, guint color);
gint ipcam_iconfig_get_video(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_video(IpcamIConfig *iconfig, const char *name, gint value);
gint ipcam_iconfig_get_scene(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_scene(IpcamIConfig *iconfig, const gchar *name, gint value);

gint ipcam_iconfig_get_network(IpcamIConfig *iconfig, gchar *name);
void ipcam_iconfig_set_network(IpcamIConfig *iconfig, gchar *name, guint value);
gchar *ipcam_iconfig_get_network_static(IpcamIConfig *iconfig, gchar *name);
void ipcam_iconfig_set_network_static(IpcamIConfig *iconfig, gchar *name, gchar *value);
gchar *ipcam_iconfig_get_network_pppoe(IpcamIConfig *iconfig, gchar *name);
void ipcam_iconfig_set_network_pppoe(IpcamIConfig *iconfig, gchar *name, gchar *value);
gint ipcam_iconfig_get_network_port(IpcamIConfig *iconfig, gchar *name);
void ipcam_iconfig_set_network_port(IpcamIConfig *iconfig, gchar *name, guint value);
void ipcam_iconfig_set_datetime(IpcamIConfig *iconfig, gchar *name, guint int_value, gchar *str_value);
void ipcam_iconfig_get_datetime(IpcamIConfig *iconfig, gchar *name, guint *int_value, gchar **str_value);
GList *ipcam_iconfig_get_users(IpcamIConfig *iconfig);
void ipcam_iconfig_set_user_password(IpcamIConfig *iconfig, gchar *username, gchar *password);
gchar *ipcam_iconfig_get_user_password(IpcamIConfig *iconfig, gchar *username);
void ipcam_iconfig_set_user_privilege(IpcamIConfig *iconfig, gchar *username, gboolean isadmin);
gboolean ipcam_iconfig_get_user_privilege(IpcamIConfig *iconfig, gchar *username);
void ipcam_iconfig_del_user(IpcamIConfig *iconfig, gchar *username);

#endif /* __ICONFIG_H__ */
