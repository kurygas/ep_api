#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"
#include "json.h"
#include "str.h"
#include "types.h"

class Work;
class WorkResult;

class Problem {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, statement_, Str::statement);
        Wt::Dbo::field(a, subject_, Str::subject);
        Wt::Dbo::field(a, semester_, Str::semester);
        Wt::Dbo::field(a, workNumber_, Str::workNumber);
        Wt::Dbo::hasMany(a, works_, Wt::Dbo::ManyToMany, "work_problems");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "problem");
    }

    operator Wt::Json::Object() const;

    Problem() = default;
    Problem(const Wt::WString& name, const Wt::WString& statement, Subject::Type subject, int semester, int workNumber);

    void setName(const Wt::WString& name);
    void setStatement(const Wt::WString& statement);
    void setSubject(Subject::Type subject);
    void setSemester(int semester);
    void setWorkNumber(int workNumber);

    const Wt::WString& getName() const;
    const Wt::WString& getStatement() const;
    Subject::Type getSubject() const;
    int getSemester() const;
    int getWorkNumber() const;
    const List<Work>& getWorks() const;
    const List<WorkResult>& getWorkResults() const;

private:
    Wt::WString name_;
    Wt::WString statement_;
    Subject::Type subject_;
    int semester_;
    int workNumber_;

    List<Work> works_;
    List<WorkResult> workResults_;
};
