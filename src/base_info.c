#include "base_info.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_VALUE,
  N_PROPERTIES
};

typedef struct _IpcamBaseInfoPrivate
{
    guint id;
    gchar *name;
    gchar *value;
} IpcamBaseInfoPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamBaseInfo, ipcam_base_info, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_base_info_finalize(GObject *object)
{
    IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(IPCAM_BASE_INFO(object));
    g_free(priv->name);
    g_free(priv->value);
    G_OBJECT_CLASS(ipcam_base_info_parent_class)->finalize(object);
}
static void ipcam_base_info_set_property(GObject      *object,
                                         guint        property_id,
                                         const GValue *value,
                                         GParamSpec   *pspec)
{
    IpcamBaseInfo *self = IPCAM_BASE_INFO(object);
    switch(property_id)
    {
    case PROP_ID:
        {
            IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(self);
            priv->id = g_value_get_int(value);
        }
        break;
    case PROP_NAME:
        {
            IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(self);
            g_free(priv->name);
            priv->name = g_value_dup_string(value);
        }
        break;
    case PROP_VALUE:
        {
            IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(self);
            g_free(priv->value);
            priv->value = g_value_dup_string(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_base_info_get_property(GObject    *object,
                                         guint       property_id,
                                         GValue     *value,
                                         GParamSpec *pspec)
{
    IpcamBaseInfo *self = IPCAM_BASE_INFO(object);
    switch(property_id)
    {
    case PROP_ID:
        {
            IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(self);
            g_value_set_int(value, priv->id);
        }
        break;
    case PROP_NAME:
        {
            IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(self);
            g_value_set_string(value, priv->name);
        }
        break;
    case PROP_VALUE:
        {
            IpcamBaseInfoPrivate *priv = ipcam_base_info_get_instance_private(self);
            g_value_set_string(value, priv->value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_base_info_init(IpcamBaseInfo *self)
{
}
static void ipcam_base_info_class_init(IpcamBaseInfoClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_base_info_set_property;
    object_class->get_property = &ipcam_base_info_get_property;
    object_class->finalize = &ipcam_base_info_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "base_info");
    gom_resource_class_set_primary_key(resource_class, "_id");
  
    obj_properties[PROP_ID] =
        g_param_spec_int("_id",
                         "ID",
                         "The ID for the basic information.",
                         0,
                         65535,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_NAME] =
        g_param_spec_string("name",
                            "Name",
                            "Base information name.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_VALUE] =
        g_param_spec_string("value",
                            "Value",
                            "Base information value.",
                            NULL, // default value
                            G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}
