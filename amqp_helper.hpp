// amqp_helper.hpp
#pragma once

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

class AMQPHelper {
public:
    static amqp_bytes_t declareQueue(amqp_connection_state_t conn, int channel);
    static void bindQueue(
        amqp_connection_state_t conn, int channel, const amqp_bytes_t& queue, const char* exchange, const char* bindingkey);
    static void consumeMessages(amqp_connection_state_t conn, int channel, const amqp_bytes_t& queue);
    static void closeConnection(amqp_connection_state_t conn);
    static void destroyConnection(amqp_connection_state_t conn);
    static void die(const char* message);

private:
    static void dieOnAMQPError(amqp_rpc_reply_t x, const char* context);
    static void dieOnError(int x, const char* context);
};
