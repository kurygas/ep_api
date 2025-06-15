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

class Session : public Wt::Dbo::Session {
public:
    Session();
    static void configureDatabase();

    Wt::WString generateToken();

    template<typename T>
    Wt::Dbo::ptr<T> create(const Wt::Json::Object& json);

    template<typename T>
    Wt::Dbo::collection<Wt::Dbo::ptr<T>> getByArray(const Wt::Json::Array& array) {
        Wt::Dbo::collection<Wt::Dbo::ptr<T>> collection;
        
        for (const auto& id : array) {
            collection.insert(getById<T>(id));
        }

        return collection;
    }

    template<typename T>
    Wt::Dbo::ptr<T> getByTgId(const Wt::WString& tgId) {
        return getPtr(find<T>().where("tg_id = ?").bind(tgId));
    }

    template<typename T>
    Wt::Dbo::ptr<T> getByToken(const Wt::WString& token) {
        return getPtr(find<T>().where("token = ?").bind(token));
    }

    template<typename T>
    Wt::Dbo::ptr<T> getById(const int id) {
        return getPtr(find<T>().where("id = ?").bind(id));
    }

    template<typename T>
    Wt::Dbo::ptr<T> getByName(const Wt::WString& name) {
        return getPtr(find<T>().where("name = ?").bind(name));
    }

    Wt::Dbo::ptr<Problem> getProblem(Subject::Type subject, int semester, int workNumber, const Wt::WString& name);
    Wt::Dbo::ptr<Work> getWork(Subject::Type subject, int semester, int workNumber, const Wt::Dbo::ptr<Group>& group);
    Wt::Dbo::ptr<WorkResult> getWorkResult(const Wt::Dbo::ptr<Work>& work, const Wt::Dbo::ptr<User>& user);

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
    Wt::Dbo::collection<Wt::Dbo::ptr<T>> getAll() {
        return find<T>().resultList();
    }

private:
    template<typename Ptr, typename... Other>
    Ptr getPtr(Wt::Dbo::Query<Ptr, Other...>& collection) {
        auto ptr = static_cast<Ptr>(collection);
        
        if (!ptr) {
            throw NotFoundException("");
        }

        if constexpr (std::is_same_v<Ptr, Wt::Dbo::ptr<User>>) {
            if (ptr->getUserType() != UserType::Admin && ptr->getTokenTimeLimit() < Wt::WDateTime::currentDateTime()) {
                ptr.modify()->setToken(generateToken());
            }
        }

        return ptr;
    }

    void checkAdmin(const Wt::WString& name);
};

template<>
inline Wt::Dbo::ptr<WorkResult> Session::create<WorkResult>(const Wt::Json::Object& json) {
    const auto work = getById<Work>(json.at(Str::workId));
    const auto user = getById<User>(json.at(Str::userId));

    if (exist(&Session::getWorkResult, work, user)) {
        throw UnprocessableEntityException("WorkResult result already exists");
    }

    return add(std::make_unique<WorkResult>(work, user));
}

template<>
inline Wt::Dbo::ptr<User> Session::create<User>(const Wt::Json::Object& json) {
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
inline Wt::Dbo::ptr<Problem> Session::create<Problem>(const Wt::Json::Object& json) {
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
inline Wt::Dbo::ptr<Work> Session::create<Work>(const Wt::Json::Object& json) {
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
inline Wt::Dbo::ptr<Group> Session::create<Group>(const Wt::Json::Object& json) {
    const auto name = json.at(Str::name);

    if (exist(&Session::getByName<Group>, name)) {
        throw UnprocessableEntityException("Group already exists");
    }

    return add(std::make_unique<Group>(name));
}

template<>
inline Wt::Dbo::ptr<Point> Session::create<Point>(const Wt::Json::Object& json) {
    const auto reason = json.at(Str::reason);
    const auto amount = json.at(Str::amount);
    const auto user = getById<User>(json.at(Str::userId));
    return add(std::make_unique<Point>(reason, amount, user));
}
