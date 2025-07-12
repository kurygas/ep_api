#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/SqlTraits.h>
#include <Wt/WString.h>

#include "json.h"
#include "str.h"
#include "types.h"

class SemesterResult;
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
        Wt::Dbo::belongsTo(a, semesterResult_, "semester_result");
    }

    operator Wt::Json::Object() const;

    WorkResult() = default;
    WorkResult(const Ptr<Work>& work, const Ptr<SemesterResult>& semesterResult);

    void setFilename(const Wt::WString& filename);
    void setMark(int mark);
    void setWork(const Ptr<Work>& work);
    void setProblem(const Ptr<Problem>& problem);
    void setSemesterResult(const Ptr<SemesterResult>& semesterResult);

    const Wt::WString& getFilename() const;
    int getMark() const;
    const Ptr<Work>& getWork() const;
    const Ptr<Problem>& getProblem() const;
    const Ptr<SemesterResult>& getSemesterResult() const;  

private:
    Wt::WString filename_;
    int mark_ = -1;

    Ptr<Work> work_;
    Ptr<Problem> problem_;
    Ptr<SemesterResult> semesterResult_;
};
