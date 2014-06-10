#ifndef __HTTP_VIDEO_PARAM_HANDLER_H__
#define __HTTP_VIDEO_PARAM_HANDLER_H__

#include "http_request_handler.h"

#define IPCAM_HTTP_VIDEO_PARAM_HANDLER_TYPE (ipcam_http_video_param_handler_get_type())
#define IPCAM_HTTP_VIDEO_PARAM_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_HTTP_VIDEO_PARAM_HANDLER_TYPE, IpcamHttposdHandler))
#define IPCAM_HTTP_VIDEO_PARAM_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_HTTP_VIDEO_PARAM_HANDLER_TYPE, IpcamHttposdHandlerClass))
#define IPCAM_IS_HTTP_VIDEO_PARAM_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_HTTP_VIDEO_PARAM_HANDLER_TYPE))
#define IPCAM_IS_HTTP_VIDEO_PARAM_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_HTTP_VIDEO_PARAM_HANDLER_TYPE))
#define IPCAM_HTTP_VIDEO_PARAM_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_HTTP_VIDEO_PARAM_HANDLER_TYPE, IpcamHttposdHandlerClass))

typedef struct _IpcamHttpVideoParamHandler IpcamHttpVideoParamHandler;
typedef struct _IpcamHttpVideoParamHandlerClass IpcamHttpVideoParamHandlerClass;

struct _IpcamHttpVideoParamHandler
{
    IpcamHttpRequestHandler parent;
};

struct _IpcamHttpVideoParamHandlerClass
{
    IpcamHttpRequestHandlerClass parent_class;
};

GType ipcam_http_video_param_handler_get_type(void);

#endif /* __HTTP_VIDEO_PARAM_HANDLER_H__ */
