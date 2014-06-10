#ifndef __HTTP_SCENE_HANDLER_H__
#define __HTTP_SCENE_HANDLER_H__

#include "http_request_handler.h"

#define IPCAM_HTTP_SCENE_HANDLER_TYPE (ipcam_http_scene_handler_get_type())
#define IPCAM_HTTP_SCENE_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_HTTP_SCENE_HANDLER_TYPE, IpcamHttposdHandler))
#define IPCAM_HTTP_SCENE_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_HTTP_SCENE_HANDLER_TYPE, IpcamHttposdHandlerClass))
#define IPCAM_IS_HTTP_SCENE_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_HTTP_SCENE_HANDLER_TYPE))
#define IPCAM_IS_HTTP_SCENE_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_HTTP_SCENE_HANDLER_TYPE))
#define IPCAM_HTTP_SCENE_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_HTTP_SCENE_HANDLER_TYPE, IpcamHttposdHandlerClass))

typedef struct _IpcamHttpSceneHandler IpcamHttpSceneHandler;
typedef struct _IpcamHttpSceneHandlerClass IpcamHttpSceneHandlerClass;

struct _IpcamHttpSceneHandler
{
    IpcamHttpRequestHandler parent;
};

struct _IpcamHttpSceneHandlerClass
{
    IpcamHttpRequestHandlerClass parent_class;
};

GType ipcam_http_scene_handler_get_type(void);

#endif /* __HTTP_SCENE_HANDLER_H__ */
