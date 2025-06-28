#pragma once

#include <Wt/WString.h>
#include <Wt/WDateTime.h>

namespace Validator {
    bool isSemesterNumberValid(int semesterNumber);
    bool isRussianString(const Wt::WString& str);
    bool isPasswordValid(const Wt::WString& password);
    bool isTimeSegmentValid(const Wt::WDateTime& start, const Wt::WDateTime& end);
}
