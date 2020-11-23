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


// Console 
static BufferedSerial serial_port(CONSOLE_TX, CONSOLE_RX, CONSOLE_BAUD);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

int main()
{    
    printf("LicensePlate2Tangle Project\r\n");

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
    display.setTemperature(sensorBoard.getValueFloat(Temperature1));

    // For now the battery level is not supported, for now set it to full
    display.setBatteryLevel(BatteryLevel::FULL);

    // The QRCode will only be generated once
    QRCode qrcode;
    uint8_t *qrcodeData = new uint8_t[qrcode_getBufferSize(4)];
    qrcode_initText(&qrcode, qrcodeData, 4, 0, "LicensePlate2Tangle,uid=E24F43FFFE44C3FC");   

    display.setQRCode(&qrcode);

    // On startup and show the welcome screen 
    display.enable();
    display.showWelcomeScreen();


    // Now start up the lora communication module
    printf("Initializing the Lora communication module\r\n");
    lora.enable(); 

    char messageBuffer[80];
    size_t receivedBytes = 0;
    
    while (true) {

        // Now wait until the lora communication module is ready
        if(LoraCommunication::Status::UP == lora.getStatus()) {
                
            // If ready regulary send sensor data to the server / iota tangle

            // To save energy only enable the sensors for reading
            std::string sensorJson = sensorBoard.toJSON();
            lora.sendMessage((uint8_t*) sensorJson.c_str(), sensorJson.size());

            // ALso if we want to see what was read from the sensors, put it on the console
            printf("Read sensor data and try to send it to the server\r\n");
            printf("Data: ");
            printf(sensorJson.c_str());
            printf("\r\n");

            // Poll for incoming data
            if(true == lora.receiveMessage((uint8_t*)messageBuffer, sizeof(messageBuffer), &receivedBytes)) {
                
                // Todo Now we have to parse the incoming Json message
                // display.setLicense();
                // display.setParkingEndTime();

                // Finally there is booking, so lets show the customer
                display.showParkingScreen();
            }

        }
        
        thread_sleep_for(30000);
    }
}
