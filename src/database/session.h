#pragma once

#include <Wt/Dbo/Session.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

#include "subject_type.h"
#include "http_exceptions.h"

class User;
class Group;
class Work;
class WorkResult;
class Problem;

class Session : public Wt::Dbo::Session {
public:
    Session();
    void configureDatabase();

    Wt::WString generateToken();

    Wt::Dbo::ptr<User> addUser(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, 
        const Wt::WString& firstName, const Wt::WString& secondName, const Wt::WString& email);
    Wt::Dbo::ptr<Group> addGroup(const Wt::WString& name);
    Wt::Dbo::ptr<Work> addWork(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, Subject::Type subject, 
        int semester, int workNumber);
    Wt::Dbo::ptr<WorkResult> addWorkResult(int userId, int workId);
    Wt::Dbo::ptr<Problem> addProblem(const Wt::WString& name, const Wt::WString& statement, Subject::Type subject, int semester, int workNumber);

    Wt::Dbo::ptr<User> getUserByTgId(const Wt::WString& tgId);
    Wt::Dbo::ptr<User> getUserByToken(const Wt::WString& token);

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
            throw NotFoundException();
        }

        if constexpr (std::is_same_v<Ptr, Wt::Dbo::ptr<User>>) {
            if (ptr->getTokenTimeLimit() < Wt::WDateTime::currentDateTime()) {
                ptr.modify()->setToken(generateToken());
            }
        }

        return ptr;
    }

    template<typename T>
    void checkName(const Wt::WString& name) {
        if (exist(&Session::getByName<T>, name)) {
            throw UnprocessableEntityException("This name already exists");
        }
    }
};
