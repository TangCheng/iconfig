# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
enabled|boolean|OSD设置项名称
rect.left|int|区域的左边距，千分比
rect.top|int|区域的上边距，千分比
rect.width|int|区域的宽度，千分比
rect.height|int|区域的高度，千分比
color.red|int|区域颜色的红色分量，0-255
color.green|int|区域颜色的绿色分量，0-255
color.blue|int|区域颜色的蓝色分量，0-255
color.alpha|int|区域颜色的透明度分量，0-255

---
### 1 get_privacy_block
#### request

	{
		'head': {
			'action': 'get_privacy_block',
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
			'action': 'get_privacy_block',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				region1: {
					"enabled": false,
					"rect": {
						"left":10,
						"top":20,
						"width":10,
						"height":20
					}
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				},
				region2: {
					"enabled": false,
					"rect": {
						"left":10,
						"top":20,
						"width":10,
						"height":20
					}
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				}
			}
		}
	}

---
### 2 set_privacy_block
#### request

	{
		'head': {
			'action': 'set_privacy_block',
			'version': '1.0'
		},
		'body': {
			'items': {
				region1: {
					"enabled": false,
					"left":10,
					"top":20,
					"width":10,
					"height":20,
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				},
				region2: {
					"enabled": false,
					"left":10,
					"top":20,
					"width":10,
					"height":20,
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_privacy_block',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				region1: {
					"enabled": false,
					"left":10,
					"top":20,
					"width":10,
					"height":20,
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				},
				region2: {
					"enabled": false,
					"left":10,
					"top":20,
					"width":10,
					"height":20,
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_privacy_block',
			'version': '1.0'
		},
		'body': {
			'items': {
				region1: {
					"enabled": false,
					"left":10,
					"top":20,
					"width":10,
					"height":20,
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				},
				region2: {
					"enabled": false,
					"left":10,
					"top":20,
					"width":10,
					"height":20,
					"color": {
						"red": 0,
						"green": 0,
						"blue": 0,
						"alpha": 0
					}
				}
			}
		}
	}