# API Reference (1.0)
---
	
### 字段说明
字段|字段类型|字段说明
---|---|---
username|string|用户名
password|string|密码
role|string|用户角色

---
### 1 get_users
#### request

	{
		'head': {
			'action': 'get_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				'password',
				'role'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'get_users',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': [
				{
					'username': 'admin',
					'password': 'passwd',
					'role': 'administrator'
				},
				{
					'username': 'guest',
					'password': 'passwd',
					'role': 'user'
				}
			]
		}
	}

---
### 2 set_users
#### request

	{
		'head': {
			'action': 'set_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				{
					'username': 'admin',
					'password': 'passwd',
					'role': 'administrator'
				}
			]
		}
	}

#### response

	{
		'head': {
			'action': 'set_users',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': [
				{
					'username': 'admin',
					'password': 'passwd',
					'role': 'administrator'
				}
			]
		}
	}

---
### 3 add_users
#### request

	{
		'head': {
			'action': 'add_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				{
					'username': 'user1',
					'password': 'passwd',
					'role': 'user'
				},
				{
					'username': 'user2',
					'password': 'passwd',
					'role':  'user'
				}
			]
		}
	}

#### response

	{
		'head': {
			'action': 'add_users',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': [
				{
					'username': 'user1',
					'password': 'passwd',
					'role': 'user'
				},
				{
					'username': 'user2',
					'password': 'passwd',
					'role':  'user'
				}
			]
		}
	}

---
### 4 del_users
#### request

	{
		'head': {
			'action': 'del_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				'user1',
				'user2'
			]
		}
	}

#### response

	{
		'head': {
			'action': 'del_users',
			'version': '1.0',
			'code': '0'
		},
		'body': {
			'items': [
				'user1',
				'user2'
			]
		}
	}

---
### 5 notice

	{
		'head': {
			'event': 'set_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				{
					'username': 'admin',
					'password': 'passwd',
					'role': 'administrator'
				}
			]
		}
	}

	{
		'head': {
			'event': 'add_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				{
					'username': 'user1',
					'password': 'passwd',
					'role': 'user'
				},
				{
					'username': 'user2',
					'password': 'passwd',
					'role':  'user'
				}
			]
		}
	}

	{
		'head': {
			'event': 'del_users',
			'version': '1.0'
		},
		'body': {
			'items': [
				'user1',
				'user2'
			]
		}
	}