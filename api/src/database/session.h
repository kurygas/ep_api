#pragma once

#include <Wt/Dbo/Session.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

#include "subject_type.h"
#include "http_exceptions.h"
#include "str.h"
#include "user.h"
#include "group.h"
#include "problem.h"
#include "work_result.h"
#include "work.h"
#include "point.h"
#include "types.h"
#include "semester.h"
#include "semester_result.h"

class Session : public Wt::Dbo::Session {
public:
    Session();
    static void configureDatabase();

    std::string generateToken();

    template<typename T>
    Ptr<T> create(const Wt::Json::Object& json);

    template<typename T>
    List<T> getByArray(const Wt::Json::Array& array) {
        List<T> collection;
        
        for (const auto& id : array) {
            collection.insert(load<T>(id));
        }

        return collection;
    }

    template<typename T>
    Ptr<T> getByTgId(int64_t tgId) {
        return getPtr(find<T>().where("tg_id = ?").bind(tgId));
    }

    template<typename T>
    Ptr<T> getByName(const std::string& name) {
        return getPtr(find<T>().where("name = ?").bind(name));
    }

    Ptr<Work> getWork(const Ptr<Semester>& semester, const std::string& name);
    Ptr<WorkResult> getWorkResult(const Ptr<Work>& work, const Ptr<SemesterResult>& semesterResult);
    Ptr<Semester> getSemester(Subject::Type subject, int semesterNumber, const Ptr<Group>& group);
    Ptr<SemesterResult> getSemesterResult(const Ptr<Semester>& semester, const Ptr<User>& user);
    Ptr<Point> getPoint(const Ptr<SemesterResult>& SemesterResult, const std::string& reason);

    template<typename F, typename... Args>
    bool exist(F method, Args&&... args) {
        try {
            (this->*method)(std::forward<Args>(args)...);
        }
        catch (...) {
            return false;
        }

        return true;
    }
    
    template<typename T> 
    List<T> getAll() {
        return find<T>().resultList();
    }

private:
    template<typename T, typename... Other>
    T getPtr(Wt::Dbo::Query<T, Other...>& collection) {
        auto ptr = static_cast<T>(collection);
        
        if (!ptr) {
            throw NotFoundException("");
        }

        return ptr;
    }
};

template<>
inline Ptr<WorkResult> Session::create<WorkResult>(const Wt::Json::Object& json) {
    auto work = load<Work>(json.at(Str::workId));
    auto semesterResult = load<SemesterResult>(json.at(Str::semesterResultId));

    if (exist(&Session::getWorkResult, work, semesterResult)) {
        throw UnprocessableEntityException("WorkResult result already exists");
    }

    return add(std::make_unique<WorkResult>(std::move(work), std::move(semesterResult)));
}

template<>
inline Ptr<User> Session::create<User>(const Wt::Json::Object& json) {
    const auto tgId = static_cast<int>(json.at(Str::tgId));
    auto tgUsername = static_cast<std::string>(json.at(Str::tgUsername));
    auto name = static_cast<std::string>(json.at(Str::name));
    auto surname = static_cast<std::string>(json.at(Str::surname));

    if (exist(&Session::getByTgId<User>, tgId)) {
        throw UnprocessableEntityException("User already exists");
    }

    return add(std::make_unique<User>(tgId, std::move(tgUsername), std::move(name), std::move(surname)));
}

template<>
inline Ptr<Problem> Session::create<Problem>(const Wt::Json::Object& json) {
    auto name = static_cast<std::string>(json.at(Str::name));
    auto statement = static_cast<std::string>(json.at(Str::statement));
    const auto subject = JsonFunctions::parse<Subject::Type>(json.at(Str::subject));
    const auto semesterNumber = static_cast<int>(json.at(Str::semesterNumber));

    if (exist(&Session::getByName<Problem>, name)) {
        throw UnprocessableEntityException("Problem already exists");
    }

    return add(std::make_unique<Problem>(std::move(name), std::move(statement), subject, semesterNumber));
}

template<>
inline Ptr<Work> Session::create<Work>(const Wt::Json::Object& json) {
    auto name = static_cast<std::string>(json.at(Str::name));
    const auto start = Wt::WDateTime::fromTime_t(json.at(Str::start));
    const auto end = Wt::WDateTime::fromTime_t(json.at(Str::end));
    auto semester = load<Semester>(json.at(Str::semesterId));
    const auto isExam = static_cast<bool>(json.at(Str::isExam));
    
    if (exist(&Session::getWork, semester, name)) {
        throw UnprocessableEntityException("Work already exists");
    }

    return add(std::make_unique<Work>(std::move(name), start, end, std::move(semester), isExam));
}

template<>
inline Ptr<Group> Session::create<Group>(const Wt::Json::Object& json) {
    auto name = static_cast<std::string>(json.at(Str::name));

    if (exist(&Session::getByName<Group>, name)) {
        throw UnprocessableEntityException("Group already exists");
    }

    return add(std::make_unique<Group>(std::move(name)));
}

template<>
inline Ptr<Point> Session::create<Point>(const Wt::Json::Object& json) {
    auto reason = static_cast<std::string>(json.at(Str::reason));
    const auto amount = static_cast<int>(json.at(Str::amount));
    auto semesterResult = load<SemesterResult>(json.at(Str::semesterResultId));
    return add(std::make_unique<Point>(std::move(reason), amount, std::move(semesterResult)));
}

template<>
inline Ptr<Semester> Session::create<Semester>(const Wt::Json::Object& json) {
    const auto semesterNumber = static_cast<int>(json.at(Str::semesterNumber));
    const auto subject = JsonFunctions::parse<Subject::Type>(json.at(Str::subject));
    const auto start = Wt::WDateTime::fromTime_t(json.at(Str::start));
    const auto end = Wt::WDateTime::fromTime_t(json.at(Str::end));
    auto group = load<Group>(json.at(Str::groupId));

    if (exist(&Session::getSemester, subject, semesterNumber, group)) {
        throw UnprocessableEntityException("Semester already exists");
    }

    return add(std::make_unique<Semester>(semesterNumber, subject, start, end, std::move(group)));
}

template<>
inline Ptr<SemesterResult> Session::create<SemesterResult>(const Wt::Json::Object& json) {
    auto semester = load<Semester>(json.at(Str::semesterId));
    auto user = load<User>(json.at(Str::userId));

    if (exist(&Session::getSemesterResult, semester, user)) {
        throw UnprocessableEntityException("SemesterResult already exists");
    }

    return add(std::make_unique<SemesterResult>(std::move(semester), std::move(user)));
}
