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
GVariant *ipcam_iconfig_get_video(IpcamIConfig *iconfig, const gchar *profile, const gchar *name);
void ipcam_iconfig_set_video(IpcamIConfig *iconfig, const gchar *profile, const gchar *name, GVariant *value);

GVariant *ipcam_iconfig_get_image(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_image(IpcamIConfig *iconfig, const gchar *name, GBytes *value);

gchar *ipcam_iconfig_get_network(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_network(IpcamIConfig *iconfig, const gchar *name, const gchar *value);
gchar *ipcam_iconfig_get_network_static(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_network_static(IpcamIConfig *iconfig, const gchar *name, gchar *value);
gchar *ipcam_iconfig_get_network_pppoe(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_network_pppoe(IpcamIConfig *iconfig, const gchar *name, gchar *value);
gint ipcam_iconfig_get_network_port(IpcamIConfig *iconfig, const gchar *name);
void ipcam_iconfig_set_network_port(IpcamIConfig *iconfig, const gchar *name, guint value);
void ipcam_iconfig_set_datetime(IpcamIConfig *iconfig, const gchar *name, guint int_value, gchar *str_value);
void ipcam_iconfig_get_datetime(IpcamIConfig *iconfig, const gchar *name, guint *int_value, gchar **str_value);
GList *ipcam_iconfig_get_users(IpcamIConfig *iconfig);
void ipcam_iconfig_set_user_password(IpcamIConfig *iconfig, const gchar *username, gchar *password);
gchar *ipcam_iconfig_get_user_password(IpcamIConfig *iconfig, const gchar *username);
void ipcam_iconfig_set_user_role(IpcamIConfig *iconfig, const gchar *username, const gchar *role);
gchar *ipcam_iconfig_get_user_role(IpcamIConfig *iconfig, const gchar *username);
void ipcam_iconfig_del_user(IpcamIConfig *iconfig, const gchar *username);

#endif /* __ICONFIG_H__ */
