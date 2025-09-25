#include "service.h"
#include "cf_puller.h"
#include "atc_puller.h"
#include "storage.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

Service::Service()
: timer_(ioService_, boost::asio::chrono::minutes(5))
, messageQueue_(ioService_) {
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

void Service::spawnPullers() {
    std::queue<Task> tasks;

    for (const auto& [userId, user] : Storage<User>::instance()) {
        for (const auto& [semesterId, semester] : Storage<Semester>::instance()) {
            if (user.groupId != semester.groupId) {
                continue;
            }

            auto& promise = tasks.emplace(CfPuller::pullPoints(user, semester, Storage<Group>::instance().at(user.groupId))).promise();
            promise.setUserId(userId);
            promise.setSemesterId(semesterId);
            promise.setType(TaskType::Cf);
            
            promise = tasks.emplace(AtcPuller::pullPoints(user, semester)).promise();
            promise.setUserId(userId);
            promise.setSemesterId(semesterId);
            promise.setType(TaskType::Atc);
        }
    }

    while (!tasks.empty()) {
        auto task = std::move(tasks.front());
        tasks.pop();
        
        if (!task.done()) {
            tasks.emplace(std::move(task));
            continue;
        }

        const auto& promise = task.promise();
        json message;
        message["user_id"] = promise.getUserId();
        message["semester_id"] = promise.getSemesterId();
        
        if (promise.getType() == TaskType::Cf) {
            message["cf_point"] = promise.getPoint();
        }
        else if (promise.getType() == TaskType::Atc) {
            message["atc_point"] = promise.getPoint();
        }

        messageQueue_.publish("algo_result", message);
        task.destroy();
    }
}

void Service::run() {
    ioService_.run();
}
