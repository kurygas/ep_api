#include "cf_service.h"
#include "user.h"
#include "session.h"
#include "http_exceptions.h"
#include "session.h"

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
                    semesterResult->getUser()->getCfName(),
                    true
                );

                unofficalPoints += getContestPoints(
                    contestId, 
                    semesterResult->getUser()->getCfName(),
                    false
                );

                sumPoints += getContestTaskCount(contestId);
            }
            catch (...) {}
        }

        if (sumPoints == 0) {
            return;
        }

        unofficalPoints -= officialPoints;
        unofficalPoints /= 2;
        const auto finalPoint = semesterResult->getSemester()->getCfMaxPoint() * (officialPoints + unofficalPoints) / sumPoints;

        if (!session.exist(&Session::getPoint, semesterResult, "cf_points")) {
            session.add(std::make_unique<Point>("cf_point", finalPoint, semesterResult));
        }
        else {
            session.getPoint(semesterResult, "cf_points").modify()->setAmount(finalPoint);
        }

        semesterResult->getUser().modify()->setCfUpdated();
    }
    catch (...) {}
}

Wt::Json::Array CfService::getContestList(const Ptr<SemesterResult>& semesterResult) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"gym", "true"},
        {"groupCode", semesterResult->getUser()->getGroup()->getCfGroupCode()}
    };

    const auto r = cpr::Get(
        cpr::Url{getUrl("contest.list", params)},
        cpr::Timeout{5000}
    );

    Wt::Json::Object json;
    Wt::Json::parse(r.text, json);
    return json.at("result");
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
