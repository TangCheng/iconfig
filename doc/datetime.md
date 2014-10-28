# API Reference (1.0)
---
	
### 字段说明
字段|字段类型|字段说明
---|---|---
timezone|string|时区
use_ntp|boolean|是否开启网络时间同步
ntp_server|string|NTP服务器域名或IP
datetime|string|设备当前日期时间

---
### 1 get_datetime
#### request

	{
		'head': {
			'action': 'get_datetime',
			'version': '1.0'
		},
		'body': {
			'items': [
				'timezone',
				'use_ntp',
				'ntp_server',
				'datetime'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_datetime',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'timezone': 'Beijing',
				'use_ntp': false,
				'ntp_server': 'xxx.xxx.xxx.xxx',
				'datetime': '2014-06-17 13:00:00'
			}
		}
	}

---
### 2 set_datetime
#### request

	{
		'head': {
			'action': 'set_datetime',
			'version': '1.0'
		},
		'body': {
			'items': {
				'timezone': 'Beijing',
				'use_ntp': false,
				'ntp_server': 'xxx.xxx.xxx.xxx',
				'datetime': '2014-06-17 13:00:00'
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_datetime',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'timezone': 'Beijing',
				'use_ntp': false,
				'ntp_server': 'xxx.xxx.xxx.xxx',
				'datetime': '2014-06-17 13:00:00'
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_datetime',
			'version': '1.0'
		},
		'body': {
			'items': {
				'timezone': 'Beijing',
				'use_ntp': false,
				'ntp_server': 'xxx.xxx.xxx.xxx',
				'datetime': '2014-06-17 13:00:00'
			}
		}
	}

