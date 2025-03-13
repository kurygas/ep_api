import requests

payload = {"tg_id": "1234", "token": "ekQnFd5HicZ2iQsM", "target": "Владимир"}
response = requests.post("http://127.0.0.1:8080/user/update_profile", json=payload)
print(response.status_code)
print(response.content)