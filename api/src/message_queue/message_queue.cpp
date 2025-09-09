#include "message_queue.h"
#include "session.h"

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
    Wt::Json::Object message;
    Wt::Json::Array groups;
    Wt::Json::Array users;
    Wt::Json::Array semesters;

    for (const auto& group : session.getAll<Group>()) {
        groups.emplace_back(static_cast<Wt::Json::Object>(*group));

        for (const auto& user : group->getUsers()) {
            users.emplace_back(*user);
        }

        for (const auto& semester : group->getSemesters()) {
            semesters.emplace_back(*semester);
        }
    }

    message[Str::groupList] = std::move(groups);
    message[Str::userList] = std::move(users);
    message[Str::semesterList] = std::move(semesters);
    publish("algo_data", message);
}

void MessageQueue::processAlgoResult(const Wt::Json::Object& message) {
    Session session;
    const Wt::Dbo::Transaction tr(session);
    const auto semesters = static_cast<Wt::Json::Array>(message.at("data"));
    for (const Wt::Json::Object& semesterData : semesters) {
        const auto semester = session.load<Semester>(semesterData.at(Str::semesterId));
        const auto users = static_cast<Wt::Json::Array>(semesterData.at(Str::userList));
        
        for (const Wt::Json::Object& userData : users) {
            const auto user = session.load<User>(userData.at(Str::userId));
            const auto cfPoint = static_cast<int>(userData.at("cf_point"));
            const auto atcPoint = static_cast<int>(userData.at("atc_point"));
            const auto semesterResult = session.getSemesterResult(semester, user);
            //ToDo: create semester_result, cf_point, atc_point after creating semester
            const auto cfPointObject = session.getPoint(semesterResult, "cf_point");
            const auto atcPointObject = session.getPoint(semesterResult, "atc_point");
            cfPointObject.modify()->setAmount(cfPoint);
            atcPointObject.modify()->setAmount(atcPoint);
        }
    }
}
