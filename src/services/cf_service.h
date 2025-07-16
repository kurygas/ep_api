#pragma once

#include "types.h"
#include "json.h"
#include "crypto.h"
#include "random_functions.h"
#include "utility_functions.h"

#include <cpr/cpr.h>

class SemesterResult;
class Session;

namespace CfService {
    inline std::string key;
    inline std::string secret;

    template<typename T>
    std::string getSignature(T&& method, std::vector<std::pair<std::string, std::string>>& params) {
        std::sort(params.begin(), params.end());
        const auto rand = Random::generateRandomString(6);
        auto data = std::format("{}/{}?{}", rand, std::forward<T>(method), Utility::joinParams(params));
        return rand + Crypto::sha512(std::format("{}#{}", std::move(data), secret));
    }

    template<typename T>
    std::string getUrl(T&& method, std::vector<std::pair<std::string, std::string>>& params) {
        params.emplace_back("apiKey", key);
        params.emplace_back("time", std::to_string(time(nullptr)));
        params.emplace_back("apiSig", getSignature(method, params));
        return std::format("https://codeforces.com/api/{}?{}", std::forward<T>(method), Utility::joinParams(params));
    }

    template<typename T>
    int getContestPoints(int contestId, T&& handle, bool official) {
        try {
            std::vector<std::pair<std::string, std::string>> params = {
                {"contestId", std::to_string(contestId)},
                {"asManager", "true"},
                {"handles", std::forward<T>(handle)},
            };

            if (!official) {
                params.emplace_back("showUnofficial", "true");
            }

            const auto r = cpr::Get(
                cpr::Url{getUrl("contest.standings", params)},
                cpr::Timeout{5000}
            );

            Wt::Json::Object json;
            Wt::Json::parse(r.text, json);
            auto result = static_cast<Wt::Json::Object>(json.at("result"));
            auto rows = static_cast<Wt::Json::Array>(result.at("rows"));

            if (rows.empty()) {
                return 0;
            }

            auto ranklistRow = static_cast<Wt::Json::Object>(rows.front());
            return static_cast<int>(ranklistRow.at("points"));
        }
        catch (...) {
            return 0;
        }
    }

    void pullPoints(Session& session, const Ptr<SemesterResult>& semesterResult);
    Wt::Json::Array getContestList(const Ptr<SemesterResult>& semesterResult);
    int getContestTaskCount(int contestId);
}
