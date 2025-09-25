#pragma once

#include <Wt/WServer.h>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

#include "types.h"
#include "json.h"

class User;
class Semester;

class MessageQueue {
public:
    static void createInstance(Wt::WServer& server);
    static MessageQueue& getInstance();

    static Wt::Json::Object parseMessage(const AMQP::Message& data);
    
    template<typename T>
    void publish(const std::string& routingKey, const T& message) {
        channel_.publish("", routingKey, Wt::Json::serialize(message));
    }

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
    void processAlgoResult(const Wt::Json::Array& data);

    AMQP::LibBoostAsioHandler handler_;
    AMQP::TcpConnection connection_;
    AMQP::TcpChannel channel_;

    inline static std::unique_ptr<MessageQueue> instance_ = nullptr;
};
