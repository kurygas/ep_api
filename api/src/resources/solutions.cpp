#include "solutions.h"
#include "work_result.h"

std::string Solutions::getSolutionDirectory(const Ptr<WorkResult>& workResult) {
    if (workResult->getFilename().empty()) {
        throw BadRequestException("No solution file");
    }

    return std::format("../solutions/{}/{}", std::to_string(workResult.id()), workResult->getFilename());
}
