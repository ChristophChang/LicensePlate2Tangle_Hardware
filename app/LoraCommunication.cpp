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

#include "LoraCommunication.h"

#include "LoRaWANNode.h"
#include "HardwareSerial_mbedPort.h"
#include "boardmap.h"

#include "platform/mbed_thread.h"


// Defined with the boardmap.h
const char appKey[] = LORAWAN_APP_KEY;
const char appEUI[] = LORAWAN_APP_EUI;


constexpr time_t kSendDutyTimeS = 30 * 60;       // Try to send data all 30min if data is available
constexpr time_t kReceiveDutyTimeS = 60;         // Check all 1min for incoming data
constexpr uint32_t kThreadSleepTimeMs = 30000;   // Wake up all 30s


// Serial to communicate with the
static BufferedSerial serialLora(LRWAN1_UART_TX, LRWAN1_UART_RX, LRWAN1_UART_BAUD);
HardwareSerial_mbedPort SerialLora(&serialLora);


LoraCommunication::LoraCommunication() : thread(osPriorityNormal, 4096),
                                         status(Status::NOINIT)
{

}

LoraCommunication::~LoraCommunication()
{

}

void LoraCommunication::enable()
{
    thread.start(callback(this, &LoraCommunication::run));
}

LoraCommunication::Status LoraCommunication::getStatus() const
{
    return status;
}

bool LoraCommunication::sendMessage(const uint8_t *data, size_t length)
{
    TxMessage *message = new TxMessage(data, length);
    if(false == txMessageQueue.try_put(message)) {
       // If the message queue is already full, the not used object must be destroyed
       delete message;
    }
    return true;
}

bool LoraCommunication::receiveMessage(uint8_t *buffer, const size_t bufferSize, size_t *length)
{
    RxMessage *message;
    length = 0;

    if(true == rxMessageQueue.try_get(&message)) {
        *length = message->bytes;
        // Todo: If buffersize is higher message->bytes
        memcpy(buffer, message->data, bufferSize);
        delete message;
        return true;
    }
    // No message available
    return false;
}

void LoraCommunication::printLRWAN1Info()
{
    printf("LoRa: Device EUI: %s", boardDevEUI.c_str());
    printf("LoRa: App EUI: %s", boardAppEUI.c_str());
    printf("LoRa: App Key: %s", boardAppKey.c_str());
}

void LoraCommunication::initialize()
{
    status = Status::NOINIT;

    // Enable the module and set the radio band
    printf("Starting LoRa module initialisation...\r\n");

    while (!loraNode.begin(&SerialLora, LORAWAN_BAND))
    {
        printf("Lora module not ready. Trying again...\r\n");
        thread_sleep_for(1000);
    }

    printf("Lora module initialized\r\n");

    thread_sleep_for(3000);

    printf("Starting join OTAA procedure...\r\n");
    // Send a join request and wait for the join accept
    while (!loraNode.joinOTAA(appKey, appEUI))
    {
        // For now, if not successful wait here forever until
        // connection has established
        printf("Join OTAA failed!\r\n");
        thread_sleep_for(2000);
    }

    // The LRWAN1 has been sucessfully connected with your LoRaWAN network e.g. TTN
    printf("Join OTAA success!\r\n");
    printf("\r\nLora module ready, join accepted.\r\n\n");

    // Read out some additonal information about your LRWAN1 board
    printf("Reading lora module information...\r\n");
    loraNode.getDevEUI(boardDevEUI);
    loraNode.getAppEUI(boardAppEUI);
    loraNode.getAppKey(boardAppKey);
    printLRWAN1Info();
    
    status = Status::UP;
}

bool LoraCommunication::receive()
{
    uint8_t port;
    bool result = false;
    RxMessage *message = new RxMessage();
    uint8_t bytes;
    
    // Check if data received from a gateway
    if (loraNode.receiveFrame(message->data, &bytes, &port)) {
        message->bytes = bytes;

        printf("LoRa: Frame received, on port %d\r\n", port);
        for (int i = 0; i < message->bytes; i++) {
            printf("0x%x, ", message->data[i]);
        }
        printf("\r\n");
        result = true;
    }
    else {
        delete message;
    }

    return result;
}

bool LoraCommunication::transmit()
{
    TxMessage *message;
    bool result = false;

    // Only transmit, if there is message to send
    if(true == txMessageQueue.try_get(&message)) {
        // Send the data from port 1
        // int status = loraNode.sendFrame(frameTx, sizeof(frameTx), UNCONFIRMED);
        int status = loraNode.sendFrame((char*) message->data, message->bytes, UNCONFIRMED);

        if (LORA_SEND_ERROR == status) {
            printf("LoRa: Send frame failed!!!\r\n");
            // Put the message back to sent it later
            // Todo: Put it at first place in queue
            if(false == txMessageQueue.try_put(message)) {
                delete message;
            }
        }
        else if (LORA_SEND_DELAYED == status) {
            printf("LoRa: Module busy or duty cycle\r\n");
            // Put the message back to sent it later
            // Todo: Put it at first place in queue
            if(false == txMessageQueue.try_put(message)) {
                delete message;
            }
        }
        else {
            printf("LoRa: Frame sent\r\n");
            result = true;
            delete message;
        }
    }
    return result;
}

void LoraCommunication::run()
{
    initialize();

    txMessageSendTimer = time(NULL);
    rxMessagePollTimer = time(NULL);
    
    while (true)
    {
        // This is a LoRaWAN Class A device so data can only be received 
        // after an uplink transmission. So first we have to send a message
        // in order to get one.
        // https://www.thethingsnetwork.org/docs/lorawan/classes.html

        // IF the time has elapsed send data, if available
        if (time(NULL) > (txMessageSendTimer + kSendDutyTimeS)) {
            transmit();
            txMessageSendTimer = time(NULL);
        }

       // IF the time has elapsed send data, if available
       if (time(NULL) > (rxMessagePollTimer + kReceiveDutyTimeS)) {
           // Since we are not sending so often, but we also don't want 
           // the customer wait too long on an incoming message.
           // So let's send a very short frame and try to listen for incoming 
           // messages.
           char pollByte = 0xff;
           loraNode.sendFrame((char*) pollByte, sizeof(pollByte), UNCONFIRMED);

           thread_sleep_for(10000);
           
           // Now let's see, if there is something in the mailbox
           receive();
           rxMessagePollTimer = time(NULL);
       }

        thread_sleep_for(kThreadSleepTimeMs);
    }
}
