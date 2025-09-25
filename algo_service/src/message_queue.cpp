#include "message_queue.h"
#include "storage.h"
#include "user.h"
#include "group.h"
#include "semester.h"

MessageQueue::MessageQueue(boost::asio::io_service& ioService)
: handler_(ioService)
, connection_(&handler_, AMQP::Address("amqp", 5672, AMQP::Login("guest", "guest"), "/"))
, channel_(&connection_) {
    configureAlgoDataQueue();
    configureAlgoResultQueue();
}

void MessageQueue::publish(const std::string& routingKey, const json& message) {
    channel_.publish("", routingKey, message.dump());
}

void MessageQueue::configureAlgoDataQueue() {
    channel_.declareQueue("algo_data");
    channel_.consume("algo_data").onReceived([this](const AMQP::Message& data, uint64_t tag, bool) {
        auto message = json::parse(std::string(data.body(), data.bodySize()));
        
        if (message.contains("user_id")) {
            if (message.contains("delete")) {
                deleteUser(message);
            }
            else {
                updateUser(message);
            }
        }
        else if (message.contains("group_id")) {
            if (message.contains("delete")) {
                deleteGroup(message);
            }
            else {
                updateGroup(message);
            }
        }
        else if (message.contains("semester_id")) {
            if (message.contains("delete")) {
                deleteSemester(message);
            }
            else {
                updateSemester(message);
            }
        }
    });
}

void MessageQueue::configureAlgoResultQueue() {
    channel_.declareQueue("algo_result");
}

void MessageQueue::updateUser(const json& message) const {
    auto& storage = Storage<User>::instance();
    auto userId = static_cast<int>(message.at("user_id"));

    if (!message.contains("group_id")) {
        if (storage.contains(userId)) {
            storage.erase(userId);
        }
        return;
    }

    auto& user = storage.at(userId);
    user.atcName = message.at("atc_name");
    user.cfName = message.at("cf_name");
    user.groupId = message.at("group_id");
}

void MessageQueue::deleteUser(const json& message) const {
    auto& storage = Storage<User>::instance();
    auto userId = static_cast<int>(message.at("user_id"));

    if (storage.contains(userId)) {
        storage.erase(userId);
    }
}

void MessageQueue::updateSemester(const json& message) const {
    auto& storage = Storage<Semester>::instance();
    auto semesterId = static_cast<int>(message.at("semester_id"));

    if (!message.contains("group_id")) {
        if (storage.contains(semesterId)) {
            storage.erase(semesterId);
        }
        return;
    }

    auto& semester = storage.at(semesterId);
    semester.start = message.at("start");
    semester.end = message.at("end");
    semester.groupId = message.at("group_id");

    if (message.contains("atc_ratio")) {
        semester.atcRatio = message.at("atc_ratio");
    }

    if (message.contains("cf_max_point")) {
        semester.cfMaxPoint = message.at("cf_max_point");
    }
}

void MessageQueue::deleteSemester(const json& message) const {
    auto& storage = Storage<Semester>::instance();
    auto semesterId = static_cast<int>(message.at("semester_id"));

    if (storage.contains(semesterId)) {
        storage.erase(semesterId);
    }
}

void MessageQueue::updateGroup(const json& message) const {
    Storage<Group>::instance().at(message.at("group_id")).cfGroupCode = message.at("cf_group_code");
}

void MessageQueue::deleteGroup(const json& message) const {
    auto groupId = static_cast<int>(message.at("group_id"));

    if (Storage<Group>::instance().contains(groupId)) {
        Storage<Group>::instance().erase(groupId);
    }

    for (const auto& [userId, user] : Storage<User>::instance()) {
        if (groupId == user.groupId) {
            Storage<User>::instance().erase(userId);
        }
    }

    for (const auto& [semesterId, semester] : Storage<Semester>::instance()) {
        if (groupId == semester.groupId) {
            Storage<Semester>::instance().erase(semesterId);
        }
    }
}
