#include "privacy_block.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_ENABLE,
  PROP_LEFT,
  PROP_TOP,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_COLOR,
  N_PROPERTIES
};

typedef struct _IpcamPrivacyBlockPrivate
{
    guint id;
    gchar *name;
    gboolean enable;
    guint left;
    guint top;
    guint width;
    guint height;
    guint32 color;
} IpcamPrivacyBlockPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamPrivacyBlock, ipcam_privacy_block, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_privacy_block_finalize(GObject *object)
{
    IpcamPrivacyBlockPrivate *priv = ipcam_privacy_block_get_instance_private(IPCAM_PRIVACY_BLOCK(object));
    g_free(priv->name);
    G_OBJECT_CLASS(ipcam_privacy_block_parent_class)->finalize(object);
}
static void ipcam_privacy_block_set_property(GObject      *object,
                                   guint        property_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
    IpcamPrivacyBlock *self = IPCAM_PRIVACY_BLOCK(object);
    IpcamPrivacyBlockPrivate *priv = ipcam_privacy_block_get_instance_private(self);
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
    case PROP_ENABLE:
        {
            priv->enable = g_value_get_boolean(value);
        }
        break;
    case PROP_LEFT:
        {
            priv->left = g_value_get_int(value);
        }
        break;
    case PROP_TOP:
        {
            priv->top = g_value_get_int(value);
        }
        break;
    case PROP_WIDTH:
        {
            priv->width = g_value_get_int(value);
        }
        break;
    case PROP_HEIGHT:
        {
            priv->height = g_value_get_int(value);
        }
        break;
    case PROP_COLOR:
        {
            priv->color = g_value_get_uint(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_privacy_block_get_property(GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
    IpcamPrivacyBlock *self = IPCAM_PRIVACY_BLOCK(object);
    IpcamPrivacyBlockPrivate *priv = ipcam_privacy_block_get_instance_private(self);
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
    case PROP_ENABLE:
        {
            g_value_set_boolean(value, priv->enable);
        }
        break;
    case PROP_LEFT:
        {
            g_value_set_int(value, priv->left);
        }
        break;
    case PROP_TOP:
        {
            g_value_set_int(value, priv->top);
        }
        break;
    case PROP_WIDTH:
        {
            g_value_set_int(value, priv->width);
        }
        break;
    case PROP_HEIGHT:
        {
            g_value_set_int(value, priv->height);
            break;
        }
    case PROP_COLOR:
        {
            g_value_set_uint(value, priv->color);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_privacy_block_init(IpcamPrivacyBlock *self)
{
}
static void ipcam_privacy_block_class_init(IpcamPrivacyBlockClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_privacy_block_set_property;
    object_class->get_property = &ipcam_privacy_block_get_property;
    object_class->finalize = &ipcam_privacy_block_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "privacy_block");
  
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
    obj_properties[PROP_ENABLE] =
        g_param_spec_boolean("enable",
                             "Enable?",
                             "Is this region enabled?",
                             FALSE,
                             G_PARAM_READWRITE);
    obj_properties[PROP_LEFT] =
        g_param_spec_int("left",
                         "Left",
                         "region left position",
                         0,
                         1000,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_TOP] =
        g_param_spec_int("top",
                         "Top",
                         "region top position",
                         0,
                         1000,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_WIDTH] =
        g_param_spec_int("width",
                         "Width",
                         "region width",
                         0,
                         1000,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_HEIGHT] =
        g_param_spec_int("height",
                         "Height",
                         "region height",
                         0,
                         1000,
                         0, // default value
                         G_PARAM_READWRITE);
    obj_properties[PROP_COLOR] =
        g_param_spec_uint("color",
                         "privacy block region color",
                         "Color of the privacy block region.",
                         0,
                         G_MAXUINT,
                         0, // default value
                         G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "enable");
    gom_resource_class_set_notnull(resource_class, "left");
    gom_resource_class_set_notnull(resource_class, "top");
    gom_resource_class_set_notnull(resource_class, "width");
    gom_resource_class_set_notnull(resource_class, "height");
    gom_resource_class_set_notnull(resource_class, "color");
}
