#pragma once

#include <Wt/WString.h>

#include "types.h"

class SemesterResult;

namespace AtcService {
    void pullPoints(const Ptr<SemesterResult>& semesterResult);
}
