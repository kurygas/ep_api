import random_functions
import random
import time
import requests
import json


def get_url():
    return "http://127.0.0.1:8080/api"


def cur_time():
    return int(time.time())

def compare


class User:
    def __init__(self, tg_id, tg_username, name, surname):
        self.id = -1
        self.tg_id = tg_id
        self.tg_username = tg_username
        self.name = name
        self.surname = surname
        self.user_type = 0
        self.cf_name = ""
        self.atc_name = ""
        self.group = None
        self.semester_result_list = []

    def random():
        return User(
            int(random_functions.random_digit_string()),
            random_functions.random_eng_string,
            random_functions.random_ru_string(),
            random_functions.random_ru_string()
        )


class Group:
    def __init__(self, name):
        self.name = name
        self.cf_group_code = ""

    def random():
        return Group(random_functions.random_ru_string())


class Point:
    def __init__(self, reason, amount, semester_result_id):
        self.reason = reason
        self.amount = amount
        self.semester_result_id = semester_result_id
        self.user_list = []
        self.semester_list = []

    def random(semester_result_id):
        return Point(
            random_functions.random_ru_string(),
            random.randint(1, 10),
            semester_result_id
        )


class Problem:
    def __init__(self, name, statement, subject, semester_number):
        self.name = name
        self.statement = statement
        self.subject = subject
        self.semester_number = semester_number
        self.work_list = []
        self.work_result_list = []

    def random():
        return Problem(
            random_functions.random_ru_string(),
            random_functions.random_ru_string(),
            random.randint(0, 2),
            random.randint(1, 8)
        )


class SemesterResult:
    def __init__(self, semester_id, user_id):
        self.semester_id = semester_id
        self.user_id = user_id
        self.work_result_list = []
        self.point_list = []


class Semester:
    def __init__(self, semester_number, subject, start, end, group_id):
        self.semester_number = semester_number
        self.subject = subject
        self.start = start
        self.end = end
        self.cf_max_point = 0
        self.atc_ratio = 0
        self.group_id = group_id
        self.semester_result_list = []

    def random(group_id):
        return Semester(
            random.randint(1, 8),
            random.randint(0, 2),
            cur_time(),
            cur_time() + 5 * 30 * 24 * 60 * 60,
            group_id
        )


class WorkResult:
    def __init__(self, work_id, semester_result_id):
        self.filename = ""
        self.mark = -1
        self.work_id = work_id
        self.problem = None
        self.semester_result_id = semester_result_id


class Work:
    def __init__(self, name, start, end, semester_id, is_exam):
        self.name = name
        self.start = start
        self.end = end
        self.is_exam = is_exam
        self.semester_id = semester_id
        self.problem_list = []
        self.work_result_list = []

    def random(semester_id):
        return Work(
            random_functions.random_ru_string(),
            cur_time(),
            cur_time() + 60 * 60,
            semester_id,
            random.choice([False, True])
        )


user = None
group = None
point = None
problem = None
semester_result = None
semester = None
work_result = None
work = None

def test_user():
    global user
    user = User.random()
    r = requests.post(f"{get_url()}/user", json=user.__dict__)
    assert r.status_code == 201
    assert r.json()[""]

