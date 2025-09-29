import random_functions
import random
import time
import requests
import string
import auth


def cur_time():
    return int(time.time())


class ApiObject:
    def __init__(self):
        self.id = -1

    def assert_request(self, json: dict[str]):
        for key, value in json.items():
            if isinstance(value, list[int]):
                assert sorted(value) == sorted(self.__dict__[key])
            else:
                assert value == self.__dict__[key]
        for key, value in self.__dict__.items():
            assert key in json or value == -1 or value == "" or value == []

    def route(self):
        return ""
            
    def url(self):
        return f"http://127.0.0.1:8080/api{self.route()}"
    
    def url_id(self):
        assert self.id != -1
        return f"{self.url()}/{self.id}"
    
    def assert_object(self):
        self.assert_post()
        self.assert_get()
        self.assert_get_id()
        self.assert_patch_id()
        self.assert_post_id()
    
    def assert_get(self):
        r = requests.get(url=self.url(), headers=auth.get_auth_headers())
        assert r.status_code == 200
        json: list[int] = r.json()
        if self.id != -1:
            assert self.id in json
        return json
    
    def assert_post(self):
        r = requests.post(url=self.url(), headers=auth.get_auth_headers(), json=self.__dict__)
        assert r.status_code == 201
        json: dict[str] = r.json()
        self.id = json["id"]
        self.assert_request(json)

    def assert_get_id(self):
        r = requests.get(url=self.url_id(), headers=auth.get_auth_headers())
        assert r.status_code == 200
        self.assert_request(r.json())

    def assert_patch_id(self):
        for key in self.__dict__:
            if not self.set_random_value(key):
                continue
            r = requests.patch(url=self.url_id(), headers=auth.get_auth_headers(), json={key: self.__dict__[key]})
            assert r.status_code == 200
            self.assert_request(r.json())
            

    def assert_post_id(self):
        pass

    
    def get_random_int(key: str):
        int_restrictions = {
            "user_type": (0, 2),
            "amount": (1, 10),
            "subject": (0, 2),
            "semester_number": (1, 8),
            "start": (cur_time() - 3600, cur_time() + 3600),
            "end": (cur_time() + 23 * 3600, cur_time() + 25 * 3600),
            "mark": (0, 12)
        }
        if key not in int_restrictions:
            return None
        return random.randint(*int_restrictions[key])

    def set_random_value(self, key: str):
        current_value = self.__dict__[key]
        if isinstance(current_value, str):
            if len(current_value) != 0 and current_value[0] not in string.ascii_letters:
                self.__dict__[key] = random_functions.random_ru_string()
            else:
                self.__dict__[key] = random_functions.random_eng_string()
        elif isinstance(current_value, int):
            value = ApiObject.get_random_int(key)
            if value is not None:
                self.__dict__[key] = value
            else:
                return False
        elif isinstance(current_value, bool):
            self.__dict__[key] = random.choice((False, True))
        else:
            return False
        return True
    
    def patch_arg(self, key: str, value):
        self.__dict__[key] = value
        r = requests.patch(url=self.url_id(), headers=auth.get_auth_headers(), json={key: value})
        assert r.status_code == 200
        self.assert_request(r.json())
    

class User(ApiObject):
    def __init__(self, tg_id: int, tg_username: str, name: str, surname: str):
        super().__init__()
        self.tg_id = tg_id
        self.tg_username = tg_username
        self.name = name
        self.surname = surname
        self.user_type = 0
        self.cf_name = ""
        self.atc_name = ""
        self.group_id = -1
        self.semester_result_list: list[int] = []

    def route(self):
        return "/user"

    def random():
        return User(
            random.randint(1, 10 ** 10),
            random_functions.random_eng_string(),
            random_functions.random_ru_string(),
            random_functions.random_ru_string()
        )


class Group(ApiObject):
    def __init__(self, name: str):
        super().__init__()
        self.name = name
        self.cf_group_code = ""
        self.user_list: list[int] = []

    def route(self):
        return "/group"

    def random():
        return Group(random_functions.random_ru_string())


class Point(ApiObject):
    def __init__(self, reason: str, amount: int, semester_result_id: int):
        super().__init__()
        self.reason = reason
        self.amount = amount
        self.semester_result_id = semester_result_id
        self.user_list: list[int] = []
        self.semester_list: list[int] = []

    def route(self):
        return "/point"

    def assert_request(self, json: dict[str]):
        if json["reason"] == "cf_point" or json["reason"] == "atc_point":
            self.amount = json["amount"]
        super().assert_request(json)

    def random(semester_result_id: int):
        return Point(
            random_functions.random_ru_string(),
            ApiObject.get_random_int("amount"),
            semester_result_id
        )


class Problem(ApiObject):
    def __init__(self, name: str, statement: str, subject: int, semester_number: int):
        super().__init__()
        self.name = name
        self.statement = statement
        self.subject = subject
        self.semester_number = semester_number
        self.work_list: list[int] = []
        self.work_result_list: list[int] = []

    def route(self):
        return "/problem"

    def random():
        return Problem(
            random_functions.random_ru_string(),
            random_functions.random_ru_string(),
            ApiObject.get_random_int("subject"),
            ApiObject.get_random_int("semester_number")
        )


class SemesterResult(ApiObject):
    def __init__(self, semester_id: int, user_id: int):
        super().__init__()
        self.semester_id = semester_id
        self.user_id = user_id
        self.work_result_list: list[int] = []
        self.point_list: list[int] = []

    def route(self):
        return "/semester_result"


class Semester(ApiObject):
    def __init__(self, semester_number: int, subject: int, start: int, end: int, group_id: int):
        super().__init__()
        self.semester_number = semester_number
        self.subject = subject
        self.start = start
        self.end = end
        self.cf_max_point = -1
        self.atc_ratio = -1
        self.group_id = group_id
        self.semester_result_list: list[int] = []

    def route(self):
        return "/semester"

    def random(group_id: int):
        return Semester(
            ApiObject.get_random_int("semester_number"),
            ApiObject.get_random_int("subject"),
            ApiObject.get_random_int("start"),
            ApiObject.get_random_int("end"),
            group_id
        )


class WorkResult(ApiObject):
    def __init__(self, work_id: int, semester_result_id: int):
        super().__init__()
        self.filename = ""
        self.mark = -1
        self.work_id = work_id
        self.problem_id = -1
        self.semester_result_id = semester_result_id

    def route(self):
        return "/work_result"


class Work(ApiObject):
    def __init__(self, name: str, start: int, end: int, semester_id: int, is_exam: bool):
        super().__init__()
        self.name = name
        self.start = start
        self.end = end
        self.is_exam = is_exam
        self.semester_id = semester_id
        self.problem_list: list[int] = []
        self.work_result_list: list[int] = []

    def route(self):
        return "/work"

    def random(semester_id: int):
        return Work(
            random_functions.random_ru_string(),
            ApiObject.get_random_int("start"),
            ApiObject.get_random_int("end"),
            semester_id,
            random.choice([False, True])
        )


user: User = None
group: Group = None
point: Point = None
problem: Problem = None
semester_result: SemesterResult = None
semester: Semester = None
work_result: WorkResult = None
work: Work = None


def test_user():
    global user
    user = User.random()
    user.assert_object()


def test_group():
    global group
    group = Group.random()
    group.assert_object()
    user.patch_arg("group_id", group.id)
    group.user_list.append(user.id)
    group.assert_get_id()
