#include "cf_service.h"
#include "user.h"
#include "session.h"
#include "http_exceptions.h"
#include "random_functions.h"
#include "crypto.h"
#include "utility_functions.h"
#include "session.h"

#include <cpr/cpr.h>

void CfService::pullPoints(Session& session, const Ptr<SemesterResult>& semesterResult) {
    if (semesterResult->getUser()->getLastCfUpdate().addDays(1) > Wt::WDateTime::currentDateTime() || 
        !semesterResult->getUser()->getGroup() ||
        semesterResult->getUser()->getGroup()->getCfGroupCode().empty() ||
        semesterResult->getUser()->getCfName().empty()) {
        return;
    }

    try {
        int officialPoints = 0;
        int unofficalPoints = 0;
        int sumPoints = 0;

        for (const Wt::Json::Object& contest : getContestList(semesterResult)) {
            try {
                const auto start = Wt::WDateTime::fromTime_t(contest.at("startTimeSeconds"));
                const auto duration = contest.at("durationSeconds");
                const auto end = start.addSecs(duration);
                const auto contestId = contest.at("contest_id");

                if (start < semesterResult->getSemester()->getStart() || end > semesterResult->getSemester()->getEnd()) {
                    continue;
                }

                const auto officalRanklistRow = getRanklistRow(
                    contestId, 
                    semesterResult->getUser()->getCfName().toUTF8(),
                    true
                );

                const auto unofficalRanklistRow = getRanklistRow(
                    contestId, 
                    semesterResult->getUser()->getCfName().toUTF8(),
                    false
                );

                const auto officialDiff = static_cast<double>(officalRanklistRow.at("points"));
                auto unofficialDiff = static_cast<double>(unofficalRanklistRow.at("points"));
                unofficialDiff -= officialDiff;
                officialPoints += officialDiff;
                unofficalPoints += unofficialDiff;
                sumPoints += static_cast<Wt::Json::Array>(contest.at("problemResults")).size();
            }
            catch (...) {}
        }

        unofficalPoints /= 2;
        // ToDo: final formula and move cf max points into the semester
        
        session.getPoint(semesterResult, "cf_points")
    }
    catch (...) {}
}

std::string CfService::getUrl(const std::string& method, std::vector<std::pair<std::string, std::string>>& params) {
    params.emplace_back("apiKey", key);
    params.emplace_back("time", std::to_string(time(nullptr)));
    params.emplace_back("apiSig", getSignature(method, params));
    return std::format("https://codeforces.com/api/{}?{}", method, Utility::joinParams(params));
}

std::string CfService::getSignature(const std::string& method, std::vector<std::pair<std::string, std::string>>& params) {
    std::sort(params.begin(), params.end());
    const auto rand = Random::generateRandomString(6);
    const auto data = std::format("{}/{}?{}", rand, method, Utility::joinParams(params));
    return rand + Crypto::sha512(std::format("{}#{}", data, secret));
}

Wt::Json::Array CfService::getContestList(const Ptr<SemesterResult>& semesterResult) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"gym", "true"},
        {"groupCode", semesterResult->getUser()->getGroup()->getCfGroupCode().toUTF8()}
    };

    const auto r = cpr::Get(
        cpr::Url{getUrl("contest.list", params)},
        cpr::Timeout{5000}
    );

    Wt::Json::Object json;
    Wt::Json::parse(r.text, json);
    return json.at("result");
}

Wt::Json::Object CfService::getRanklistRow(const int contestId, const std::string& handle, const bool official) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"contestId", std::to_string(contestId)},
        {"asManager", "true"},
        {"handles", handle},
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
    return static_cast<Wt::Json::Array>(json.at("result")).front();
}
