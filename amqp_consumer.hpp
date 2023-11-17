// AMQPConsumer.hpp
#pragma once

#include <rabbitmq-c/amqp.h>

class AMQPConsumer {
public:
    static void run(amqp_connection_state_t conn);

private:
    static uint64_t nowMicroseconds();
};
