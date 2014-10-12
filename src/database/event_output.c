#include "event_output.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_NORMAL,
  PROP_PERIOD,
  N_PROPERTIES
};

typedef struct _IpcamEventOutputPrivate
{
    guint id;
    gchar *name;
    gchar *normal;
    guint period;
} IpcamEventOutputPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamEventOutput, ipcam_event_output, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_event_output_finalize(GObject *object)
{
    IpcamEventOutputPrivate *priv = ipcam_event_output_get_instance_private(IPCAM_EVENT_OUTPUT(object));
    g_free(priv->name);
    g_free(priv->normal);
    G_OBJECT_CLASS(ipcam_event_output_parent_class)->finalize(object);
}
static void ipcam_event_output_set_property(GObject      *object,
                                           guint        property_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
    IpcamEventOutput *self = IPCAM_EVENT_OUTPUT(object);
    IpcamEventOutputPrivate *priv = ipcam_event_output_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        priv->id = g_value_get_uint(value);
        break;
    case PROP_NAME:
        g_free(priv->name);
        priv->name = g_value_dup_string(value);
        break;
    case PROP_NORMAL:
        g_free(priv->normal);
        priv->normal = g_value_dup_string(value);
        break;
    case PROP_PERIOD:
        priv->period = g_value_get_uint(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_event_output_get_property(GObject    *object,
                                           guint       property_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
    IpcamEventOutput *self = IPCAM_EVENT_OUTPUT(object);
    IpcamEventOutputPrivate *priv = ipcam_event_output_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        g_value_set_uint(value, priv->id);
        break;
    case PROP_NAME:
        g_value_set_string(value, priv->name);
        break;
    case PROP_NORMAL:
        g_value_set_string(value, priv->normal);
        break;
    case PROP_PERIOD:
        g_value_set_uint(value, priv->period);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_event_output_init(IpcamEventOutput *self)
{
}
static void ipcam_event_output_class_init(IpcamEventOutputClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_event_output_set_property;
    object_class->get_property = &ipcam_event_output_get_property;
    object_class->finalize = &ipcam_event_output_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "event_output");
  
    obj_properties[PROP_ID] =
        g_param_spec_uint("id",
                          "ID",
                          "The ID for the network pppoe parameter.",
                          0,
                          G_MAXUINT,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_NAME] =
        g_param_spec_string("name",
                            "Name",
                            "output name.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_NORMAL] =
        g_param_spec_string("normal",
                            "normal state",
                            "output port normal state",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_PERIOD] =
        g_param_spec_uint("period",
                          "output period",
                          "output port trigger state period",
                          0,
                          G_MAXUINT,
                          0, // default value
                          G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "normal");
    gom_resource_class_set_notnull(resource_class, "period");
}
