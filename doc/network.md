# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
method|string|IP地址获取方法（static/dhcp/pppoe）
address.ipaddr|string|IP地址
address.netmask|string|网络掩码
address.gateway|string|网关IP地址
address.dns1|string|主DNS
address.dns2|string|备用DNS
pppoe.username|string|PPPoE账户名
pppoe.password|string|PPPoE账户密码
port.http|int|HTTP服务端口
port.ftp|int|FTP服务端口
port.rtsp|int|RTSP服务端口

---
### 2.1 get_network
#### request

	{
		'head': {
			'action': 'get_network',
			'version': '1.0'
		},
		'body': {
			'items': [
				'method',
				'address',
				'pppoe',
				'port'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_network',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'method': 'static',
				'address': {
					'ipaddr': '192.168.0.15',
					'netmask': '255.255.255.0',
					'gateway': '192.168.0.1',
					'dns1': '192.168.0.1',
					'dns2': ''
				},
				'pppoe': {
					'username': 'admin',
					'password': '123456'
				},
				'port': {
					'http': 80,
					'ftp': 21
					'rtsp': 554
				}
			}
		}
	}

---
### 2 set_network
#### request

	{
		'head': {
			'action': 'set_network',
			'version': '1.0'
	},
		'body': {
			'items': {
				'method': 'static',
				'address': {
					'ipaddr': '192.168.0.15',
					'netmask': '255.255.255.0',
					'gateway': '192.168.0.1',
					'dns1': '192.168.0.1',
					'dns2': ''
				},
				'pppoe': {
					'username': 'admin',
					'password': '123456'
				},
				'port': {
					'http': 80,
					'ftp': 21,
					'rtsp': 554
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_network',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'method': 'static',
				'address': {
					'ipaddr': '192.168.0.15',
					'netmask': '255.255.255.0',
					'gateway': '192.168.0.1',
					'dns1': '192.168.0.1',
					'dns2': ''
				},
				'pppoe': {
					'username': 'admin',
					'password': '123456'
				},
				'port': {
					'http': 80,
					'ftp': 21,
					'rtsp': 554
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_network',
			'version': '1.0'
		},
		'body': {
			'items': {
				'method': 'static',
				'address': {
					'ipaddr': '192.168.0.15',
					'netmask': '255.255.255.0',
					'gateway': '192.168.0.1',
					'dns1': '192.168.0.1',
					'dns2': ''
				},
				'pppoe': {
					'username': 'admin',
					'password': '123456'
				},
				'port': {
					'http': 80,
					'ftp': 21,
					'rtsp': 554
				}
			}
		}
	}