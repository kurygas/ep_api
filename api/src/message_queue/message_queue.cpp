#include "message_queue.h"
#include "session.h"

#include <Wt/WIOService.h>

void MessageQueue::createInstance(Wt::WServer& server) {
    if (instance_) {
        throw std::runtime_error("Already created");
    }

    instance_ = std::make_unique<MessageQueue>(server);
}

MessageQueue& MessageQueue::getInstance() {
    if (!instance_) {
        throw std::runtime_error("Not created yet");
    }

    return *instance_;
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
            processAlgoResult(message.at("data"));
        }
    });
}

void MessageQueue::processAlgoRequest() {
    Session session;
    const Wt::Dbo::Transaction tr(session);

    for (const auto& group : session.getAll<Group>()) {
        auto message = static_cast<Wt::Json::Object>(*group);
        message[Str::groupId] = group.id();
        publish("algo_data", message);

        for (const auto& user : group->getUsers()) {
            message = static_cast<Wt::Json::Object>(*user);
            message[Str::userId] = user.id();
            publish("algo_data", message);
        }

        for (const auto& semester : group->getSemesters()) {
            message = static_cast<Wt::Json::Object>(*semester);
            message[Str::semesterId] = semester.id();
            publish("algo_data", message);
        }
    }
}

void MessageQueue::processAlgoResult(const Wt::Json::Array& data) {
    Session session;
    const Wt::Dbo::Transaction tr(session);

    for (const Wt::Json::Object& result : data) {
        const auto semester = session.load<Semester>(result.at(Str::semesterId));
        const auto user = session.load<User>(result.at(Str::userId));
        const auto semesterResult = session.getSemesterResult(semester, user);

        if (result.contains("cf_point")) {
            const auto cfPointObject = session.getPoint(semesterResult, "cf_point");
            cfPointObject.modify()->setAmount(result.at("cf_point"));
        }
        else if (result.contains("atc_point")) {
            const auto atcPointObject = session.getPoint(semesterResult, "atc_point");
            atcPointObject.modify()->setAmount(result.at("atc_point"));
        }
    }
}
