#include "service.h"

Service::Service()
: timer_(ioService_, boost::asio::chrono::minutes(5))
, messageQueue(ioService_) {
    timer_.async_wait([](const boost::system::error_code& code) {
        if (!code) {
            1;
        }
    });
}

void Service::run() {
    ioService_.run();
}
