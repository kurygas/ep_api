#pragma once

#include <boost/asio.hpp>

#include "message_queue.h"

class Service {
public:
    Service();
    void run();
    
    void pullConfig() const;
    void spawnPullers();
    void runTimer();

private:
    boost::asio::io_service ioService_;
    boost::asio::steady_timer timer_;
    MessageQueue messageQueue_;
};