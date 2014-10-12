#include "event_proc.h"

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_RECORD,
  PROP_SOUND,
  PROP_OUTPUT1,
  N_PROPERTIES
};

typedef struct _IpcamEventProcPrivate
{
    guint id;
    gchar *name;
    gboolean record;
    gboolean sound;
    gboolean output1;
} IpcamEventProcPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamEventProc, ipcam_event_proc, GOM_TYPE_RESOURCE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_event_proc_finalize(GObject *object)
{
    IpcamEventProcPrivate *priv = ipcam_event_proc_get_instance_private(IPCAM_EVENT_PROC(object));
    g_free(priv->name);
    G_OBJECT_CLASS(ipcam_event_proc_parent_class)->finalize(object);
}
static void ipcam_event_proc_set_property(GObject      *object,
                                   guint        property_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
    IpcamEventProc *self = IPCAM_EVENT_PROC(object);
    IpcamEventProcPrivate *priv = ipcam_event_proc_get_instance_private(self);
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
    case PROP_RECORD:
        {
            priv->record = g_value_get_boolean(value);
        }
        break;
    case PROP_SOUND:
        {
            priv->sound = g_value_get_boolean(value);
        }
        break;
    case PROP_OUTPUT1:
        {
            priv->output1 = g_value_get_boolean(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_event_proc_get_property(GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
    IpcamEventProc *self = IPCAM_EVENT_PROC(object);
    IpcamEventProcPrivate *priv = ipcam_event_proc_get_instance_private(self);
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
    case PROP_RECORD:
        {
            g_value_set_boolean(value, priv->record);
        }
        break;
    case PROP_SOUND:
        {
            g_value_set_boolean(value, priv->sound);
        }
        break;
    case PROP_OUTPUT1:
        {
            g_value_set_boolean(value, priv->output1);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_event_proc_init(IpcamEventProc *self)
{
}
static void ipcam_event_proc_class_init(IpcamEventProcClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = &ipcam_event_proc_set_property;
    object_class->get_property = &ipcam_event_proc_get_property;
    object_class->finalize = &ipcam_event_proc_finalize;

    GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);
    gom_resource_class_set_table(resource_class, "event_proc");
  
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
                            "Username.",
                            NULL, // default value
                            G_PARAM_READWRITE);
    obj_properties[PROP_RECORD] =
        g_param_spec_boolean("record",
                             "Record?",
                             "event trigger recording",
                             FALSE,
                             G_PARAM_READWRITE);
    obj_properties[PROP_SOUND] =
        g_param_spec_boolean("sound",
                             "Sound?",
                             "event trigger play sound",
                             FALSE,
                             G_PARAM_READWRITE);
    obj_properties[PROP_OUTPUT1] =
        g_param_spec_boolean("output1",
                             "Output1?",
                             "event trigger output1",
                             FALSE,
                             G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
    gom_resource_class_set_primary_key(resource_class, "id");
    gom_resource_class_set_unique(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "name");
    gom_resource_class_set_notnull(resource_class, "record");
    gom_resource_class_set_notnull(resource_class, "sound");
    gom_resource_class_set_notnull(resource_class, "output1");
}
