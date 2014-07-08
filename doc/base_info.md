# API Reference (1.0)
---

## 1. base_info（ajax）

### url
http://x.x.x.x/api/1.0/base_info.json

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
GET|items|是|array: ['device_name', 'comment']|需要查询的基本信息的名称
PUT|items|是|object: {'device_name':'ipcam','location'='China','comment':'my content'}|要设置的基本信息的名值对

### 注意事项
无

### 调用样例
GET http://x.x.x.x/api/1.0/base_info.json?items%5B%5D=device_name&items%5B%5D=comment  
PUT http://x.x.x.x/api/1.0/base_info.json json_data

### 返回结果
JSON示例  
GET

	{
		'items': {
			“device_name”: "ipcam",
			"comment": "my content"
		}
	}
	
PUT

	{'success': 'true' | 'false'}
	
### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
device_name|string|设备名称
comment|string|用户自定义说明

## 2. base_info（message）

### 2.1 get_base_info
#### request
    {
		'head': {
            'action': 'get_base_info',
            'version': '1.0'
		},
        'body': {
            'items': ['device_name', 'comment']
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
                'comment': 'comment'
			}
        }
    }
---
### 2.2 set_base_info
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
		}
    }
