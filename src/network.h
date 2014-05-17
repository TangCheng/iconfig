#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <gom/gom.h>

#define IPCAM_NETWORK_TYPE (ipcam_network_get_type())
#define IPCAM_NETWORK(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_NETWORK_TYPE, IpcamNetwork))
#define IPCAM_NETWORK_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_NETWORK_TYPE, IpcamNetworkClass))
#define IPCAM_IS_NETWORK(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_NETWORK_TYPE))
#define IPCAM_IS_NETWORK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_NETWORK_TYPE))
#define IPCAM_NETWORK_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_NETWORK_TYPE, IpcamNetworkClass))

typedef struct _IpcamNetwork IpcamNetwork;
typedef struct _IpcamNetworkClass IpcamNetworkClass;

struct _IpcamNetwork
{
    GomResource parent;
};

struct _IpcamNetworkClass
{
    GomResourceClass parent_class;
};

GType ipcam_network_get_type(void);

#endif /* __NETWORK_H__ */
