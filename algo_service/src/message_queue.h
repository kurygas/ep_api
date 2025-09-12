#pragma once

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MessageQueue {
public:
    MessageQueue(boost::asio::io_service& ioService);

    static json parseMessage(const AMQP::Message& data);
    void publish(const std::string routingKey, const json& message);

    void configureAlgoDataQueue();
    void configureAlgoResultQueue();

private:
    AMQP::LibBoostAsioHandler handler_;
    AMQP::TcpConnection connection_;
    AMQP::TcpChannel channel_;
};
