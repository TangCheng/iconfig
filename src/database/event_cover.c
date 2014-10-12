#include "event_cover.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_ENABLE,
  PROP_SENSITIVITY,
  PROP_LEFT,
  PROP_TOP,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_MON,
  PROP_TUE,
  PROP_WED,
  PROP_THU,
  PROP_FRI,
  PROP_SAT,
  PROP_SUN,
  N_PROPERTIES
};

typedef struct _IpcamEventCoverPrivate
{
    guint id;
    gchar *name;
    gboolean enable;
    guint sensitivity;
    guint left;
    guint top;
    guint width;
    guint height;
    gchar *monday;
    gchar *tuesday;
    gchar *wednesday;
    gchar *thursday;
    gchar *friday;
    gchar *saturday;
    gchar *sunday;
} IpcamEventCoverPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamEventCover, ipcam_event_cover, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_event_cover_finalize(GObject *object)
{
    IpcamEventCoverPrivate *priv = ipcam_event_cover_get_instance_private(IPCAM_EVENT_COVER(object));
    g_free(priv->name);
    g_free(priv->monday);
    g_free(priv->tuesday);
    g_free(priv->wednesday);
    g_free(priv->thursday);
    g_free(priv->friday);
    g_free(priv->saturday);
    g_free(priv->sunday);
    G_OBJECT_CLASS(ipcam_event_cover_parent_class)->finalize(object);
}
static void ipcam_event_cover_set_property(GObject      *object,
                                           guint        property_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
    IpcamEventCover *self = IPCAM_EVENT_COVER(object);
    IpcamEventCoverPrivate *priv = ipcam_event_cover_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        priv->id = g_value_get_uint(value);
        break;
    case PROP_NAME:
        g_free(priv->name);
        priv->name = g_value_dup_string(value);
        break;
    case PROP_ENABLE:
        priv->enable = g_value_get_boolean(value);
        break;
    case PROP_SENSITIVITY:
        priv->sensitivity = g_value_get_uint(value);
        break;
    case PROP_LEFT:
        priv->left = g_value_get_uint(value);
        break;
    case PROP_TOP:
        priv->top = g_value_get_uint(value);
        break;
    case PROP_WIDTH:
        priv->width = g_value_get_uint(value);
        break;
    case PROP_HEIGHT:
        priv->height = g_value_get_uint(value);
        break;
    case PROP_MON:
        g_free(priv->monday);
        priv->monday = g_value_dup_string(value);
        break;
    case PROP_TUE:
        g_free(priv->tuesday);
        priv->tuesday = g_value_dup_string(value);
        break;
    case PROP_WED:
        g_free(priv->wednesday);
        priv->wednesday = g_value_dup_string(value);
        break;
    case PROP_THU:
        g_free(priv->thursday);
        priv->thursday = g_value_dup_string(value);
        break;
    case PROP_FRI:
        g_free(priv->friday);
        priv->friday = g_value_dup_string(value);
        break;
    case PROP_SAT:
        g_free(priv->saturday);
        priv->saturday = g_value_dup_string(value);
        break;
    case PROP_SUN:
        g_free(priv->sunday);
        priv->sunday = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_event_cover_get_property(GObject    *object,
                                           guint       property_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
    IpcamEventCover *self = IPCAM_EVENT_COVER(object);
    IpcamEventCoverPrivate *priv = ipcam_event_cover_get_instance_private(self);
    switch(property_id)
    {
    case PROP_ID:
        g_value_set_uint(value, priv->id);
        break;
    case PROP_NAME:
        g_value_set_string(value, priv->name);
        break;
    case PROP_ENABLE:
        g_value_set_boolean(value, priv->enable);
        break;
    case PROP_SENSITIVITY:
        g_value_set_uint(value, priv->sensitivity);
        break;
    case PROP_LEFT:
        g_value_set_uint(value, priv->left);
        break;
    case PROP_TOP:
        g_value_set_uint(value, priv->top);
        break;
    case PROP_WIDTH:
        g_value_set_uint(value, priv->width);
        break;
    case PROP_HEIGHT:
        g_value_set_uint(value, priv->height);
        break;
    case PROP_MON:
        g_value_set_string(value, priv->monday);
        break;
    case PROP_TUE:
        g_value_set_string(value, priv->tuesday);
        break;
    case PROP_WED:
        g_value_set_string(value, priv->wednesday);
        break;
    case PROP_THU:
        g_value_set_string(value, priv->thursday);
        break;
    case PROP_FRI:
        g_value_set_string(value, priv->friday);
        break;
    case PROP_SAT:
        g_value_set_string(value, priv->saturday);
        break;
    case PROP_SUN:
        g_value_set_string(value, priv->sunday);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_event_cover_init(IpcamEventCover *self)
{
}
static void ipcam_event_cover_class_init(IpcamEventCoverClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_event_cover_set_property;
    object_class->get_property = &ipcam_event_cover_get_property;
    object_class->finalize = &ipcam_event_cover_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "event_cover");
  
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
                            "cover detect region name.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_ENABLE] =
        g_param_spec_boolean("enable",
                             "enable",
                             "Enable?",
                             FALSE, // default value
                             G_PARAM_READWRITE);
    obj_properties[PROP_SENSITIVITY] =
        g_param_spec_uint("sensitivity",
                          "sensitivity",
                          "cover detect sensitivity.",
                          0,
                          100,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_LEFT] =
        g_param_spec_uint("left",
                          "left",
                          "cover detect region left position.",
                          0,
                          1000,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_TOP] =
        g_param_spec_uint("top",
                          "top",
                          "cover detect region top position.",
                          0,
                          1000,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_WIDTH] =
        g_param_spec_uint("width",
                          "width",
                          "cover detect region width.",
                          0,
                          1000,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_HEIGHT] =
        g_param_spec_uint("height",
                          "height",
                          "cover detect region height.",
                          0,
                          1000,
                          0, // default value
                          G_PARAM_READWRITE);
    obj_properties[PROP_MON] =
        g_param_spec_string("mon",
                            "Monday",
                            "time schdule of Monday",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_TUE] =
        g_param_spec_string("tue",
                            "Tuesday",
                            "time schdule of Tuesday",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_WED] =
        g_param_spec_string("wed",
                            "Wednesday",
                            "time schdule of Wednesday",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_THU] =
        g_param_spec_string("thu",
                            "Thursday",
                            "time schdule of Thursday",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_FRI] =
        g_param_spec_string("fri",
                            "Friday",
                            "time schdule of Friday",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_SAT] =
        g_param_spec_string("sat",
                            "Saturday",
                            "time schdule of Saturday",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_SUN] =
        g_param_spec_string("sun",
                            "Sunday",
                            "time schdule of Sunday",
                            NULL, // default value
                            G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "enable");
    gom_resource_class_set_notnull(resource_class, "mon");
    gom_resource_class_set_notnull(resource_class, "tue");
    gom_resource_class_set_notnull(resource_class, "wed");
    gom_resource_class_set_notnull(resource_class, "thu");
    gom_resource_class_set_notnull(resource_class, "fri");
    gom_resource_class_set_notnull(resource_class, "sat");
    gom_resource_class_set_notnull(resource_class, "sun");
}
