#pragma once

#include "subject_type.h"

#include <Wt/Dbo/Session.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

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
    Wt::Dbo::ptr<Group> addGroup(const Wt::WString& groupName);
    Wt::Dbo::ptr<Work> addWork(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, SubjectType subject, int semester, 
        int workNumber);
    Wt::Dbo::ptr<WorkResult> addWorkResult(const Wt::WString& token);
    Wt::Dbo::ptr<Problem> addProblem(const Wt::WString& name, const Wt::WString& statement, SubjectType subject, int semester, int workNumber);

    Wt::Dbo::ptr<User> getUserByTgId(const Wt::WString& tgId);
    Wt::Dbo::ptr<User> getUserByToken(const Wt::WString& token);
    Wt::Dbo::ptr<Group> getGroupByName(const Wt::WString& groupName);
    Wt::Dbo::ptr<Group> getGroupById(int groupId);
    Wt::Dbo::ptr<Problem> getProblemByName(const Wt::WString& problemName);
    Wt::Dbo::ptr<Work> getWorkByName(const Wt::WString& workName);
    Wt::Dbo::ptr<Work> getWorkByTimeSegment(const Wt::WDateTime& start, const Wt::WDateTime& end);
    Wt::Dbo::ptr<Work> getCurrentWork(const Wt::Dbo::ptr<Group>& group);

    template<typename F, typename... Args>
    bool exist(const F method, Args&&... args) {
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
            throw std::runtime_error("Does not exist");
        }

        if constexpr (std::is_same_v<Ptr, Wt::Dbo::ptr<User>>) {
            if (ptr->getTokenTimeLimit() < Wt::WDateTime::currentDateTime()) {
                ptr.modify()->setToken(generateToken());
            }
        }

        return ptr;
    }
};
