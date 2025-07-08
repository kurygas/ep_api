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
                const auto contestId = contest.at("id");

                if (start < semesterResult->getSemester()->getStart() || end > semesterResult->getSemester()->getEnd()) {
                    continue;
                }

                officialPoints += getContestPoints(
                    contestId, 
                    semesterResult->getUser()->getCfName().toUTF8(),
                    true
                );

                unofficalPoints += getContestPoints(
                    contestId, 
                    semesterResult->getUser()->getCfName().toUTF8(),
                    false
                );

                sumPoints += getContestTaskCount(contestId);
            }
            catch (...) {}
        }

        if (sumPoints == 0) {
            return;
        }

        unofficalPoints /= 2;
        const auto finalPoint = semesterResult->getSemester()->getCfMaxPoint() * (officialPoints + unofficalPoints) / sumPoints;

        if (!session.exist(&Session::getPoint, semesterResult, "cf_points")) {
            session.add(std::make_unique<Point>("cf_point", finalPoint, semesterResult));
        }
        else {
            session.getPoint(semesterResult, "cf_points").modify()->setAmount(finalPoint);
        }
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

int CfService::getContestPoints(const int contestId, const std::string& handle, const bool official) {
    try {
        std::vector<std::pair<std::string, std::string>> params = {
            {"contestId", std::to_string(contestId)},
            {"asManager", "true"},
            {"handles", handle},
        };

        auto officialPoints = 0; 

        if (!official) {
            params.emplace_back("showUnofficial", "true");
            officialPoints = getContestPoints(contestId, handle, true);
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
        return static_cast<int>(ranklistRow.at("points")) - officialPoints;
    }
    catch (...) {
        return 0;
    }
}

int CfService::getContestTaskCount(const int contestId) {
    try {
        std::vector<std::pair<std::string, std::string>> params = {
            {"contestId", std::to_string(contestId)},
            {"asManager", "true"}
        };

        const auto r = cpr::Get(
            cpr::Url{getUrl("contest.standings", params)},
            cpr::Timeout{5000}
        );

        Wt::Json::Object json;
        Wt::Json::parse(r.text, json);
        auto result = static_cast<Wt::Json::Object>(json.at("result"));
        auto problems = static_cast<Wt::Json::Array>(result.at("problems"));
        return problems.size();
    }
    catch (...) {
        return 0;
    }
}
