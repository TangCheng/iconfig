#ifndef __SCENE_H__
#define __SCENE_H__

#include <gom/gom.h>

#define IPCAM_SCENE_TYPE (ipcam_scene_get_type())
#define IPCAM_SCENE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_SCENE_TYPE, IpcamScene))
#define IPCAM_SCENE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_SCENE_TYPE, IpcamSceneClass))
#define IPCAM_IS_SCENE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_SCENE_TYPE))
#define IPCAM_IS_SCENE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_SCENE_TYPE))
#define IPCAM_SCENE_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_SCENE_TYPE, IpcamSceneClass))

typedef struct _IpcamScene IpcamScene;
typedef struct _IpcamSceneClass IpcamSceneClass;

struct _IpcamScene
{
    GomResource parent;
};

struct _IpcamSceneClass
{
    GomResourceClass parent_class;
};

GType ipcam_scene_get_type(void);

#endif /* __SCENE_H__ */
