#ifndef __PRIVACY_BLOCK_H__
#define __PRIVACY_BLOCK_H__

#include <gom/gom.h>

#define IPCAM_PRIVACY_BLOCK_TYPE (ipcam_privacy_block_get_type())
#define IPCAM_PRIVACY_BLOCK(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_PRIVACY_BLOCK_TYPE, IpcamPrivacyBlock))
#define IPCAM_PRIVACY_BLOCK_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_PRIVACY_BLOCK_TYPE, IpcamPrivacyBlockClass))
#define IPCAM_IS_PRIVACY_BLOCK(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_PRIVACY_BLOCK_TYPE))
#define IPCAM_IS_PRIVACY_BLOCK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_PRIVACY_BLOCK_TYPE))
#define IPCAM_PRIVACY_BLOCK_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_PRIVACY_BLOCK_TYPE, IpcamPrivacyBlockClass))

typedef struct _IpcamPrivacyBlock IpcamPrivacyBlock;
typedef struct _IpcamPrivacyBlockClass IpcamPrivacyBlockClass;

struct _IpcamPrivacyBlock
{
    GomResource parent;
};

struct _IpcamPrivacyBlockClass
{
    GomResourceClass parent_class;
};

GType ipcam_privacy_block_get_type(void);

#endif /* __PRIVACY_BLOCK_H__ */
