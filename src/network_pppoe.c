#include "network_pppoe.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_VALUE,
  N_PROPERTIES
};

typedef struct _IpcamNetworkPPPoEPrivate
{
    guint id;
    gchar *name;
    gchar *value;
} IpcamNetworkPPPoEPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamNetworkPPPoE, ipcam_network_pppoe, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_network_pppoe_finalize(GObject *object)
{
    IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(IPCAM_NETWORK_PPPOE(object));
    g_free(priv->name);
    g_free(priv->value);
    G_OBJECT_CLASS(ipcam_network_pppoe_parent_class)->finalize(object);
}
static void ipcam_network_pppoe_set_property(GObject      *object,
                                         guint        property_id,
                                         const GValue *value,
                                         GParamSpec   *pspec)
{
    IpcamNetworkPPPoE *self = IPCAM_NETWORK_PPPOE(object);
    switch(property_id)
    {
    case PROP_ID:
        {
            IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(self);
            priv->id = g_value_get_int(value);
        }
        break;
    case PROP_NAME:
        {
            IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(self);
            g_free(priv->name);
            priv->name = g_value_dup_string(value);
        }
        break;
    case PROP_VALUE:
        {
            IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(self);
            g_free(priv->value);
            priv->value = g_value_dup_string(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_network_pppoe_get_property(GObject    *object,
                                         guint       property_id,
                                         GValue     *value,
                                         GParamSpec *pspec)
{
    IpcamNetworkPPPoE *self = IPCAM_NETWORK_PPPOE(object);
    switch(property_id)
    {
    case PROP_ID:
        {
            IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(self);
            g_value_set_int(value, priv->id);
        }
        break;
    case PROP_NAME:
        {
            IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(self);
            g_value_set_string(value, priv->name);
        }
        break;
    case PROP_VALUE:
        {
            IpcamNetworkPPPoEPrivate *priv = ipcam_network_pppoe_get_instance_private(self);
            g_value_set_string(value, priv->value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_network_pppoe_init(IpcamNetworkPPPoE *self)
{
}
static void ipcam_network_pppoe_class_init(IpcamNetworkPPPoEClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_network_pppoe_set_property;
    object_class->get_property = &ipcam_network_pppoe_get_property;
    object_class->finalize = &ipcam_network_pppoe_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "network_pppoe");
  
    obj_properties[PROP_ID] =
        g_param_spec_int("id",
                         "ID",
                         "The ID for the network pppoe parameter.",
                         0,
                         65535,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_NAME] =
        g_param_spec_string("name",
                            "Name",
                            "PPPoE username.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_VALUE] =
        g_param_spec_string("value",
                            "Value",
                            "PPPoE password.",
                            NULL, // default value
                            G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
}
