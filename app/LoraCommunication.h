/*
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef LORA_COMMUNICATION_H
#define LORA_COMMUNICATION_H

#include "mbed.h"
#include <stdint.h>

class LoraCommunication
{
public:
    LoraCommunication();
    ~LoraCommunication();

    enum class Status{
        NOINIT,
        DOWN,
        UP,
        ERROR
    };


    /**
     * Get the status of the LoRa communication
     */
    Status getStatus() const;

    /**
     * Send lora message to the gateway
     * The airtime of LoRa messages is limited by payload, interval and datarate
     * therefore it is not guaranteed that the message is acutally send.
     *
     * So the there is an internal message queue, to send the the data, when it
     * is allowed according to the LoRa dutycyle.
     *
     * If the internal message buffer is full, the message is dropped
     */
    bool sendMessage(const uint8_t *data, size_t length);

    /**
     * If available get the data, that was sent over the LoRa communiation
     */
    bool receiveMessage(uint8_t *buffer, const size_t bufferSize, size_t *length);

    /**
     * Print LRWAN board information on the console
     */
    void printLRWAN1Info();


private:
    Thread thread;

    Status status;

    /**
     * Internal used message buffer
     */
    template<size_t N>
    struct Message{
        uint8_t data[N];
        size_t bytes;

        Message() : bytes(0) {
            memset(data, 0x00, sizeof(data));
        }

        // Make a deep copy
        Message(const uint8_t *data, size_t bytes) {
            this->bytes = bytes;
            memcpy(this->data, data, bytes);
        };
    };

    typedef Message<64> TxMessage;
    typedef Message<80> RxMessage;

    Queue<TxMessage, 4> txMessageQueue;
    Queue<RxMessage, 2> rxMessageQueue;

    time_t rxMessagePollTimer;
    time_t txMessageSendTimer;

    std::string boardDevEUI;
    std::string boardAppKey;
    std::string boardAppEUI;

    /**
     * Initialize the LRWAN1 board
     */
    void initialize();

    bool receive();
    bool transmit();

    /**
     * Is automatically called by mbed os
     */
    void run();


};

#endif /* LORA_COMMUNICATION_H */

