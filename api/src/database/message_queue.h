#pragma once

#include <Wt/WServer.h>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

#include "types.h"

class User;
class Semester;

class MessageQueue {
public:
    static void createInstance(Wt::WServer& server);
    static const std::unique_ptr<MessageQueue>& getInstance();

    static Wt::Json::Object parseMessage(const AMQP::Message& data);
    static Wt::Json::Object getUserData(const Ptr<User>& user);
    static Wt::Json::Object getSemesterData(const Ptr<Semester>& semester);

    MessageQueue() = delete;
    MessageQueue(const MessageQueue&) = delete;
    MessageQueue(MessageQueue&&) = delete;
    MessageQueue& operator=(const MessageQueue&) = delete;
    MessageQueue& operator=(MessageQueue&&) = delete;

private:
    MessageQueue(Wt::WServer& server);
    ~MessageQueue() = default;

    void configureAlgoDataQueue();
    void configureAlgoResultQueue();
    void processAlgoRequest();
    void processAlgoResult(const Wt::Json::Object& message);

    AMQP::LibBoostAsioHandler handler_;
    AMQP::TcpConnection connection_;
    AMQP::TcpChannel channel_;

    inline static std::unique_ptr<MessageQueue> instance_ = nullptr;
};