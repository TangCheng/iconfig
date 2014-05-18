# iConfig
---
## action list
---
### 1. get_base_info
#### request
    {
        action: 'get_base_info',
        id: $id,
        version: '1.0',
        body: {
            info_name: ['device_name', 'comment'],
        }
    }
#### response
    {
        action: 'get_base_info',
        id: $id,
        version: '1.0',
		code: '0',
        body: {
            info_name: ['device_name', 'comment'],
			info_value: ['$(device_name)', '$(comment)'],
        }
    }
---
### 2. set_base_info
#### request
    {
		action: 'set_base_info',
        id: $id,
        version: '1.0',
        body: {
			info_name: ['device_name', 'comment'],
			info_value: ['$(device_name)', '$(comment)'],
		}
    }
#### response
    {
		action: 'set_base_info',
        id: $id,
        version: '1.0',
		code: 0,
        body: {
		}
    }
---
### 3. get_user_info
#### request
    {
		action: 'get_user_info',
        id: $id,
        version: '1.0',
        body: {
			  
		}
    }
#### response
    {
		action: 'get_user_info',
        id: $id,
        version: '1.0',
        body: {
			  
		}
    }
---
### 4. set_user_info
#### request
    {
		action: 'set_user_info',
        id: $id,
        version: '1.0',
        body: {
			  
		}
    }
#### response
    {
		action: 'set_user_info',
        id: $id,
        version: '1.0',
        body: {
			  
		}
    }
---
