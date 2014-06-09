# API Reference (1.0)
---

## 1. osd（ajax）

### url
http://x.x.x.x/api/1.0/osd.json

### 支持格式
JSON

### HTTP请求方式
GET  
PUT

### 是否需要登录
是

### 请求参数
 请求|参数|必选|类型及范围|说明
---|---|:-:|---|---|---
GET|osd_name|是|array: ['datetime', 'device_name', 'comment', 'frame_rate', 'bit_rate']|需要查询的基本OSD的名称
PUT|osd|是|object: {'osd_name':'datetime','isshow':true,'size':5,'x':10,'y':20,'color':0}|要设置的OSD的名称，是否显示，大小，X/Y坐标，和颜色值

### 注意事项
无

### 调用样例
GET http://x.x.x.x/api/1.0/osd.json?osd_names%5B%5D=datetime&osd_names%5B%5D=device_name  
PUT http://x.x.x.x/api/1.0/osd.json json_data

### 返回结果
JSON示例  
GET

	{
		osds: [
			{"name":"datetime","isshow":true,"size":5,"x":10,"y":20,"color":0},
			{"name":"device_name","isshow":true,"size":5,"x":10,"y":10,"color":0}
		]
	}
	
PUT

	{'success': 'true' | 'false'}
	
### 返回字段说明
返回值字段|字段类型|字段说明
---|---|---
device_name|string|设备名称
comment|string|用户自定义说明

## 2. osd（message）

### 2.1 get_osd
#### request
    {
		head: {
        	action: 'get_osd',
            version: '1.0'
		},
        body: {
            osds: ['datetime', 'device_name', 'comment']
        }
    }
#### response
    {
		head: {
            action: 'get_osd',
            version: '1.0',
		    code: '0'
		},
        body: {
		    osds: [
                {"name":"datetime","isshow":true,"size":5,"x":10,"y":20,"color":0},
				{"name":"device_name","isshow":true,"size":5,"x":10,"y":10,"color":0}
			]
        }
    }
---
### 2.2 set_osd
#### request
    {
		head: {
		    action: 'set_osd',
            version: '1.0'
        },
        body: {
			osds: [
                {"name":"datetime","isshow":true,"size":5,"x":10,"y":20,"color":0},
				{"name":"device_name","isshow":true,"size":5,"x":10,"y":10,"color":0}
			]
		}
    }
#### response
    {
		head: {
    		action: 'set_osd',
            version: '1.0',
			code: '0'
		},
        body: {
		}
    }