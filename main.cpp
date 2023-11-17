// main.cpp
#include "main.hpp"
#include <iostream>

int main(int argc, const char* argv[]) {
    char const* hostname;
    int port, status;
    char const* exchange;
    char const* bindingkey;
    char const* user;
    char const* password;
    amqp_socket_t* socket = NULL;
    amqp_connection_state_t conn;
    amqp_bytes_t queuename;

    if (argc < 3) {
        fprintf(stderr, "Usage: amqp_consumer host port\n");
        return 1;
    }

    hostname = argv[1];
    port = atoi(argv[2]);
    exchange = argv[3];
    bindingkey = argv[4];

    const char* amqpUser = std::getenv("AMQP_USER");
    const char* amqpPass = std::getenv("AMQP_PASS");
    const char* amqpVhost = std::getenv("AMQP_VHOST");

    if (amqpUser == nullptr || amqpPass == nullptr || amqpVhost == nullptr) {
        std::cerr << "Error: Missing one or more required environment variables." << std::endl;
        return 1;
    }

    std::cout << "AMQP_USER: " << amqpUser << std::endl;
    std::cout << "AMQP_PASS: " << amqpPass << std::endl;
    std::cout << "AMQP_VHOST: " << amqpVhost << std::endl;

    std::cout << "hostname: " << hostname << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "exchange: " << exchange << std::endl;
    std::cout << "bindingkey: " << bindingkey << std::endl;

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);

    if (!socket) {
        AMQPHelper::die("creating TCP socket");
    }

    status = amqp_socket_open(socket, hostname, port);
    // print status
    std::cout << "Status of amqp_socket_open: " << status << std::endl;

    if (status != AMQP_STATUS_OK) {
        AMQPHelper::die("opening TCP socket");
    }

    amqp_login(conn, amqpVhost, 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, amqpUser, amqpPass);

    amqp_channel_open(conn, 1);

    std::cout << "amqp_channel_open result" << std::endl;

    amqp_get_rpc_reply(conn);

    std::cout << "amqp_get_rpc_reply result" << std::endl;

    queuename = AMQPHelper::declareQueue(conn, 1);

    std::string queuename_str(static_cast<const char*>(queuename.bytes),
                              static_cast<const char*>(queuename.bytes) + queuename.len);

    std::cout << queuename_str << std::endl;

    if (queuename.bytes == NULL) {
        fprintf(stderr, "Out of memory while copying queue name");
        return 1;
    }

    std::cout << "amqp_bytes_malloc_dup result" << std::endl;

    AMQPHelper::bindQueue(conn, 1, queuename, exchange, bindingkey);

    std::cout << "amqp_queue_bind result" << std::endl;
    amqp_get_rpc_reply(conn);

    AMQPHelper::consumeMessages(conn, 1, queuename);
    amqp_get_rpc_reply(conn);

    AMQPConsumer::run(conn);

    amqp_bytes_free(queuename);

    AMQPHelper::closeConnection(conn);
    AMQPHelper::destroyConnection(conn);

    Actor actor{"respond_to_linkedin_posts"};

    std::cout << "Create Actor: " << actor.name() << std::endl;

    return 0;
}
