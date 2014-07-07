#include "users.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_PASSWORD,
  PROP_PRIVILEGE,
  N_PROPERTIES
};

typedef struct _IpcamUsersPrivate
{
    guint id;
    gchar *name;
    gchar *password;
    guint privilege;
} IpcamUsersPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamUsers, ipcam_users, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_users_finalize(GObject *object)
{
    IpcamUsersPrivate *priv = ipcam_users_get_instance_private(IPCAM_USERS(object));
    g_free(priv->name);
    g_free(priv->password);
    G_OBJECT_CLASS(ipcam_users_parent_class)->finalize(object);
}
static void ipcam_users_set_property(GObject      *object,
                                     guint        property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
    IpcamUsers *self = IPCAM_USERS(object);
    IpcamUsersPrivate *priv = ipcam_users_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        {
            priv->id = g_value_get_int(value);
        }
        break;
    case PROP_NAME:
        {
            g_free(priv->name);
            priv->name = g_value_dup_string(value);
        }
        break;
    case PROP_PASSWORD:
        {
            g_free(priv->password);
            priv->password = g_value_dup_string(value);
        }
        break;
    case PROP_PRIVILEGE:
        {
            priv->privilege = g_value_get_uint(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_users_get_property(GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    IpcamUsers *self = IPCAM_USERS(object);
    IpcamUsersPrivate *priv = ipcam_users_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        {
            g_value_set_int(value, priv->id);
        }
        break;
    case PROP_NAME:
        {
            g_value_set_string(value, priv->name);
        }
        break;
    case PROP_PASSWORD:
        {
            g_value_set_string(value, priv->password);
        }
        break;
    case PROP_PRIVILEGE:
        {
            g_value_set_uint(value, priv->privilege);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_users_init(IpcamUsers *self)
{
}
static void ipcam_users_class_init(IpcamUsersClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_users_set_property;
    object_class->get_property = &ipcam_users_get_property;
    object_class->finalize = &ipcam_users_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "users");
  
    obj_properties[PROP_ID] =
        g_param_spec_int("id",
                         "ID",
                         "The ID for the user.",
                         0,
                         65535,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_NAME] =
        g_param_spec_string("name",
                            "Name",
                            "Username.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_PASSWORD] =
        g_param_spec_string("password",
                            "Password",
                            "User's password.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_PRIVILEGE] =
        g_param_spec_uint("privilege",
                          "User Privilege Level",
                          "User Privilege Level",
                          0,
                          65536,
                          0,
                          G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "password");
}
