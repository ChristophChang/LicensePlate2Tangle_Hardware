/*
 *  Copyright (C) ChaLie    November 13 2020
 * 
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

#include "mbed.h"
#include "platform/mbed_thread.h"

#include "app/boardmap.h"

#include "app/Sensors.h"
#include "app/LoraCommunication.h"
#include "app/LicensePlateDisplay.h"
#include "app/ParkingArea.h"

#include "QRCode/src/qrcode.h"
#include "JSON/Json.h"

// Console 
static BufferedSerial serial_port(CONSOLE_TX, CONSOLE_RX, CONSOLE_BAUD);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

/**
 * Parse a json message and look for license plate and endtime information 
 */
bool parseJsonMessage(const char* jsonString, size_t length, std::string &license, time_t &endtime)
{

    Json json(jsonString, strlen(jsonString));

    // Do some sanity checking first
    if (!json.isValidJson()) {
        printf("JSON: json string is not valid. String: %sr\r\n", jsonString);
        return false;
    }

    if (json.type(0) != JSMN_OBJECT) {
        printf("JSON: Root object is not a valid json object\r\n");
        return false;
    }

    // 1. find valid license plate data
    int keyIndex = json.findKeyIndexIn("l", 0);

    if (-1 == keyIndex) {
        printf("JSON: No valid license data found\r\n");
        return false;
    }
    else {
        int valueIndex = json.findChildIndexOf(keyIndex, -1);
        if (valueIndex > 0) {
            const char *valueStart = json.tokenAddress(valueIndex);
            int valueLength = json.tokenLength(valueIndex);

            char value[32];
            strncpy(value, valueStart, valueLength);
            value[valueLength] = 0; // NULL-terminate the string
            license = std::string(value);
        }
    }

    // 2. find the parking end time 
    keyIndex = json.findKeyIndexIn("t", 0);

    if (-1 == keyIndex) {
        printf("JSON: No valid parking end time data found\r\n");
        return false;
    }
    else {
        int valueIndex = json.findChildIndexOf(keyIndex, -1);
        if (valueIndex > 0) {
            const char *valueStart = json.tokenAddress(valueIndex);
            int valueLength = json.tokenLength(valueIndex);

            // Even we are looking for a numer the time_t is a 8byte value
            // therefore we must 
            char value[32];
            strncpy(value, valueStart, valueLength);
            value[valueLength] = 0; // NULL-terminate the string
            endtime = (time_t) stoll(value);
        }
    }
    
    return true;
}


/**
 * The main program starts here
 */
int main()
{    
    printf("\r\n");
    printf("=====================================\r\n");
    printf("=====LicensePlate2Tangle Project=====\r\n");
    printf("=====================================\r\n\n");

    // Initialize the license plate components
    ParkingArea parkingArea;
    Sensors sensorBoard;
    LoraCommunication lora;
    LicensePlateDisplay display;

    printf("Initializing Waveshare 4.2 Epaper display\r\n");
    // Enable sensors to read data from
    sensorBoard.enable(Temperature1);
    sensorBoard.read();

    // Update the display information with the temperature value
    // read from the sensor
    printf("Get temperature\r\n");
    display.setTemperature(sensorBoard.getValueFloat(Temperature1));

    // For now the battery level is not supported, for now set it to full
    printf("Get battery level\r\n");
    display.setBatteryLevel(BatteryLevel::FULL);

    // The QRCode will only be generated once
    printf("Create QRCode from device\r\n");
    QRCode qrcode;
    uint8_t *qrcodeData = new uint8_t[qrcode_getBufferSize(4)];
    qrcode_initText(&qrcode, qrcodeData, 4, 0, QRCODE_APP_ID);   

    display.setQRCode(&qrcode);

    // On startup and show the welcome screen 
    printf("Show welcome screen\r\n");
    display.enable();
    
    // Now start up the lora communication module
    printf("Initializing the Lora communication module\r\n");
    lora.enable(); 


    char messageBuffer[80];
    size_t receivedBytes = 0;
    uint8_t deviceStatus = 0x00;

    while (true) {
        // Now wait until the lora communication module is ready
        if (LoraCommunication::Status::UP == lora.getStatus()) {
            
            // If ready regulary send sensor data to the server / iota tangle
            std::string sensorJson("{");
            // Read the sensor information and put it to in the JSON string
            sensorJson.append(sensorBoard.toJSON());
            sensorJson.append(",\"s\":\"0x00\"}");
            // Print the data we want to send
            printf("%s\r\n", sensorJson.c_str());

            // Try to send the message
            lora.sendMessage((uint8_t *)sensorJson.c_str(), sensorJson.size());


            // Poll for incoming data
            if (true == lora.receiveMessage((uint8_t *)messageBuffer, sizeof(messageBuffer), &receivedBytes))
            {
                std::string license;
                time_t endtime;

                if(true == parseJsonMessage(messageBuffer, sizeof(receivedBytes), license, endtime)) {
                    printf("Show the ne booking parameter on the parking sign\r\n");
                    printf("License Plate: %s\r\n", license.c_str());

                    display.setLicense(license);
                    display.setParkingEndTime(endtime);

                    // Finally there is booking, so lets show the customer
                    display.showParkingScreen();
                }
                else {
                    printf("Received message from the server is not a valid json message\n");
                }

            }
        }

        thread_sleep_for(30000);
    }
}
