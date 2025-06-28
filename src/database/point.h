#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

class User;
class SemesterResult;

class Point {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, reason_, Str::reason);
        Wt::Dbo::field(a, amount_, Str::amount);
        Wt::Dbo::belongsTo(a, semesterResult_, "semester_result");
    }

    operator Wt::Json::Object() const;

    Point() = default;
    Point(const Wt::WString& reason, int amount, const Ptr<SemesterResult>& semesterResult);

    void setReason(const Wt::WString& reason);
    void setAmount(int amount);
    void setSemesterResult(const Ptr<SemesterResult>& semesterResult);

    const Wt::WString& getReason() const;
    int getAmount() const;
    const Ptr<SemesterResult>& getSemesterResult() const;

private:
    Wt::WString reason_;
    int amount_;

    Ptr<SemesterResult> semesterResult_;
};
