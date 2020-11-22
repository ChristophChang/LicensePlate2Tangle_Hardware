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

#include "mbed.h"
#include "platform/mbed_thread.h"

#include "app/boardmap.h"

#include "app/Sensors.h"
#include "app/LoraCommunication.h"
#include "app/LicensePlateDisplay.h"


// Console 
static BufferedSerial serial_port(CONSOLE_TX, CONSOLE_RX, CONSOLE_BAUD);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

int main()
{    
    Sensors sensorBoard;
    sensorBoard.enable(AllSensors);

//    LoraCommunication lora;
    LicensePlateDisplay display;
    
    sensorBoard.read();
    printf("\r\n");
    sensorBoard.print();
    printf("\r\n");
    std::string sensorJson = sensorBoard.toJSON();
    printf(sensorJson.c_str());
    printf("\r\n");
//    lora.sendMessage(sensorJson.c_str(), sensorJson.size());
  
    while (true) {

  
      
        sensorBoard.read();
        printf("\r\n");
        sensorBoard.print();
        printf("\r\n");

        std::string sensorJson = sensorBoard.toJSON();
        printf(sensorJson.c_str());
        printf("\r\n");
        printf("size %d", sensorJson.size());
        printf("\r\n");
        
//        lora.sendMessage(sensorJson.c_str(), sensorJson.size());
        
        thread_sleep_for(5000);
    }
}
