# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
normal|string|常态[open, close]
period|int|触发态保持时间,单位为秒

---
### 1 get_event_output
#### request

	{
		'head': {
			'action': 'get_event_output',
			'version': '1.0'
		},
		'body': {
			'items': [
				'output1',
				'output2'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_event_output',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"output1" : {
					"normal" : "open",
					"period" : 0
				}
			}
		}
	}

---
### 2 set_event_output
#### request

	{
		'head': {
			'action': 'set_event_output',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"output1" : {
					"normal" : "open",
					"period" : 0
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_event_output',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"output1" : {
					"normal" : "open",
					"period" : 0
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_event_output',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"output1" : {
					"normal" : "open",
					"period" : 0
				}
			}
		}
	}