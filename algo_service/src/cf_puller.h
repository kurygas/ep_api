#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "user.h"
#include "semester.h"
#include "group.h"
#include "coroutine_task.h"

using json = nlohmann::json;

namespace CfPuller {
    // ToDo: get this from config
    inline std::string key;
    inline std::string secret;
    
    std::string getSignature(std::string method, const std::map<std::string, std::string>& params);
    std::string getUrl(std::string method, const std::map<std::string, std::string>& params);
    
    cpr::Url getContestPointsUrl(int contestId, std::string handle, bool official);
    int getContestPoints(cpr::Response r);

    cpr::Url getContestListUrl(std::string cfGroupCode);
    json getContestList(cpr::Response r);

    cpr::Url getContestTaskCountUrl(int contestId);
    int getContestTaskCount(cpr::Response r);

    Task pullPoints(const User& user, const Semester& semester, const Group& group);
}
