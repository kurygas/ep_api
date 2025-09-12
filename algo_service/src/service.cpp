#include "service.h"

Service::Service()
: timer_(ioContext_, boost::asio::chrono::minutes(5)) {
    timer_.async_wait([](const boost::system::error_code& code) {
        if (!code) {
            1;
        }
    });
}

void Service::run() {
    ioContext_.run();
}
