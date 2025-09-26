from requests import post
from configparser import ConfigParser


def get_admin_access_token() -> str:
    config = ConfigParser()
    config.read("/home/kuryga/projects/ep_api/auth_service/config.ini")
    tg_bot_password = config.get("tg_bot", "password")
    response = post("http://127.0.0.1:8081/auth/refresh_token", json={"name": "tg_bot", "password": tg_bot_password})
    refresh_token: str = response.json()["refresh_token"]
    response = post("http://127.0.0.1:8081/auth/access_token", json={"refresh_token": refresh_token})
    return response.json()["access_token"]

def get_auth_headers():
    return {"Authorization": f"Bearer {get_admin_access_token()}"}
