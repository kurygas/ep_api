#pragma once

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MessageQueue {
public:
    MessageQueue(boost::asio::io_service& ioService);

    void publish(const std::string& routingKey, const json& message);

private:
    void configureAlgoDataQueue();
    void configureAlgoResultQueue();

    void updateUser(const json& message) const;
    void deleteUser(const json& message) const;

    void updateSemester(const json& message) const;
    void deleteSemester(const json& message) const;

    void updateGroup(const json& message) const;
    void deleteGroup(const json& message) const;

    AMQP::LibBoostAsioHandler handler_;
    AMQP::TcpConnection connection_;
    AMQP::TcpChannel channel_;
};
