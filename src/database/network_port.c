#include "network_port.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_VALUE,
  N_PROPERTIES
};

typedef struct _IpcamNetworkPortPrivate
{
    guint id;
    gchar *name;
    guint value;
} IpcamNetworkPortPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamNetworkPort, ipcam_network_port, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_network_port_finalize(GObject *object)
{
    IpcamNetworkPortPrivate *priv = ipcam_network_port_get_instance_private(IPCAM_NETWORK_PORT(object));
    g_free(priv->name);
    G_OBJECT_CLASS(ipcam_network_port_parent_class)->finalize(object);
}
static void ipcam_network_port_set_property(GObject      *object,
                                            guint        property_id,
                                            const GValue *value,
                                            GParamSpec   *pspec)
{
    IpcamNetworkPort *self = IPCAM_NETWORK_PORT(object);
    IpcamNetworkPortPrivate *priv = ipcam_network_port_get_instance_private(self);
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
static void ipcam_network_port_get_property(GObject    *object,
                                            guint       property_id,
                                            GValue     *value,
                                            GParamSpec *pspec)
{
    IpcamNetworkPort *self = IPCAM_NETWORK_PORT(object);
    IpcamNetworkPortPrivate *priv = ipcam_network_port_get_instance_private(self);
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
static void ipcam_network_port_init(IpcamNetworkPort *self)
{
}
static void ipcam_network_port_class_init(IpcamNetworkPortClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_network_port_set_property;
    object_class->get_property = &ipcam_network_port_get_property;
    object_class->finalize = &ipcam_network_port_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "network_port");
  
    obj_properties[PROP_ID] =
        g_param_spec_uint("id",
                          "ID",
                          "The ID for the network port parameter.",
                          0,
                          G_MAXUINT,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_NAME] =
        g_param_spec_string("name",
                            "Parameter Name",
                            "Network port parameter name.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_VALUE] =
        g_param_spec_uint("value",
                          "Parameter value",
                          "Network port parameter value.",
                          0,
                          65535,
                          0, // default value
                          G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_unique(resource_class, "value");
    gom_resource_class_set_notnull(resource_class, "value");
}
