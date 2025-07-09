#pragma once

#include <Wt/WDateTime.h>

#include "types.h"

class Session;
class SemesterResult;

namespace AtcService {
    void pullPoints(Session& session, const Ptr<SemesterResult>& semesterResult);
    Wt::WDateTime parseDate(const std::string& str);
}
