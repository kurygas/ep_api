#include "service.h"
#include "cf_puller.h"
#include "atc_puller.h"
#include "storage.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

Service::Service()
: timer_(ioService_, boost::asio::chrono::minutes(5))
, messageQueue(ioService_) {
    pullConfig();
    runTimer();
}

void Service::runTimer() {
    timer_.async_wait([this](const boost::system::error_code& code) {
        if (!code) {
            spawnPullers();
            runTimer();
        }
    });
}

void Service::pullConfig() const {
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini("config.ini", pt);
    CfPuller::key = pt.get<std::string>("cf.key");
    CfPuller::secret = pt.get<std::string>("cf.secret");
}

void Service::spawnPullers() const {
    std::queue<Task> tasks;

    for (const auto& [userId, user] : Storage<User>::instance()) {
        for (const auto& [semesterId, semester] : Storage<Semester>::instance()) {
            if (user.groupId != semester.groupId) {
                continue;
            }

            tasks.emplace(CfPuller::pullPoints(user, semester, Storage<Group>::instance().at(user.groupId)));
            tasks.emplace(AtcPuller::pullPoints(user, semester));
        }
    }

    while (!tasks.empty()) {
        auto task = std::move(tasks.front());
        tasks.pop();
        
        if (!task.done()) {
            tasks.emplace(std::move(task));
            continue;
        }

        MessageQueue::
    }
}

void Service::run() {
    ioService_.run();
}
