#include "message_queue.h"

MessageQueue::MessageQueue(boost::asio::io_service& ioService)
: handler_(ioService)
, connection_(&handler_, AMQP::Address("amqp", 5672, AMQP::Login("guest", "guest"), "/"))
, channel_(&connection_) {
    configureAlgoDataQueue();
    configureAlgoResultQueue();
}

void MessageQueue::configureAlgoDataQueue() {
    channel_.declareQueue("algo_data");
    channel_.consume("algo_data").onReceived([this](const AMQP::Message& data, uint64_t tag, bool) {
        const auto message = parseMessage(data);
        
        if (message.contains("user_id")) {
            
        }
        if (message.contains("user_list")) {

        }
        if (message.contains("group_id")) {

        }
        if (message.contains("group_list")) {

        }
        if (message.contains("semester_id")) {

        }
        if (message.contains("semester_list")) {

        }
    });
}