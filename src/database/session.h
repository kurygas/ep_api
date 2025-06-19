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

class Session : public Wt::Dbo::Session {
public:
    Session();
    static void configureDatabase();

    Wt::WString generateToken();

    template<typename T>
    Ptr<T> create(const Wt::Json::Object& json);

    template<typename T>
    List<T> getByArray(const Wt::Json::Array& array) {
        List<T> collection;
        
        for (const auto& id : array) {
            collection.insert(getById<T>(id));
        }

        return collection;
    }

    template<typename T>
    Ptr<T> getByTgId(int64_t tgId) {
        return getPtr(find<T>().where("tg_id = ?").bind(tgId));
    }

    template<typename T>
    Ptr<T> getByTgUsername(const Wt::WString& tgUsername) {
        return getPtr(find<T>().where("tg_username = ?").bind(tgUsername));
    }

    template<typename T>
    Ptr<T> getByToken(const Wt::WString& token) {
        return getPtr(find<T>().where("token = ?").bind(token));
    }

    template<typename T>
    Ptr<T> getById(const int id) {
        return getPtr(find<T>().where("id = ?").bind(id));
    }

    template<typename T>
    Ptr<T> getByName(const Wt::WString& name) {
        return getPtr(find<T>().where("name = ?").bind(name));
    }

    Ptr<Problem> getProblem(Subject::Type subject, int semester, int workNumber, const Wt::WString& name);
    Ptr<Work> getWork(Subject::Type subject, int semester, int workNumber, const Ptr<Group>& group);
    Ptr<WorkResult> getWorkResult(const Ptr<Work>& work, const Ptr<User>& user);

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

        if constexpr (std::is_same_v<T, Ptr<User>>) {
            if (ptr->getUserType() != UserType::Admin && ptr->getTokenTimeLimit() < Wt::WDateTime::currentDateTime()) {
                ptr.modify()->setToken(generateToken());
            }
        }

        return ptr;
    }

    void checkAdmin(const Wt::WString& name);
};

template<>
inline Ptr<WorkResult> Session::create<WorkResult>(const Wt::Json::Object& json) {
    const auto work = getById<Work>(json.at(Str::workId));
    const auto user = getById<User>(json.at(Str::userId));

    if (exist(&Session::getWorkResult, work, user)) {
        throw UnprocessableEntityException("WorkResult result already exists");
    }

    return add(std::make_unique<WorkResult>(work, user));
}

template<>
inline Ptr<User> Session::create<User>(const Wt::Json::Object& json) {
    const auto tgId = json.at(Str::tgId);
    const auto tgUsername = json.at(Str::tgUsername);
    const auto name = json.at(Str::name);
    const auto surname = json.at(Str::surname);

    if (exist(&Session::getByTgId<User>, tgId)) {
        throw UnprocessableEntityException("User already exists");
    }

    return add(std::make_unique<User>(tgId, tgUsername, name, surname));
}

template<>
inline Ptr<Problem> Session::create<Problem>(const Wt::Json::Object& json) {
    const auto subject = JsonFunctions::parse<Subject::Type>(json.at(Str::subject));
    const auto semester = json.at(Str::semester);
    const auto workNumber = json.at(Str::workNumber);
    const auto statement = json.at(Str::statement);
    const auto name = json.at(Str::name);

    if (exist(&Session::getProblem, subject, semester, workNumber, name)) {
        throw UnprocessableEntityException("Problem already exists");
    }

    return add(std::make_unique<Problem>(name, statement, subject, semester, workNumber));
}

template<>
inline Ptr<Work> Session::create<Work>(const Wt::Json::Object& json) {
    const auto group = getById<Group>(json.at(Str::groupId));
    const auto subject = JsonFunctions::parse<Subject::Type>(json.at(Str::subject));
    const auto semester = json.at(Str::semester);
    const auto workNumber = json.at(Str::workNumber);
    const auto start = Wt::WDateTime::fromTime_t(json.at(Str::start));
    const auto end = Wt::WDateTime::fromTime_t(json.at(Str::end));

    if (exist(&Session::getWork, subject, semester, workNumber, group)) {
        throw UnprocessableEntityException("Work already exists");
    }

    return add(std::make_unique<Work>(start, end, subject, semester, workNumber, group));
}

template<>
inline Ptr<Group> Session::create<Group>(const Wt::Json::Object& json) {
    const auto name = json.at(Str::name);

    if (exist(&Session::getByName<Group>, name)) {
        throw UnprocessableEntityException("Group already exists");
    }

    return add(std::make_unique<Group>(name));
}

template<>
inline Ptr<Point> Session::create<Point>(const Wt::Json::Object& json) {
    const auto reason = json.at(Str::reason);
    const auto amount = json.at(Str::amount);
    const auto user = getById<User>(json.at(Str::userId));
    return add(std::make_unique<Point>(reason, amount, user));
}
