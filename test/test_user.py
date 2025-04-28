import requests


user_id = 0
token = ""


def cmp(a, b):
    for key in a:
        if a[key] != b[key]:
            return False

    return True


def test_auth():
    global token
    data = {"password": "testtest"}
    token = requests.post("http://127.0.0.1:8080/api/user/0/auth", json=data).json()["token"]
    assert(len(token) != 0)


def test_post():
    global user_id
    data = {"tg_id": "testtest", 
            "name": "Иван", 
            "surname": "Курышкин", 
            "tg_username": "@kurygas", 
            "user_type": 0, 
            "work_result_list": [],
            "group_id": -1,
            }
    response = requests.post("http://127.0.0.1:8080/api/user", json=data)
    assert(response.status_code == 201)
    assert(cmp(data, response.json()))
    assert("user_id" in response.json())

    data = {"tg_id": "testtest2", 
            "name": "Иван", 
            "surname": "Иванов", 
            "tg_username": "@ivanov", 
            "user_type": 0, 
            "work_result_list": [],
            "group_id": -1,
            }
    response = requests.post("http://127.0.0.1:8080/api/user", json=data)
    assert(response.status_code == 201)
    assert(cmp(data, response.json()))
    assert("user_id" in response.json())
    user_id = response.json()["user_id"]


def test_get():
    data = {"tg_id": "testtest", 
            "name": "Иван", 
            "surname": "Курышкин", 
            "tg_username": "@kurygas", 
            "user_type": 0, 
            "work_result_list": [],
            "group_id": -1,
            "user_id": user_id
            }
    headers = {"Authorization": "Bearer " + token}
    response = requests.get("http://127.0.0.1:8080/api/user/" + str(user_id), headers=headers)
    assert(response.status_code == 200)
    assert(data == response.json())
