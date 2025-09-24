#pragma once

#include <cstdint>
#include <cpr/cpr.h>

#include "coroutine_task.h"
#include "user.h"
#include "semester.h"

namespace AtcPuller {
    int64_t parseDate(const std::string& str);
    cpr::Url getUrl(std::string atcName);
    int getFinalPoint(const std::vector<int>& perfomanceVector, int atcRatio);
    Task pullPoints(const User& user, const Semester& semester);
}
