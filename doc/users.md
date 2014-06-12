# API Reference (1.0)
---

## 1. users（ajax）

### url
http://x.x.x.x/api/1.0/users.json

### 支持格式
JSON

### HTTP请求方式
GET  
PUT
POST
DELETE

### 是否需要登录
是

### 请求参数
 请求|参数|必选|类型及范围|说明
---|---|:-:|---|---|---
GET|items|是|array: ['profile', 'flip', 'quanlity', 'frame_rate', 'bit_rate', 'bit_rate_value']|需要查询的Users设置项的名称
PUT|items|是|object: {'profile':0, 'flip':0, 'quanlity':0, 'frame_rate':25, 'bit_rate':0, 'bit_rate_value':300}|要设置的Users设置项的名称，值

### 注意事项
无

### 调用样例
GET http://x.x.x.x/api/1.0/users.json&items%5B%5D=password&items%5B%5D=privilege
PUT http://x.x.x.x/api/1.0/users.json json_data

### 返回结果
JSON示例
GET

	{
		'items': [
			{
				'username':'admin',
				'password':'passwd',
				'privilege':1
			},
			{
				'username':'guest',
				'password':'passwd',
				'privilege':0
			}
		]
	}
	
PUT

	{'success': 'true' | 'false'}
	
### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
username|string|用户名
password|string|密码
privilege|int|是否特权用户

## 2. users（message）

### 2.1 get_users
#### request
    {
		'head': {
        	'action': 'get_users',
            'version': '1.0'
		},
        'body': {
            'items': [
                'password',
                'privilege'
            ]
        }
    }
#### response
    {
		'head': {
            'action': 'get_users',
            'version': '1.0',
		    'code': '0'
		},
        'body': {
		    'items': [
                {
                    'username':'admin',
                    'password':'passwd',
                    'privilege':1
                }
                {
                    'username':'guest',
                    'password':'passwd',
                    'privilege':0
                }
            ]
        }
    }
---
### 2.2 set_users
#### request
    {
		'head': {
		    'action': 'set_users',
            'version': '1.0'
        },
        'body': {
			'items': [
                {
                    'username':'user1',
                    'password':'passwd',
                    'privilege':0
                },
                {
                    'username':'user2',
                    'password':'passwd',
                    'privilege':0
                }
			]
		}
    }
#### response
    {
		'head': {
    		'action': 'set_users',
            'version': '1.0',
			'code': '0'
		},
        'body': {
		}
    }
### 2.3 add_users
#### request
    {
		'head': {
		    'action': 'set_users',
            'version': '1.0'
        },
        'body': {
			'items': [
                {
                    'username':'user1',
                    'password':'passwd',
                    'privilege':0
                },
                {
                    'username':'user2',
                    'password':'passwd',
                    'privilege':0
                }
			]
		}
    }
#### response
    {
		'head': {
    		'action': 'set_users',
            'version': '1.0',
			'code': '0'
		},
        'body': {
		}
    }
### 2.4 del_users
#### request
    {
		'head': {
		    'action': 'set_users',
            'version': '1.0'
        },
        'body': {
			'items': [
                {
                    'username':'user1',
                },
                {
                    'username':'user2',
                }
			]
		}
    }
#### response
    {
		'head': {
    		'action': 'set_users',
            'version': '1.0',
			'code': '0'
		},
        'body': {
		}
    }
