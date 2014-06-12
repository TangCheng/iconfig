#ifndef __BASE_INFO_ACTION_HANDLER_H__
#define __BASE_INFO_ACTION_HANDLER_H__

#include "action_handler.h"

#define IPCAM_BASE_INFO_ACTION_HANDLER_TYPE (ipcam_base_info_action_handler_get_type())
#define IPCAM_BASE_INFO_ACTION_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_BASE_INFO_ACTION_HANDLER_TYPE, IpcamBaseInfoActionHandler))
#define IPCAM_BASE_INFO_ACTION_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_BASE_INFO_ACTION_HANDLER_TYPE, IpcamBaseInfoActionHandlerClass))
#define IPCAM_IS_BASE_INFO_ACTION_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_BASE_INFO_ACTION_HANDLER_TYPE))
#define IPCAM_IS_BASE_INFO_ACTION_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_BASE_INFO_ACTION_HANDLER_TYPE))
#define IPCAM_BASE_INFO_ACTION_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_BASE_INFO_ACTION_HANDLER_TYPE, IpcamBaseInfoActionHandlerClass))

typedef struct _IpcamBaseInfoActionHandler IpcamBaseInfoActionHandler;
typedef struct _IpcamBaseInfoActionHandlerClass IpcamBaseInfoActionHandlerClass;

struct _IpcamBaseInfoActionHandler
{
    IpcamActionHandler parent;
};

struct _IpcamBaseInfoActionHandlerClass
{
    IpcamActionHandlerClass parent_class;
};

GType ipcam_base_info_action_handler_get_type(void);

#endif /* __BASE_INFO_ACTION_HANDLER_H__ */
