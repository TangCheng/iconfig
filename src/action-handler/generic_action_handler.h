#ifndef __GENERIC_ACTION_HANDLER_H__
#define __GENERIC_ACTION_HANDLER_H__

#include "action_handler.h"

#define IPCAM_GENERIC_ACTION_HANDLER_TYPE (ipcam_generic_action_handler_get_type())
#define IPCAM_GENERIC_ACTION_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_GENERIC_ACTION_HANDLER_TYPE, IpcamGenericActionHandler))
#define IPCAM_GENERIC_ACTION_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_GENERIC_ACTION_HANDLER_TYPE, IpcamGenericActionHandlerClass))
#define IPCAM_IS_GENERIC_ACTION_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_GENERIC_ACTION_HANDLER_TYPE))
#define IPCAM_IS_GENERIC_ACTION_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_GENERIC_ACTION_HANDLER_TYPE))
#define IPCAM_GENERIC_ACTION_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_GENERIC_ACTION_HANDLER_TYPE, IpcamGenericActionHandlerClass))

typedef struct _IpcamGenericActionHandler IpcamGenericActionHandler;
typedef struct _IpcamGenericActionHandlerClass IpcamGenericActionHandlerClass;

struct _IpcamGenericActionHandler
{
    IpcamActionHandler parent;
};

struct _IpcamGenericActionHandlerClass
{
    IpcamActionHandlerClass parent_class;
};

GType ipcam_generic_action_handler_get_type(void);

#endif /* __GENERIC_ACTION_HANDLER_H__ */
