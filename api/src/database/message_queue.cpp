#include "message_queue.h"
#include "session.h"
#include "json.h"

#include <Wt/WIOService.h>

void MessageQueue::createInstance(Wt::WServer& server) {
    if (instance_) {
        throw std::runtime_error("Already created");
    }

    instance_ = std::make_unique<MessageQueue>(server);
}

const std::unique_ptr<MessageQueue>& MessageQueue::getInstance() {
    if (!instance_) {
        throw std::runtime_error("Not created yet");
    }

    return instance_;
}

Wt::Json::Object MessageQueue::parseMessage(const AMQP::Message& data) {
    Wt::Json::Object message;
    Wt::Json::parse({data.body(), data.bodySize()}, message);
    return message;
}

Wt::Json::Object MessageQueue::getUserData(const Ptr<User>& user) {
    Wt::Json::Object userData;
    userData[Str::userId] = user.id();

    if (!user->getCfName().empty()) {
        userData[Str::cfName] = user->getCfName().c_str();
    }

    if (!user->getAtcName().empty()) {
        userData[Str::atcName] = user->getAtcName().c_str();
    }

    return userData;
}

Wt::Json::Object MessageQueue::getSemesterData(const Ptr<Semester>& semester) {
    Wt::Json::Object semesterData;
    semesterData[Str::semesterId] = semester.id();
    semesterData[Str::start] = semester->getStart().toTime_t();
    semesterData[Str::end] = semester->getEnd().toTime_t();
    return semesterData;
}

MessageQueue::MessageQueue(Wt::WServer& server)
: handler_(server.ioService())
, connection_(&handler_, AMQP::Address("amqp", 5672, AMQP::Login("guest", "guest"), "/"))
, channel_(&connection_) {
    configureAlgoDataQueue();
    configureAlgoResultQueue();
}

void MessageQueue::configureAlgoDataQueue() {
    channel_.declareQueue("algo_data");
}

void MessageQueue::configureAlgoResultQueue() {
    channel_.declareQueue("algo_result");
    channel_.consume("algo_result").onReceived([this](const AMQP::Message& data, uint64_t tag, bool) {
        const auto message = parseMessage(data);
        const auto type = static_cast<std::string>(message.at("type"));

        if (type == "request") {
            processAlgoRequest();
        }
        else if (type == "result") {
            processAlgoResult(message);
        }
    });
}

void MessageQueue::processAlgoRequest() {
    Session session;
    const Wt::Dbo::Transaction tr(session);
    Wt::Json::Array groups;

    for (const auto& group : session.getAll<Group>()) {
        Wt::Json::Object groupData;
        Wt::Json::Array users;
        Wt::Json::Array semesters;
        groupData[Str::cfGroupCode] = group->getCfGroupCode().c_str();

        for (const auto& user : group->getUsers()) {
            users.emplace_back(getUserData(user));
        }

        for (const auto& semester : group->getSemesters()) {
            if (semester->getSubject() != Subject::Type::Algo) {
                continue;
            }

            semesters.emplace_back(getSemesterData(semester));
        }

        groupData["users"] = std::move(users);
        groupData["semesters"] = std::move(semesters);
        groups.emplace_back(std::move(groupData));
    }

    channel_.publish("", "algo_data", Wt::Json::serialize(groups));
}

void MessageQueue::processAlgoResult(const Wt::Json::Object& message) {

}
