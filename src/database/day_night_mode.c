#include "day_night_mode.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_VALUE,
  N_PROPERTIES
};

typedef struct _IpcamDayNightModePrivate
{
    guint id;
    gchar *name;
    guint value;
} IpcamDayNightModePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamDayNightMode, ipcam_day_night_mode, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_day_night_mode_finalize(GObject *object)
{
    IpcamDayNightModePrivate *priv = ipcam_day_night_mode_get_instance_private(IPCAM_DAY_NIGHT_MODE(object));
    g_free(priv->name);
    G_OBJECT_CLASS(ipcam_day_night_mode_parent_class)->finalize(object);
}
static void ipcam_day_night_mode_set_property(GObject      *object,
                                   guint        property_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
    IpcamDayNightMode *self = IPCAM_DAY_NIGHT_MODE(object);
    IpcamDayNightModePrivate *priv = ipcam_day_night_mode_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        {
            priv->id = g_value_get_uint(value);
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
            priv->value = g_value_get_uint(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_day_night_mode_get_property(GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
    IpcamDayNightMode *self = IPCAM_DAY_NIGHT_MODE(object);
    IpcamDayNightModePrivate *priv = ipcam_day_night_mode_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        {
            g_value_set_uint(value, priv->id);
        }
        break;
    case PROP_NAME:
        {
            g_value_set_string(value, priv->name);
        }
        break;
    case PROP_VALUE:
        {
            g_value_set_uint(value, priv->value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_day_night_mode_init(IpcamDayNightMode *self)
{
}
static void ipcam_day_night_mode_class_init(IpcamDayNightModeClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_day_night_mode_set_property;
    object_class->get_property = &ipcam_day_night_mode_get_property;
    object_class->finalize = &ipcam_day_night_mode_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "day_night_mode");
  
    obj_properties[PROP_ID] =
        g_param_spec_uint("id",
                          "ID",
                          "The ID for the user.",
                          0,
                          G_MAXUINT,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_NAME] =
        g_param_spec_string("name",
                            "Name",
                            "name.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_VALUE] =
        g_param_spec_uint("value",
                          "value",
                          "value",
                          0,
                          100,
                          0, // default value
                          G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
}
