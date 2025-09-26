import requests
import configparser


def test_admin_valid():
    config = configparser.ConfigParser()
    config.read("../auth_service/config.ini")
    tg_bot_password = config.get("tg_bot", "password")
    response = requests.post("http://127.0.0.1:8081/auth/refresh_token", json={"name": "tg_bot", "password": tg_bot_password})
    assert response.status_code == 200
    json = response.json()
    assert "refresh_token" in json
    refresh_token = json["refresh_token"]
    response = requests.post("http://127.0.0.1:8081/auth/access_token", json={"refresh_token": refresh_token})
    assert response.status_code == 200
    json = response.json()
    assert "access_token" in json
    access_token = json["access_token"]
    response = requests.get("http://127.0.0.1:8080/api/group", headers={"Authorization": f"Bearer {access_token}"})
    assert response.status_code == 200


def test_admin_invalid():
    response = requests.post("http://127.0.0.1:8081/auth/refresh_token")
    assert response.status_code == 400
    assert "error" in response.json()
    response = requests.post("http://127.0.0.1:8081/auth/refresh_token", json={"name": "tg_bot"})
    assert response.status_code == 400
    assert "error" in response.json()
    response = requests.post("http://127.0.0.1:8081/auth/refresh_token", json={"name": "tg_bot", "password": "wrong_password"})
    assert response.status_code == 400
    assert "error" in response.json()
    response = requests.post("http://127.0.0.1:8081/auth/access_token")
    assert response.status_code == 400
    assert "error" in response.json()
    response = requests.post("http://127.0.0.1:8081/auth/access_token", json={"refresh_token": "wrong_token"})
    assert response.status_code == 400
    assert "error" in response.json()
