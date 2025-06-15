#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/SqlTraits.h>
#include <Wt/WString.h>

#include "json.h"
#include "str.h"
#include "types.h"

class User;
class Problem;
class Work;

class WorkResult {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, filename_, Str::filename);
        Wt::Dbo::field(a, mark_, Str::mark);
        Wt::Dbo::belongsTo(a, work_, "work");
        Wt::Dbo::belongsTo(a, problem_, "problem");
        Wt::Dbo::belongsTo(a, user_, "user");
    }

    operator Wt::Json::Object() const;
    static const std::string& getListName();

    WorkResult() = default;
    WorkResult(const Ptr<Work>& work, const Ptr<User>& user);

    void setFilename(const Wt::WString& filename);
    void setMark(int mark);
    void setWork(const Ptr<Work>& work);
    void setProblem(const Ptr<Problem>& problem);
    void setUser(const Ptr<User>& user);

    const Wt::WString& getFilename() const;
    int getMark() const;
    const Ptr<Work>& getWork() const;
    const Ptr<Problem>& getProblem() const;
    const Ptr<User>& getUser() const;
    std::string getSolutionPath() const;    

private:
    Wt::WString filename_;
    int mark_;

    Ptr<Work> work_;
    Ptr<Problem> problem_;
    Ptr<User> user_;
};
