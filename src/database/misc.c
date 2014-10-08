#include "misc.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_VALUE,
  PROP_VTYPE,
  N_PROPERTIES
};

typedef struct _IpcamMiscPrivate
{
    guint id;
    gchar *name;
    gchar *value;
    gchar *vtype;
} IpcamMiscPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamMisc, ipcam_misc, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_misc_finalize(GObject *object)
{
    IpcamMiscPrivate *priv = ipcam_misc_get_instance_private(IPCAM_MISC(object));
    g_free(priv->name);
    g_free(priv->value);
    g_free(priv->vtype);
    G_OBJECT_CLASS(ipcam_misc_parent_class)->finalize(object);
}
static void ipcam_misc_set_property(GObject      *object,
                                        guint        property_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
    IpcamMisc *self = IPCAM_MISC(object);
    IpcamMiscPrivate *priv = ipcam_misc_get_instance_private(self);
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
    case PROP_VALUE:
        {
            g_free(priv->value);
            priv->value = g_value_dup_string(value);
        }
        break;
    case PROP_VTYPE:
        {
            g_free(priv->vtype);
            priv->vtype = g_value_dup_string(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_misc_get_property(GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
    IpcamMisc *self = IPCAM_MISC(object);
    IpcamMiscPrivate *priv = ipcam_misc_get_instance_private(self);
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
    case PROP_VALUE:
        {
            g_value_set_string(value, priv->value);
        }
        break;
    case PROP_VTYPE:
        {
            g_value_set_string(value, priv->vtype);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_misc_init(IpcamMisc *self)
{
}
static void ipcam_misc_class_init(IpcamMiscClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_misc_set_property;
    object_class->get_property = &ipcam_misc_get_property;
    object_class->finalize = &ipcam_misc_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "misc");
  
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
                            "Parameter Name",
                            "Misc parameter name.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_VALUE] =
        g_param_spec_string("value",
                            "Parameter value",
                            "Misc parameter value.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_VTYPE] =
        g_param_spec_string("vtype",
                            "Parameter value",
                            "Misc parameter value.",
                            NULL, // default value
                            G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "vtype");
}
