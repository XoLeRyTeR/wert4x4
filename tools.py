import uuid
import requests
import urllib3


def get_ans(task, lang="python, ввод данных осуществляется с помощью input()"):

    urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

    AUTH_KEY = "MDE5OWMyZTItMjUxZi03ZDZkLWI4ZDEtZTQ2NzU3ZGEwMzgyOjY5MDZkMDYyLWQ2MzUtNDRjNy04Mzc0LTkyZGU5YmY0YjAwMQ=="

    # =========================
    # Получение access token
    # =========================

    oauth_url = "https://ngw.devices.sberbank.ru:9443/api/v2/oauth"

    oauth_headers = {
        "Content-Type": "application/x-www-form-urlencoded",
        "Accept": "application/json",
        "RqUID": str(uuid.uuid4()),
        "Authorization": f"Basic {AUTH_KEY}"
    }

    oauth_data = {
        "scope": "GIGACHAT_API_PERS"
    }

    oauth_response = requests.post(
        oauth_url,
        headers=oauth_headers,
        data=oauth_data,
        verify=False
    )

    oauth_response.raise_for_status()

    access_token = oauth_response.json()["access_token"]

    chat_url = "https://gigachat.devices.sberbank.ru/api/v1/chat/completions"

    chat_headers = {
        "Content-Type": "application/json",
        "Accept": "application/json",
        "Authorization": f"Bearer {access_token}"
    }

    payload = {
        "model": "GigaChat",
        "messages": [
            {
                "role": "user",
                "content": f"Ты опытный спортивный программист, которому все задачи не по чем, тебе будет на вход подаваться задача, и ты должен решить ее на {lang}, в коде не должно быть никаких пояснений и комментариев, пиши код максимально просто"
            }
        ],
        "stream": False
    }

    response = requests.post(
        chat_url,
        headers=chat_headers,
        json=payload,
        verify=False
    )

    response.raise_for_status()

    return response.json()["choices"][0]["message"]["content"]
