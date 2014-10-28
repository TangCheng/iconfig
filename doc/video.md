# API Reference (1.0)
---
	
### 字段说明
字段|字段类型|字段说明
---|---|---
profile|string|视频编码级别['baseline','main','high']
flip|boolean|视频垂直翻转
mirror|boolean|视频左右镜像
main_profile.frame_rate|int|视频帧率(1～30fps)
main_profile.bit_rate|string|视频码率['CBR','VBR']
main_profile.bit_rate_value|int|视频码率值
main_profile.resolution|string|视频分辨率['UXGA', '1080P', '960H', '720P']
main_profile.stream_path|string|视频流路径
sub_profile.frame_rate|int|视频帧率(1～30fps)
sub_profile.bit_rate|string|视频码率['CBR','VBR']
sub_profile.bit_rate_value|int|视频码率值
sub_profile.resolution|string|视频分辨率['D1', 'CIF']
sub_profile.stream_path|string|视频流路径

--
### 1 get_video
#### request

	{
		'head': {
			'action': 'get_video',
			'version': '1.0'
		},
		'body': {
			'items': [
				'profile',
				'flip',
				'mirror',
				'main_profile',
				'sub_profile'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_video',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'profile': 'baseline',
				'flip': false,
				'mirror': false,
				'main_profile': {
					'frame_rate': 25,
					'bit_rate': 'CBR',
					'bit_rate_value': 4096,
					'resolution': 'UXGA',
					'stream_path': 'main_stream'
				},
				'sub_profile': {
					'frame_rate': 25,
					'bit_rate': 'VBR',
					'bit_rate_value': 1024,
					'resolution': 'D1',
					'stream_path': 'sub_stream'
				}
			}
		}
	}

---
### 2 set_video
#### request

	{
		'head': {
			'action': 'set_video',
			'version': '1.0'
		},
		'body': {
			'items': {
				'profile': 'baseline',
				'flip': false,
				'mirror': false,
				'main_profile': {
					'frame_rate': 25,
					'bit_rate': 'CBR',
					'bit_rate_value': 4096,
					'resolution': 'UXGA',
					'stream_path': 'main_stream'
				},
				'sub_profile': {
					'frame_rate': 25,
					'bit_rate': 'VBR',
					'bit_rate_value': 1024,
					'resolution': 'D1',
					'stream_path': 'sub_stream'
				}
			}
		}
	}

#### response

	{
		'head': {
			'action': 'set_video',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': {
				'profile': 'baseline',
				'flip': false,
				'mirror': false,
				'main_profile': {
					'frame_rate': 25,
					'bit_rate': 'CBR',
					'bit_rate_value': 4096,
					'resolution': 'UXGA',
					'stream_path': 'main_stream'
				},
				'sub_profile': {
					'frame_rate': 25,
					'bit_rate': 'VBR',
					'bit_rate_value': 1024,
					'resolution': 'D1',
					'stream_path': 'sub_stream'
				}
			}
		}
	}

---
### 3 notice

	{
		'head': {
			'event': 'set_video',
			'version': '1.0'
		},
		'body': {
			'items': {
				'profile': 'baseline',
				'flip': false,
				'mirror': false,
				'main_profile': {
					'frame_rate': 25,
					'bit_rate': 'CBR',
					'bit_rate_value': 4096,
					'resolution': 'UXGA',
					'stream_path': 'main_stream'
				},
				'sub_profile': {
					'frame_rate': 25,
					'bit_rate': 'VBR',
					'bit_rate_value': 1024,
					'resolution': 'D1',
					'stream_path': 'sub_stream'
				}
			}
		}
	}