#ifndef __NETWORK_STATIC_H__
#define __NETWORK_STATIC_H__

#include <gom/gom.h>

#define IPCAM_NETWORK_STATIC_TYPE (ipcam_network_static_get_type())
#define IPCAM_NETWORK_STATIC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_NETWORK_STATIC_TYPE, IpcamNetworkStatic))
#define IPCAM_NETWORK_STATIC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_NETWORK_STATIC_TYPE, IpcamNetworkStaticClass))
#define IPCAM_IS_NETWORK_STATIC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_NETWORK_STATIC_TYPE))
#define IPCAM_IS_NETWORK_STATIC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_NETWORK_STATIC_TYPE))
#define IPCAM_NETWORK_STATIC_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_NETWORK_STATIC_TYPE, IpcamNetworkStaticClass))

typedef struct _IpcamNetworkStatic IpcamNetworkStatic;
typedef struct _IpcamNetworkStaticClass IpcamNetworkStaticClass;

struct _IpcamNetworkStatic
{
    GomResource parent;
};

struct _IpcamNetworkStaticClass
{
    GomResourceClass parent_class;
};

GType ipcam_network_static_get_type(void);

#endif /* __NETWORK_STATIC_H__ */
