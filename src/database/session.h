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

class Session : public Wt::Dbo::Session {
public:
    Session();
    static void configureDatabase();

    Wt::WString generateToken();

    Wt::Dbo::ptr<Problem> getCurrentProblem(const Wt::Dbo::ptr<User>& user);

    template<typename T>
    Wt::Dbo::ptr<T> create(const Wt::Json::Object& json) {
        if (exist(&Session::getByName<T>, json.at(Str::name))) {
            throw UnprocessableEntityException("Name already exists");
        }

        return add(std::make_unique<T>(json));
    }

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
            if (ptr->getTokenTimeLimit() < Wt::WDateTime::currentDateTime()) {
                ptr.modify()->setToken(generateToken());
            }
        }

        return ptr;
    }
};

template<>
Wt::Dbo::ptr<WorkResult> Session::create<WorkResult>(const Wt::Json::Object& json) {
    return add(std::make_unique<WorkResult>(getById<Work>(json.at(Str::workId)), getById<User>(json.at(Str::userId))));
}

template<>
Wt::Dbo::ptr<User> Session::create<User>(const Wt::Json::Object& json) {
    if (exist(&Session::getByTgId<User>, json.at(Str::tgId))) {
        throw UnprocessableEntityException("User already exists");
    }

    return add(std::make_unique<User>(json));
}

template<>
Wt::Dbo::ptr<Problem> Session::create<Problem>(const Wt::Json::Object& json) {
    const auto results = find<Problem>()
        .where("subject = ?").bind(json.at(Str::subject))
        .where("semester = ?").bind(json.at(Str::semester))
        .where("work_number = ?").bind(json.at(Str::workNumber))
        .where("name = ?").bind(json.at(Str::name));

    if (!results.resultList().empty()) {
        throw UnprocessableEntityException("Problem already exists");
    }

    return add(std::make_unique<Problem>(json));
}
