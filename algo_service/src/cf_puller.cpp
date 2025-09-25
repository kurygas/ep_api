#include "cf_puller.h"
#include "utility_functions.h"
#include "async_request.h"

#include <format>
#include <unordered_map>

std::string CfPuller::getSignature(std::string method, const std::map<std::string, std::string>& params) {
    const auto rand = Utility::generateRandomString(6);
    auto data = std::format("{}/{}?{}", rand, std::move(method), Utility::joinParams(params));
    return rand + Utility::sha512(std::format("{}#{}", std::move(data), secret));
}

std::string CfPuller::getUrl(std::string method, std::map<std::string, std::string>& params) {
    params["apiKey"] = key;
    params["time"] = std::to_string(time(nullptr));
    params["apiSig"] = getSignature(method, params);
    return std::format("https://codeforces.com/api/{}?{}", std::move(method), Utility::joinParams(params));
}

cpr::Url CfPuller::getContestPointsUrl(const int contestId, std::string handle, const bool official) {
    std::map<std::string, std::string> params = {
        {"contestId", std::to_string(contestId)},
        {"asManager", "true"},
        {"handles", std::move(handle)}
    };

    if (!official) {
        params["showUnofficial"] = "true";
    }

    return {getUrl("contest.standings", params)};
}

int CfPuller::getContestPoints(cpr::Response r) {
    try {
        auto rows = json::parse(r.text).at("result").at("rows");

        if (rows.empty()) {
            return 0;
        }

        return rows.front().at("points");
    }
    catch (...) {
        return 0;
    }
}

cpr::Url CfPuller::getContestListUrl(std::string cfGroupCode) {
    std::map<std::string, std::string> params = {
        {"gym", "true"},
        {"groupCode", std::move(cfGroupCode)}
    };

    return {getUrl("contest.list", params)};
}

json CfPuller::getContestList(cpr::Response r) {
    try {
        return json::parse(r.text).at("result");
    }
    catch (...) {
        return json::array();
    }
}

cpr::Url CfPuller::getContestTaskCountUrl(const int contestId) {
    std::map<std::string, std::string> params = {
        {"contestId", std::to_string(contestId)},
        {"asManager", "true"}
    };

    return {getUrl("contest.standings", params)};
}

int CfPuller::getContestTaskCount(cpr::Response r) {
    try {
        return json::parse(r.text).at("result").at("problems").size();
    }
    catch (...) {
        return 0;
    }
}

int CfPuller::getFinalPoint(const int officialPoints, int unofficialPoints, const int sumPoints, const int cfMaxPoint) {
    if (sumPoints == 0) {
        return 0;
    }

    unofficialPoints -= officialPoints;
    unofficialPoints /= 2;
    return cfMaxPoint * (officialPoints + unofficialPoints) / sumPoints;
}

Task CfPuller::pullPoints(const User& user, const Semester& semester, const Group& group) {
    if (group.cfGroupCode.empty() ||
        user.cfName.empty()) {
        co_return 0;
    }

    int officialPoints = 0;
    int unofficialPoints = 0;
    int sumPoints = 0;

    try {
        for (const auto& contest : getContestList(co_await GetAsync(getContestListUrl(group.cfGroupCode)))) {
            try {
                const int64_t start = contest.at("startTimeSeconds");
                const int64_t duration = contest.at("durationSeconds");
                const int64_t end = start + duration;
                const auto contestId = contest.at("id");

                if (start < semester.start || end > semester.end) {
                    continue;
                }

                officialPoints += getContestPoints(co_await GetAsync(getContestPointsUrl(contestId, user.cfName, true)));
                unofficialPoints += getContestPoints(co_await GetAsync(getContestPointsUrl(contestId, user.cfName, false)));
                sumPoints += getContestTaskCount(co_await GetAsync(getContestTaskCountUrl(contestId)));
            }
            catch (...) {}
        }
    }
    catch (...) {
        co_return 0;
    }

    co_return getFinalPoint(officialPoints, unofficialPoints, sumPoints, semester.cfMaxPoint);
}
