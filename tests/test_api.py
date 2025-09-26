import random_functions
import random
import time
import requests
import json
import auth


def cur_time():
    return int(time.time())


class Object:
    def __init__(self):
        self.id = -1
        self.route = ""

    def assert_request(self, json: dict[str]):
        for key, value in json.items():
            if isinstance(value, list[int]):
                assert sorted(value) == sorted(self.__dict__[key])
            else:
                assert value == self.__dict__[key]
        for key, value in self.__dict__.items():
            assert key == "route" or key in json or value == -1 or value == "" or value == []
            
    def url(self):
        return f"http://127.0.0.1:8080/api{self.route}"
    
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
        pass

    def assert_post_id(self):
        pass


class User(Object):
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
        self.route = "/user"

    def random():
        return User(
            int(random_functions.random_digit_string()),
            random_functions.random_eng_string(),
            random_functions.random_ru_string(),
            random_functions.random_ru_string()
        )


class Group(Object):
    def __init__(self, name: str):
        super().__init__()
        self.name = name
        self.cf_group_code = ""
        self.route = "/group"

    def random():
        return Group(random_functions.random_ru_string())


class Point(Object):
    def __init__(self, reason: str, amount: int, semester_result_id: int):
        super().__init__()
        self.reason = reason
        self.amount = amount
        self.semester_result_id = semester_result_id
        self.user_list: list[int] = []
        self.semester_list: list[int] = []
        self.route = "/point"

    def random(semester_result_id: int):
        return Point(
            random_functions.random_ru_string(),
            random.randint(1, 10),
            semester_result_id
        )


class Problem(Object):
    def __init__(self, name: str, statement: str, subject: int, semester_number: int):
        super().__init__()
        self.name = name
        self.statement = statement
        self.subject = subject
        self.semester_number = semester_number
        self.work_list: list[int] = []
        self.work_result_list: list[int] = []
        self.route = "/problem"

    def random():
        return Problem(
            random_functions.random_ru_string(),
            random_functions.random_ru_string(),
            random.randint(0, 2),
            random.randint(1, 8)
        )


class SemesterResult(Object):
    def __init__(self, semester_id: int, user_id: int):
        super().__init__()
        self.semester_id = semester_id
        self.user_id = user_id
        self.work_result_list: list[int] = []
        self.point_list: list[int] = []
        self.route = "/semester_result"


class Semester(Object):
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
        self.route = "/semester"

    def random(group_id: int):
        return Semester(
            random.randint(1, 8),
            random.randint(0, 2),
            cur_time(),
            cur_time() + 5 * 30 * 24 * 60 * 60,
            group_id
        )


class WorkResult(Object):
    def __init__(self, work_id: int, semester_result_id: int):
        super().__init__()
        self.filename = ""
        self.mark = -1
        self.work_id = work_id
        self.problem_id = -1
        self.semester_result_id = semester_result_id
        self.route = "/work_result"


class Work(Object):
    def __init__(self, name: str, start: int, end: int, semester_id: int, is_exam: bool):
        super().__init__()
        self.name = name
        self.start = start
        self.end = end
        self.is_exam = is_exam
        self.semester_id = semester_id
        self.problem_list: list[int] = []
        self.work_result_list: list[int] = []
        self.route = "/work"

    def random(semester_id: int):
        return Work(
            random_functions.random_ru_string(),
            cur_time(),
            cur_time() + 60 * 60,
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
