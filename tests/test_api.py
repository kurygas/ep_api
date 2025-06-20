from random import choices
from string import ascii_lowercase, digits
from requests import get, post, delete, patch
from datetime import datetime


url = "http://127.0.0.1:8080/api/"


def rand_string():
    return ''.join(choices(ascii_lowercase + digits, k=10))


def rand_ru_string():
    letters = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'
    return ''.join(choices(letters, k=10))


def check_dicts(a, b, keys):
    for key in keys:
        assert(a[key] == b[key])


class User:
    def __init__(self):
        self.user_url = url + "user/"
        self.data = {
            "tg_id": rand_string(),
            "tg_username": "@" + rand_string(),
            "name": rand_ru_string(),
            "surname": rand_ru_string(),
            "password": rand_string(),
            "id": 0,
            "user_type": 0,
            "group_id": -1,
            "work_result_list": [],
            "token": ""
        }

    def test(self):
        self.test_post()
        self.test_auth()
        self.test_get_id()
        self.test_get()
        self.test_patch()

    def user_url_id(self):
        return self.user_url + str(self.data["id"]) + "/"
    
    def auth_params(self):
        return {"Authorization": "Bearer " + self.data["token"]}

    def test_get_id(self):
        response = get(self.user_url_id(), headers=self.auth_params())
        assert response.status_code == 200
        check_dicts(self.data, response.json(), ["tg_username", "name", "surname", "user_type", "group_id", "work_result_list", "id"])

    def test_post(self):
        response = post(self.user_url, json=self.data)
        assert response.status_code == 201
        check_dicts(self.data, response.json(), ["tg_username", "name", "surname", "user_type", "group_id", "work_result_list"])
        self.data["id"] = response.json()["id"]

    def test_auth(self):
        response = post(self.user_url_id() + "auth/", json=self.data)
        assert response.status_code == 200
        self.data["token"] = response.json()["token"]

    def test_patch_arg(self, arg, new_value):
        self.data[arg] = new_value
        response = patch(self.user_url_id(), json={arg: new_value}, headers=self.auth_params())
        assert response.status_code == 200
        check_dicts(self.data, response.json(), ["tg_username", "name", "surname", "user_type", "group_id", "work_result_list", "id"])

    def test_patch(self):
        self.test_patch_arg("tg_username", "@" + rand_string())
        self.test_patch_arg("name", rand_ru_string())
        self.test_patch_arg("surname", rand_ru_string())
        
    def test_get(self):
        response = get(self.user_url, headers=self.auth_params())
        assert response.status_code == 200
        assert self.data["id"] in response.json()["user_list"]


def test_api():
    token = "Bearer qEl1Qzs3UWSiN3Lb"
    start = datetime(2025, 6, 20, 10)
    end = datetime(2025, 6, 20, 11)
    json = {
        "name": "test_work",
        "start": int(start.timestamp()),
        "end": int(end.timestamp()),
        "subject": 0,
        "semester": 1,
        "work_number": 1,
        
    }
    response = post(url + "work/", headers={"Authorization": token}, json=json)
    print(response.json())
    assert response.status_code == 201
