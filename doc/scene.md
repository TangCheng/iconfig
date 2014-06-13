# API Reference (1.0)
---

## 1. scene（ajax）

### url
http://x.x.x.x/api/1.0/scene.json

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
GET|items|是|array: ['scenario']|需要查询的场景设置项的名称
PUT|items|是|object: {'scenario':0}|要设置的场景设置项的名称，值

### 注意事项
无

### 调用样例
GET http://x.x.x.x/api/1.0/scene.json?items%5B%5D=scenario  
PUT http://x.x.x.x/api/1.0/scene.json json_data

### 返回结果
JSON示例  
GET

	{
		'items': {
			'scenario':0,
		}
	}
	
PUT

	{'success': 'true' | 'false'}
	
### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
scenario|int|场景设置（50Hz/60Hz/混合模式）

## 2. scene（message）

### 2.1 get_scene
#### request
    {
		'head': {
            'action': 'get_scene',
            'version': '1.0'
		},
        'body': {
            'items': [
				'scenario'
			]
        }
    }
#### response
    {
		'head': {
            'action': 'get_scene',
            'version': '1.0',
            'code': '0'
		},
        'body': {
            'items': {
                'scenario':0
			}
        }
    }
---
### 2.2 set_scene
#### request
    {
		'head': {
            'action': 'set_scene',
            'version': '1.0'
        },
        'body': {
			'items': {
                'profile':0,
				'flip':0,
				'quanlity':0
				'frame_rate':25,
				'bit_rate':0,
				'bit_rate_value':300
			}
		}
    }
#### response
    {
		'head': {
            'action': 'set_scene',
            'version': '1.0',
			'code': '0'
		},
        'body': {
		}
    }
