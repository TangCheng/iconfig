# API Reference (1.0)
---

### 字段说明
字段|字段类型|字段说明|取值范围
---|---|---|---
misc:language|string|语言|简体中文 / English
misc:rtsp_auth|boolean|rtsp用户认证
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
base_inf:device_name|string|设备名称
base_inf:comment|string|用户自定义说明
base_inf:location|string|设备位置
base_inf:hwaddr|string|设备MAC地址，只读
base_inf:manufacturer|string|制造商，只读
base_inf:model|string|设备型号，只读
base_inf:serial|string|设备序列号，只读
base_inf:firmware|string|设备软件版本，只读
base_inf:hardware|string|设备硬件版本，只读
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
datetime:timezone|string|时区
datetime:use_ntp|boolean|是否自动同步
datetime:ntp_server|string|ntp服务器地址
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
video:profile|string|视频编码等级|baseline / main / high
video:flip|boolean|视频翻转
video:mirror|boolean|视频镜像
video:master:resolution|string|主码流分辨率|UXGA / 1080P / 960H / 720P / D1
video:master:frame_rate|int|主码流帧率|[1,  30]
video:master:bit_rate_ctrl|string|主码流码率控制|VBR / CBR
video:master:bit_rate|int|主码流码率|[128, 4096]
video:master:stream_path|string|主码流url路径
video:slave:resolution|string|次码流分辨率|D1 / CIF
video:slave:frame_rate|int|次码流帧率|[1,30]
video:slave:bit_rate_ctrl|string|次码流码率控制|VBR / CBR
video:slave:bit_rate|int|次码流码率|[128, 1024]
video:slave:stream_path|string|次码流url路径
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
image:watermark|boolean|数字水印
image:3ddnr|boolean|3D降噪
image:birghtness|int|亮度|[1 - 100]
image:chrominance|int|色度|[1 - 100]
image:contrast|int|对比度|[1 - 100]
image:saturation|int|饱和度|[1 - 100]
image:scenario|string|场景设置|50Hz / 60Hz / 混合模式
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
privacy_block:enabled|boolean|OSD设置项名称
privacy_block:rect.left|int|区域的左边距，千分比|[0, 1000]
privacy_block:rect.top|int|区域的上边距，千分比|[0, 1000]
privacy_block:rect.width|int|区域的宽度，千分比|[0, 1000]
privacy_block:rect.height|int|区域的高度，千分比|[0, 1000]
privacy_block:color|int|区域颜色|[0, 0xFFFFFFFF]
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
day_night_mod:force_night_mode|boolean|强制夜间模式
day_night_mod:night_mode_threshold|int|日夜转换阈值，百分比|[1, 100]
day_night_mod:ir_intensity|int|夜间模式，IR发光强度，百分比|[1, 100]
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
osd:master:device_name:
osd:master:comment:
osd:master:datetime:
osd:master:frame_rate:
osd:master:bit_rate:
osd:slave:device_name:
osd:slave:comment:
osd:slave:datetime:
osd:slave:frame_rate:
osd:slave:bit_rate:
isshow|boolean|是否在屏幕显示
size|int|字体大小|[1, 100]
left|int|左边距，千分比|[0, 1000]
top|int|上边距，千分比|[0, 1000]
color|int|颜色|[0, 0xFFFFFFFF]
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
szyc:train_num|string|车次
szyc:carriage_num|int|车厢|[1, 32]
szyc:index|int|位置|[1, 8]
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
network:method|string|IP地址获取方法|static / dhcp / pppoe
network:address:ipaddr|string|IP地址
network:address:netmask|string|网络掩码
network:address:gateway|string|网关IP地址
network:address:dns1|string|主DNS
network:address:dns2|string|备用DNS
network:pppoe:username|string|PPPoE账户名
network:pppoe:password|string|PPPoE账户密码
network:port:http|int|HTTP服务端口
network:port:ftp|int|FTP服务端口
network:port:rtsp|int|RTSP服务端口
-----------------------------------------|-------------|---------------------------------|----------------------------------------------
event_input:enable|boolean|是否启用
event_input:schedules:mon|string|周一布防时间|1,2,3,4,5,6,7,8,9,10...23
event_input:schedules:tue|string|周二布防时间
event_input:schedules:wed|string|周三布防时间
event_input:schedules:thu|string|周四布防时间
event_input:schedules:fri|string|周五布防时间
event_input:schedules:sat|string|周六布防时间
event_input:schedules:sun|string|周日布防时间

---
### 1 get_params
#### request

    {
		'head': {
			'action': 'get_params',
			'version': '1.0'
		},
		'body': {
			'items': [
				'base_info:device_name',
				'base_info:comment',
				'base_info:location',
				'base_info:hwaddr',
				'base_info:manufacturer',
				'base_info:model',
				'base_info:serial',
				'base_info:firmware',
				'base_info:hardware'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_params',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'base_info:device_name': 'device_name',
				'base_info:comment': 'comment',
				'base_info:location': 'China',
				"base_info:manufacturer": "IPNC",
				"base_info:model": "IPNC-100",
				"base_info:serial": "12345678",
				"base_info:firmware": "1.0.0",
				"base_info:hardware": "Rev. 1"
			}
		}
	}

---
### 2 set_params
#### request

	{
		'head': {
			'action': 'set_params',
			'version': '1.0'
		},
		'body': {
			'items': {
				'base_info:device_name': 'device_name'
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_params',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'base_info:device_name': 'device_name'
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_params',
			'version': '1.0',
		},
		'body': {
			'items': {
				'base_info:device_name': 'device_name'
			}
		}
	}
