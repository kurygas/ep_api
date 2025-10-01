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

    def assert_request(self, r: requests.Response, status_code: int):
        json: dict[str] = r.json()
        try:
            assert r.status_code == status_code
            if self.id == -1:
                self.id = json["id"]
            for key, value in json.items():
                if isinstance(value, list):
                    for id in self.__dict__[key]:
                        assert id in value
                else:
                    assert value == self.__dict__[key]
            for key, value in self.__dict__.items():
                assert key in json or value == -1 or value == "" or value == []
        except AssertionError:
            print(json)
            raise

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
        try:
            r = requests.get(url=self.url(), headers=auth.get_auth_headers())
            assert r.status_code == 200
            json: dict[str] = r.json()
            if self.id != -1:
                assert self.id in json["list"]
        except AssertionError:
            print(json)
            raise
    
    def assert_post(self):
        r = requests.post(url=self.url(), headers=auth.get_auth_headers(), json=self.__dict__)
        self.assert_request(r, 201)

    def assert_get_id(self):
        r = requests.get(url=self.url_id(), headers=auth.get_auth_headers())
        self.assert_request(r, 200)

    def assert_patch_id(self):
        for key in self.__dict__:
            if key != "filename" and not self.set_random_value(key):
                continue
            if key == "filename":
                file = open("/home/kuryga/projects/ep_api/tests/test.cpp")
                r = requests.patch(
                    url=self.url_id(), 
                    headers=auth.get_auth_headers(), 
                    json={key: self.__dict__[key]}, 
                    files={"test.cpp": file}
                )
            else:
                r = requests.patch(url=self.url_id(), headers=auth.get_auth_headers(), json={key: self.__dict__[key]})
            self.assert_request(r, 200)
            

    def assert_post_id(self):
        pass


    def assert_delete(self):
        pass

    
    def get_random_int(key: str):
        int_restrictions = {
            "user_type": (0, 1),
            "amount": (0, 10),
            "start": (cur_time() - 3600, cur_time() + 3600),
            "end": (cur_time() + 23 * 3600, cur_time() + 25 * 3600),
            "mark": (0, 12),
            "cf_max_point": (1, 15),
            "atc_ratio": (100, 200)
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
    
    def set_group(self, group: Group):
        self.group_id = group.id
        group.user_list.append(self.id)
        r = requests.patch(url=self.url_id(), headers=auth.get_auth_headers(), json={"group_id": group.id})
        self.assert_request(r, 200)


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
            0,
            1
        )


class SemesterResult(ApiObject):
    def __init__(self, semester_id: int, user_id: int):
        super().__init__()
        self.semester_id = semester_id
        self.user_id = user_id
        self.work_result_list: list[int] = []
        self.point_list: list[int] = []
        for id in requests.get(url=self.url(), headers=auth.get_auth_headers()).json()["list"]:
            json: dict[str] = requests.get(url=f"{self.url()}/{id}", headers=auth.get_auth_headers()).json()
            if json["semester_id"] == self.semester_id and json["user_id"] == self.user_id:
                self.id = json["id"]
                break

    def route(self):
        return "/semester_result"
    
    def assert_object(self):
        self.assert_get()
        self.assert_get_id()
        self.assert_patch_id()
        self.assert_post_id()


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
            1,
            0,
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


def test_api():
    user = User.random()
    user.assert_object()

    group = Group.random()
    group.assert_object()
    
    user.set_group(group)

    semester = Semester.random(group.id)
    semester.assert_object()

    semester_result = SemesterResult(semester.id, user.id)
    semester_result.assert_object()

    work = Work.random(semester.id)
    work.assert_object()

    problem = Problem.random()
    problem.assert_object()

    work_result = WorkResult(work.id, semester_result.id)
    work_result.assert_object()

