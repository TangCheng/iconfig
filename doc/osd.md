# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明
---|---|---
isshow|boolean|是否在屏幕显示
size|int|字体大小
left|int|左边距 千分比
top|int|上边距 千分比
red|int|红色分量 (0-255)
green|int|绿色分量 (0-255)
blue|int|蓝色分量 (0-255)
alpha|int|透明度 (0-255)

---
### 1 get_osd
#### request

	{
		'head': {
			'action': 'get_osd',
			'version': '1.0'
		},
		'body': {
			'items': [
				'master:comment',
				'master:bit_rate',
				'slave:device_name',
				'slave:frame_rate'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_osd',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"master" : {
					"comment" : {
						"isshow" : true,
						"size" : 20,
						"left" : 800,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"bit_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 970,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				},
				"slave" : {
					"device_name" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"frame_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 945,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				}
			}
		}
	}

---
### 2 set_osd
#### request

	{
		'head': {
			'action': 'set_osd',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"master" : {
					"comment" : {
						"isshow" : true,
						"size" : 20,
						"left" : 800,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"bit_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 970,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				},
				"slave" : {
					"device_name" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"frame_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 945,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_osd',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			"items" : {
				"master" : {
					"comment" : {
						"isshow" : true,
						"size" : 20,
						"left" : 800,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"bit_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 970,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				},
				"slave" : {
					"device_name" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"frame_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 945,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_osd',
			'version': '1.0'
		},
		'body': {
			"items" : {
				"master" : {
					"comment" : {
						"isshow" : true,
						"size" : 20,
						"left" : 800,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"bit_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 970,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				},
				"slave" : {
					"device_name" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 10,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					},
					"frame_rate" : {
						"isshow" : true,
						"size" : 20,
						"left" : 10,
						"top" : 945,
						"color" : {
							"red" : 0,
							"green" : 0,
							"blue" : 0,
							"alpha" : 0
						}
					}
				}
			}
		}
	}