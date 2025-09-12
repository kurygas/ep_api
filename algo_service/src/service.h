#pragma once

#include <boost/asio.hpp>

#include "message_queue.h"

class Service {
public:
    Service();
    void run();

private:
    boost::asio::io_service ioContext_;
    boost::asio::steady_timer timer_;
    MessageQueue messageQueue;
};