# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
record|boolean|是否触发录像
sound|boolean|是否触发报警声
output1|boolean|是否触发输出

---
### 1 get_event_proc
#### request

	{
		'head': {
			'action': 'get_event_proc',
			'version': '1.0'
		},
		'body': {
			'items': [
				'input11'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_event_proc',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"input1" : {
      				"record" : true,
					"sound" : true,
					"output1" : true
				}
			}
		}
	}

---
### 2 set_event_proc
#### request

	{
		'head': {
			'action': 'set_event_proc',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"input1" : {
      				"record" : true,
					"sound" : true,
					"output1" : true
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_event_proc',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"input1" : {
      				"record" : true,
					"sound" : true,
					"output1" : true
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_event_proc',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"input1" : {
      				"record" : true,
					"sound" : true,
					"output1" : true
				}
			}
		}
	}
