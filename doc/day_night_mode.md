# API Reference (1.0)
---
	
### 字段说明
字段|字段类型|字段说明
---|---|---
force_night_mode|boolean|强制夜间模式是否开启
night_mode_threshold|int|日夜转换阈值，百分比
ir_intensity|int|夜间模式，IR发光强度，百分比

---
### 1 get_day_night_mode
#### request

	{
		'head': {
			'action': 'get_day_night_mode',
			'version': '1.0'
		},
		'body': {
			'items': [
				'force_night_mode',
				'night_mode_threshold',
				'ir_intensity'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_day_night_mode',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				“force_night_mode”: false,
				"night_mode_threshold": 50,
				"ir_intensity": 80
			}
		}
	}

---
### 2 set_day_night_mode
#### request

	{
		'head': {
			'action': 'set_day_night_mode',
			'version': '1.0'
		},
		'body': {
			'items': {
				“force_night_mode”: false,
				"night_mode_threshold": 50,
				"ir_intensity": 80
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_day_night_mode',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				“force_night_mode”: false,
				"night_mode_threshold": 50,
				"ir_intensity": 80
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_day_night_mode',
			'version': '1.0'
		},
		'body': {
			'items': {
				“force_night_mode”: false,
				"night_mode_threshold": 50,
				"ir_intensity": 80
		}
	}