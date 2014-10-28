# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
enable|boolean|是否启用
sensitivity|int|灵敏度 1% - 100%
rect.left|int|区域左边距，千分比
rect.top|int|区域上边距，千分比
rect.width|int|区域宽度，千分比
rect.height|int|区域高度，千分比
schedules.mon|string|周一布防时间
schedules.tue|string|周二布防时间
schedules.wed|string|周三布防时间
schedules.thu|string|周四布防时间
schedules.fri|string|周五布防时间
schedules.sat|string|周六布防时间
schedules.sun|string|周日布防时间

---
### 1 get_event_motion
#### request

	{
		'head': {
			'action': 'get_event_motion',
			'version': '1.0'
		},
		'body': {
			'items': [
				'region1',
				'region2'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_event_motion',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"region1" : {
					"enable" : false,
					"sensitivity": 50,
					"rect" : {
						"left": 0,
						"top": 0,
						"width": 0,
						"height": 0
					}
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
### 2 set_event_motion
#### request

	{
		'head': {
			'action': 'set_event_motion',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"region1" : {
					"enable" : false,
					"sensitivity": 50,
					"rect" : {
						"left": 0,
						"top": 0,
						"width": 0,
						"height": 0
					}
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
			'action': 'set_event_motion',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"region1" : {
					"enable" : false,
					"sensitivity": 50,
					"rect" : {
						"left": 0,
						"top": 0,
						"width": 0,
						"height": 0
					}
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
			'event': 'set_event_motion',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"region1" : {
					"enable" : false,
					"sensitivity": 50,
					"rect" : {
						"left": 0,
						"top": 0,
						"width": 0,
						"height": 0
					}
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