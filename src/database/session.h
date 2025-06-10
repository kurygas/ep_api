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

class User;
class Group;
class Work;
class WorkResult;
class Problem;

class Session : public Wt::Dbo::Session {
public:
    Session();
    static void configureDatabase();

    Wt::WString generateToken();

    template<typename T>
    Wt::Dbo::ptr<T> create(const Wt::Json::Object& json) {
        if constexpr (std::is_same_v<T, User>) {
            if (exist(&Session::getByTgId<User>, json.at(Str::tgId))) {
                throw UnprocessableEntityException("User already exists");
            }
        }
        else {
            if (exist(&Session::getByName<T>, json.at(Str::name))) {
                throw UnprocessableEntityException("Name already exists");
            }
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
