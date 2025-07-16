#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/SqlTraits.h>
#include <Wt/WString.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

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
    WorkResult(Ptr<Work> work, Ptr<SemesterResult> semesterResult);

    void setFilename(std::string filename);
    void setWork(Ptr<Work> work);
    void setProblem(Ptr<Problem> problem);
    void setSemesterResult(Ptr<SemesterResult> semesterResult);
    void setMark(int mark);

    const std::string& getFilename() const;
    int getMark() const;
    const Ptr<Work>& getWork() const;
    const Ptr<Problem>& getProblem() const;
    const Ptr<SemesterResult>& getSemesterResult() const;  

private:
    std::string filename_;
    int mark_ = -1;

    Ptr<Work> work_;
    Ptr<Problem> problem_;
    Ptr<SemesterResult> semesterResult_;
};
