# API Reference (1.0)
---
	
### 字段说明
字段|字段类型|字段说明
---|---|---
language|string|语言['简体中文','English']
rtsp_auth|boolean|rtsp用户认证

--
### 1 get_misc
#### request

	{
		'head': {
			'action': 'get_misc',
			'version': '1.0'
		},
		'body': {
			'items': [
				'language',
				'rtsp_auth'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_misc',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'language': '简体中文',
				'rtsp_auth': false
			}
		}
	}

---
### 2 set_misc
#### request

	{
		'head': {
			'action': 'set_misc',
			'version': '1.0'
		},
		'body': {
			'items': {
				'language': '简体中文',
				'rtsp_auth': false
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_misc',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'language': '简体中文',
				'rtsp_auth': false
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_misc',
			'version': '1.0'
		},
		'body': {
			'items': {
				'language': '简体中文',
				'rtsp_auth': false
			}
		}
	}