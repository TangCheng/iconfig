# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
enable|boolean|是否启用
schedules.mon|string|周一布防时间
schedules.tue|string|周二布防时间
schedules.wed|string|周三布防时间
schedules.thu|string|周四布防时间
schedules.fri|string|周五布防时间
schedules.sat|string|周六布防时间
schedules.sun|string|周日布防时间

---
### 1 get_event_input
#### request

	{
		'head': {
			'action': 'get_event_input',
			'version': '1.0'
		},
		'body': {
            		'items': [
				'input1',
				'input2'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_event_input',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"input1" : {
					"enable" : false,
					"schedules" : {
						"mon" : "",
						"tue" : "1,2,3",
						"wed" : "",
						"thu" : "",
						"fri" : "22,23",
						"sat" : "",
						"sun" : ""
					}
				}
			}
		}
	}

---
### 2 set_event_input
#### request

	{
		'head': {
			'action': 'set_event_input',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"input1" : {
					"enable" : false,
					"schedules" : {
						"mon" : "",
						"tue" : "1,2,3",
						"wed" : "",
						"thu" : "",
						"fri" : "22,23",
						"sat" : "",
						"sun" : ""
					}
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_event_input',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"input1" : {
					"enable" : false,
					"schedules" : {
						"mon" : "",
						"tue" : "1,2,3",
						"wed" : "",
						"thu" : "",
						"fri" : "22,23",
						"sat" : "",
						"sun" : ""
					}
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_event_input',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"input1" : {
					"enable" : false,
					"schedules" : {
						"mon" : "",
						"tue" : "1,2,3",
						"wed" : "",
						"thu" : "",
						"fri" : "22,23",
						"sat" : "",
						"sun" : ""
					}
				}
			}
		}
	}