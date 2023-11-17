// AMQPConsumer.cpp
#include "amqp_consumer.hpp"
#include "amqp_helper.hpp"
#include <iostream>
#include <chrono>

#define SUMMARY_EVERY_US 1000000

uint64_t AMQPConsumer::nowMicroseconds() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

void AMQPConsumer::run(amqp_connection_state_t conn) {
    uint64_t start_time = nowMicroseconds();
    int received = 0;
    int previous_received = 0;
    uint64_t previous_report_time = start_time;
    uint64_t next_summary_time = start_time + SUMMARY_EVERY_US;

    amqp_frame_t frame;

    uint64_t now;

    for (;;) {
        amqp_rpc_reply_t ret;
        amqp_envelope_t envelope;

        now = nowMicroseconds();
        if (now > next_summary_time) {
            int countOverInterval = received - previous_received;
            double intervalRate =
                countOverInterval / ((now - previous_report_time) / 1000000.0);
            printf("%d ms: Received %d - %d since last report (%d Hz)\n",
                   (int)(now - start_time) / 1000, received, countOverInterval,
                   (int)intervalRate);

            previous_received = received;
            previous_report_time = now;
            next_summary_time += SUMMARY_EVERY_US;
        }

        amqp_maybe_release_buffers(conn);
        ret = amqp_consume_message(conn, &envelope, NULL, 0);

        if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
            if (AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type &&
                AMQP_STATUS_UNEXPECTED_STATE == ret.library_error) {
                if (AMQP_STATUS_OK != amqp_simple_wait_frame(conn, &frame)) {
                    return;
                }

                if (AMQP_FRAME_METHOD == frame.frame_type) {
                    switch (frame.payload.method.id) {
                        case AMQP_BASIC_ACK_METHOD:
                            // if we've turned publisher confirms on, and we've published a
                            // message here is a message being confirmed.
                            break;
                        case AMQP_BASIC_RETURN_METHOD:
                            // if a published message couldn't be routed and the mandatory
                            // flag was set this is what would be returned. The message then
                            // needs to be read.
                            {
                                amqp_message_t message;
                                ret = amqp_read_message(conn, frame.channel, &message, 0);
                                if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
                                    return;
                                }

                                amqp_destroy_message(&message);
                            }

                            break;

                        case AMQP_CHANNEL_CLOSE_METHOD:
                            // a channel.close method happens when a channel exception occurs,
                            // this can happen by publishing to an exchange that doesn't exist
                            // for example.
                            //
                            // In this case you would need to open another channel redeclare
                            // any queues that were declared auto-delete, and restart any
                            // consumers that were attached to the previous channel.
                            return;

                        case AMQP_CONNECTION_CLOSE_METHOD:
                            // a connection.close method happens when a connection exception
                            // occurs, this can happen by trying to use a channel that isn't
                            // open for example.
                            //
                            // In this case the whole connection must be restarted.
                            return;

                        default:
                            fprintf(stderr, "An unexpected method was received %u\n",
                                    frame.payload.method.id);
                            return;
                    }
                }
            }
        } else {
            amqp_destroy_envelope(&envelope);
        }

        received++;
    }
}
