#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "crypto.h"
#include "user.h"
#include "semester_result.h"

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
    Point(std::string reason, int amount, Ptr<SemesterResult> semesterResult);

    void setReason(std::string reason);
    void setSemesterResult(Ptr<SemesterResult> semesterResult);
    void setAmount(int amount);

    const std::string& getReason() const;
    int getAmount() const;
    const Ptr<SemesterResult>& getSemesterResult() const;

private:
    std::string reason_;
    int amount_;

    Ptr<SemesterResult> semesterResult_;
};
