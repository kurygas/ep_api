#pragma once

#include "types.h"
#include "json.h"

class SemesterResult;
class Session;

namespace CfService {
    inline std::string key;
    inline std::string secret;

    void pullPoints(Session& session, const Ptr<SemesterResult>& semesterResult);
    std::string getUrl(const std::string& method, std::vector<std::pair<std::string, std::string>>& params);
    std::string getSignature(const std::string& method, std::vector<std::pair<std::string, std::string>>& params);
    Wt::Json::Array getContestList(const Ptr<SemesterResult>& semesterResult);
    int getContestPoints(int contestId, const std::string& handle, bool official);
    int getContestTaskCount(int contestId);
}
