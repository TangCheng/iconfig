# API Reference (1.0)
---

### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
wartermark|boolean|数字水印开关
3ddnr|boolean|3D降噪开关
brightness|int|亮度 (1 - 100)
chrominance|int|色度 (1 - 100)
contrast|int|对比度 (1 - 100)
saturation|int|饱和度 (1 - 100)
scenario|string|场景设置（50Hz/60Hz/混合模式）

---
### 1 get_image
#### request

	{
		'head': {
			'action': 'get_image',
			'version': '1.0'
		},
		'body': {
			'items': [
				'brightness',
				'scenario'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_image',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'brightness': 50,
				'scenario': '50Hz'
			}
		}
	}

---
### 2 set_image
#### request

	{
		'head': {
			'action': 'set_image',
			'version': '1.0'
		},
		'body': {
			'items': {
				'brightness': 100
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_image',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'brightness': 100
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_image',
			'version': '1.0'
		},
		'body': {
			'items': {
				'brightness': 100
			}
		}
	}