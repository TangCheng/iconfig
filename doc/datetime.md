# API Reference (1.0)
---

## 1. datetime（ajax）

### url
http://x.x.x.x/api/1.0/datetime.json

### 支持格式
JSON

### HTTP请求方式
GET  
PUT

### 是否需要登录
是

### 请求参数
 请求|参数|必选|类型及范围|说明
---|---|:-:|---|---|---
GET|items|是|array: ['timezone', 'use_ntp', 'ntp_server', 'datetime']|需要查询的Datetime设置项的名称
PUT|items|是|object: {'timezone': { int_value':0, 'str_value':'GMT+8' }, 'use_ntp':{ 'int_value':0, 'str_value':'' }, 'ntp_server': { 'int_value':0, 'str_value':'xxx.xxx.xxx.xxx' }, 'datetime': {'int_value': 1, 'str_value':'2014-06-17:13:00:00'}}|要设置的Datetime设置项的名值对

### 注意事项
无

### 调用样例
GET http://x.x.x.x/api/1.0/datetime.json?items%5B%5D=timezone&items%5B%5D=use_ntp&items%5B%5D=ntp_server&items%5B%5D=datetime  
PUT http://x.x.x.x/api/1.0/datetime.json json_data

### 返回结果
JSON示例  
GET

	{
		'items': {
			'timezone': {
				'int_value':0,
				'str_value':'GMT+8'
			}
			'use_ntp': {
				'int_value':0,
				'str_value':''
			}
			'ntp_server': {
				'int_value':0
				'str_value':'xxx.xxx.xxx.xxx'
			},
			'datetime': {
				'int_value':1
				'str_value':'2014-06-17 13:00:00'
			}
		}
	}
	
PUT

	{'success': 'true' | 'false'}
	
### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
timezone|object|时区
use_ntp|object|是否开启网络时间
ntp_server|object|NTP服务器

## 2. datetime（message）

### 2.1 get_datetime
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
                'timezone': {
                    'int_value':0,
                    'str_value':'GMT+8'
                },
                'use_ntp': {
                    'int_value':0,
                    'str_value':''
                },
                'ntp_server': {
                    'int_value':0,
                    'str_value':'xxx.xxx.xxx.xxx'
                }
                'datetime': {
                    'int_value':1
                    'str_value':'2014-06-17 13:00:00'
                }
            }
        }
    }
---
### 2.2 set_datetime
#### request
    {
		'head': {
            'action': 'set_datetime',
            'version': '1.0'
        },
        'body': {
            'items': {
                'timezone': {
                    'int_value':0,
                    'str_value':'GMT+8'
                },
                'use_ntp': {
                    'int_value':0,
                    'str_value':''
                },
                'ntp_server': {
                    'int_value':0,
                    'str_value':'xxx.xxx.xxx.xxx'
                }
                'datetime': {
                    'int_value':1
                    'str_value':'2014-06-17 13:00:00'
                }
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
		}
    }
