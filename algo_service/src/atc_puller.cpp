#include "atc_puller.h"
#include "async_request.h"

#include <vector>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int64_t AtcPuller::parseDate(const std::string& str) {
    const auto year = std::stoi(str.substr(0, 4));
    const auto month = std::stoi(str.substr(5, 2));
    const auto day = std::stoi(str.substr(8, 2));
    const auto hour = std::stoi(str.substr(11, 2));
    const auto minute = std::stoi(str.substr(14, 2));

    auto date = std::chrono::year_month_day(
        std::chrono::year(year),
        std::chrono::month(month),
        std::chrono::day(day)
    );

    auto timePoint = std::chrono::sys_days(date) + std::chrono::hours(hour) + std::chrono::minutes(minute);
    return std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();
}

cpr::Url AtcPuller::getUrl(std::string atcName) {
    return {std::format("https://atcoder.jp/users/{}/history/json", std::move(atcName))};
}

int AtcPuller::getFinalPoint(const std::vector<int>& perfomanceVector, const int atcRatio) {
    if (perfomanceVector.size() < 8) {
        return 0;
    }

    std::sort(perfomanceVector.begin(), perfomanceVector.end(), std::greater<int>());
    return std::accumulate(perfomanceVector.begin(), perfomanceVector.begin() + 5, 0) / 5 / atcRatio;
}

Task AtcPuller::pullPoints(const User& user, const Semester& semester) {
    if (user.atcName.empty() || semester.atcRatio == 0) {
        co_return 0;
    }

    try {
        std::vector<int> perfomanceVector;

        for (const auto& contest : json::parse((co_await GetAsync(getUrl(user.atcName))).text)) {
            try {
                const bool isRated = contest.at("IsRated");
                const int64_t endTime = parseDate(contest.at("EndTime"));

                if (!isRated || endTime < semester.start || endTime > semester.finish) {
                    continue;
                }

                perfomanceVector.emplace_back(contest.at("Perfomance"));
            }
            catch (...) {}
        }

        co_return getFinalPoint(perfomanceVector, semester.atcRatio);
    }
    catch (...) {
        co_return 0;
    }
}
