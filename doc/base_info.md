# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
device_name|string|设备名称
comment|string|用户自定义说明
location|string|设备位置
hwaddr|string|设备MAC地址，只读
manufacturer|string|制造商，只读
model|string|设备型号，只读
serial|string|设备序列号，只读
firmware|string|设备软件版本，只读
hardware|string|设备硬件版本，只读

---
### 1 get_base_info
#### request

    {
		'head': {
			'action': 'get_base_info',
			'version': '1.0'
		},
		'body': {
			'items': [
				'device_name',
				'comment',
				'location',
				'hwaddr',
				'manufacturer',
				'model',
				'serial',
				'firmware',
				'hardware'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_base_info',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'device_name': 'device_name',
				'comment': 'comment',
				'location': 'China',
				"manufacturer": "IPNC",
				"model": "IPNC-100",
				"serial": "12345678",
				"firmware": "V1.0.0",
				"hardware": "Rev. 1"
			}
		}
	}

---
### 2 set_base_info
#### request

	{
		'head': {
			'action': 'set_base_info',
			'version': '1.0'
		},
		'body': {
			'items': {
				'device_name': 'device_name',
				'comment': 'comment'
				'location': 'China'
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_base_info',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'device_name': 'device_name',
				'comment': 'comment'
				'location': 'China'
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_base_info',
			'version': '1.0',
		},
		'body': {
			'items': {
				'device_name': 'device_name',
				'comment': 'comment'
				'location': 'China'
			}
		}
	}
