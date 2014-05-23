#ifndef __NETWORK_PORT_H__
#define __NETWORK_PORT_H__

#include <gom/gom.h>

#define IPCAM_NETWORK_PORT_TYPE (ipcam_network_port_get_type())
#define IPCAM_NETWORK_PORT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_NETWORK_PORT_TYPE, IpcamNetworkPort))
#define IPCAM_NETWORK_PORT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_NETWORK_PORT_TYPE, IpcamNetworkPortClass))
#define IPCAM_IS_NETWORK_PORT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_NETWORK_PORT_TYPE))
#define IPCAM_IS_NETWORK_PORT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_NETWORK_PORT_TYPE))
#define IPCAM_NETWORK_PORT_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_NETWORK_PORT_TYPE, IpcamNetworkPortClass))

typedef struct _IpcamNetworkPort IpcamNetworkPort;
typedef struct _IpcamNetworkPortClass IpcamNetworkPortClass;

struct _IpcamNetworkPort
{
    GomResource parent;
};

struct _IpcamNetworkPortClass
{
    GomResourceClass parent_class;
};

GType ipcam_network_port_get_type(void);

#endif /* __NETWORK_PORT_H__ */
