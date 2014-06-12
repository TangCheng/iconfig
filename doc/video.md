# API Reference (1.0)
---

## 1. video（ajax）

### url
http://x.x.x.x/api/1.0/video.json

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
GET|items|是|array: ['profile', 'flip', 'quanlity', 'frame_rate', 'bit_rate', 'bit_rate_value']|需要查询的Video设置项的名称
PUT|items|是|object: {'profile':0, 'flip':0, 'quanlity':0, 'frame_rate':25, 'bit_rate':0, 'bit_rate_value':300}|要设置的Video设置项的名称，值

### 注意事项
无

### 调用样例
GET http://x.x.x.x/api/1.0/video.json?params%5B%5D=profile&params%5B%5D=flip  
PUT http://x.x.x.x/api/1.0/video.json json_data

### 返回结果
JSON示例  
GET

	{
		'items': {
			'profile':0,
			'flip':0,
			'quanlity':0,
			'frame_rate':25,
			'bit_rate':0,
			'bit_rate_value':300
		}
	}
	
PUT

	{'success': 'true' | 'false'}
	
### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
profile|int|视频编码级别
flip|int|视频垂直和左右翻转
quanlity|int|视频质量（高、中、低）
frame_rate|int|视频帧率（1～30fps）
bit_rate|int|视频码率（CBR/VBR）
bit_rate_value|int|视频码率值

## 2. video（message）

### 2.1 get_video
#### request
    {
		'head': {
        	'action': 'get_video',
            'version': '1.0'
		},
        'body': {
            'items': [
				'profile',
				'flip',
				'quanlity',
				'frame_rate',
				'bit_rate',
				'bit_rate_value'
			]
        }
    }
#### response
    {
		'head': {
            'action': 'get_video',
            'version': '1.0',
		    'code': '0'
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
---
### 2.2 set_video
#### request
    {
		'head': {
		    'action': 'set_video',
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
    		'action': 'set_video',
            'version': '1.0',
			'code': '0'
		},
        'body': {
		}
    }
