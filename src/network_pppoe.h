#ifndef __NETWORK_PPPOE_H__
#define __NETWORK_PPPOE_H__

#include <gom/gom.h>

#define IPCAM_NETWORK_PPPOE_TYPE (ipcam_network_pppoe_get_type())
#define IPCAM_NETWORK_PPPOE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_NETWORK_PPPOE_TYPE, IpcamNetworkPPPoE))
#define IPCAM_NETWORK_PPPOE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_NETWORK_PPPOE_TYPE, IpcamNetworkPPPoEClass))
#define IPCAM_IS_NETWORK_PPPOE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_NETWORK_PPPOE_TYPE))
#define IPCAM_IS_NETWORK_PPPOE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_NETWORK_PPPOE_TYPE))
#define IPCAM_NETWORK_PPPOE_GET_CLASS(obj) (G_TYPE_GET_INSTANCE_CLASS((obj), IPCAM_NETWORK_PPPOE_TYPE, IpcamNetworkPPPoEClass))

typedef struct _IpcamNetworkPPPoE IpcamNetworkPPPoE;
typedef struct _IpcamNetworkPPPoEClass IpcamNetworkPPPoEClass;

struct _IpcamNetworkPPPoE
{
    GomResource parent;
};

struct _IpcamNetworkPPPoEClass
{
    GomResourceClass parent_class;
};

GType ipcam_network_pppoe_get_type(void);

#endif /* __NETWORK_PPPOE_H__ */
