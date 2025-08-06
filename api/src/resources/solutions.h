#pragma once

#include "types.h"
#include "http_exceptions.h"

class WorkResult;

namespace Solutions {
    std::string getSolutionDirectory(const Ptr<WorkResult>& workResult);
}