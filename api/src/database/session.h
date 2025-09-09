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
