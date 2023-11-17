// amqp_helper.cpp
#include "amqp_helper.hpp"
#include <iostream>

void AMQPHelper::dieOnAMQPError(amqp_rpc_reply_t x, const char* context) {
    if (x.reply_type == AMQP_RESPONSE_NORMAL) {
        return;
    }

    std::cerr << context << ": " << amqp_error_string2(x.library_error) << std::endl;
    exit(1);
}

void AMQPHelper::die(const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void AMQPHelper::dieOnError(int x, const char* context) {
    if (x < 0) {
        fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x));
        exit(1);
    }
}

amqp_bytes_t AMQPHelper::declareQueue(amqp_connection_state_t conn, int channel) {
    amqp_queue_declare_ok_t* result =
        amqp_queue_declare(conn, channel, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);
    dieOnAMQPError(amqp_get_rpc_reply(conn), "Declaring queue");
    return amqp_bytes_malloc_dup(result->queue);
}

void AMQPHelper::bindQueue(
    amqp_connection_state_t conn, int channel, const amqp_bytes_t& queue, const char* exchange, const char* bindingkey) {
    amqp_queue_bind(conn, channel, queue, amqp_cstring_bytes(exchange),
                    amqp_cstring_bytes(bindingkey), amqp_empty_table);
    dieOnAMQPError(amqp_get_rpc_reply(conn), "Binding queue");
}

void AMQPHelper::consumeMessages(amqp_connection_state_t conn, int channel, const amqp_bytes_t& queue) {
    amqp_basic_consume(conn, channel, queue, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    dieOnAMQPError(amqp_get_rpc_reply(conn), "Consuming");
}

void AMQPHelper::closeConnection(amqp_connection_state_t conn) {
    dieOnAMQPError(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
    dieOnAMQPError(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
}

void AMQPHelper::destroyConnection(amqp_connection_state_t conn) {
    dieOnError(amqp_destroy_connection(conn), "Ending connection");
}
