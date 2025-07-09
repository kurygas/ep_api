#include "atc_service.h"
#include "session.h"
#include "http_exceptions.h"

#include <cpr/cpr.h>

void AtcService::pullPoints(Session& session, const Ptr<SemesterResult>& semesterResult) {
    if (semesterResult->getUser()->getLastAtcUpdate().addDays(1) > Wt::WDateTime::currentDateTime() || 
        !semesterResult->getUser()->getGroup() ||
        semesterResult->getUser()->getAtcName().empty() ||
        semesterResult->getSemester()->getAtcRatio() == 0) {
        return;
    }

    try {
        const auto r = cpr::Get(
            cpr::Url{std::format("https://atcoder.jp/users/{}/history/json", semesterResult->getUser()->getAtcName().toUTF8())},
            cpr::Timeout{100000}
        );

        Wt::Json::Array json;
        Wt::Json::parse(r.text, json);
        std::vector<int> perfomanceVec;
        
        for (const Wt::Json::Object& contest : json) {
            const auto isRated = contest.at("IsRated");
            const auto endTime = parseDate(contest.at("EndTime"));

            if (!isRated || 
                endTime < semesterResult->getSemester()->getStart() || 
                endTime > semesterResult->getSemester()->getEnd()) {
                continue;
            }

            perfomanceVec.emplace_back(contest.at("Performance"));
        }

        std::sort(perfomanceVec.begin(), perfomanceVec.end());
        std::reverse(perfomanceVec.begin(), perfomanceVec.end());

        if (perfomanceVec.size() < 8) {
            return;
        }

        auto points = std::accumulate(perfomanceVec.begin(), perfomanceVec.begin() + 5, 0) / 5;
        points /= semesterResult->getSemester()->getAtcRatio();

        if (!session.exist(&Session::getPoint, semesterResult, "atc_points")) {
            session.add(std::make_unique<Point>("atc_points", points, semesterResult));
        }
        else {
            session.getPoint(semesterResult, "atc_points").modify()->setAmount(points);
        }

        semesterResult->getUser().modify()->setAtcUpdated();
    }
    catch (...) {}
}

Wt::WDateTime AtcService::parseDate(const std::string& str) {
    const auto year = std::stoi(str.substr(0, 4));
    const auto month = std::stoi(str.substr(5, 2));
    const auto day = std::stoi(str.substr(8, 2));
    const auto hour = std::stoi(str.substr(11, 2));
    const auto minute = std:: stoi(str.substr(14, 2));
    return {{year, month, day}, {hour, minute}};
}
