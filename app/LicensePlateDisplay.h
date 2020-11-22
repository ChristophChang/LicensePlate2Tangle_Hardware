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


#ifndef LICENSE_PLATE_DISPLAY_H
#define LICENSE_PLATE_DISPLAY_H

#include "epd4in2b.h"
#include "epdpaint.h"

#include "QRCode/src/qrcode.h"

#include "mbed.h"
#include <string>





class LicensePlateDisplay
{
public:
    enum class BatteryLevel
    {
        EMPTY,
        LITTLE,
        MEDIUM,
        FULL
    };

    LicensePlateDisplay();
    ~LicensePlateDisplay();

    /**
     * Enable the Epaper display 
     */
    void enable();

    /**
     * Disable the Epaper display 
     */
    void disable();


    /**
     * Set the temperatur in celsius 
     */
    void setTemperature(float temperature);

    /**
     * Set the battery level information
     */
    void setBatteryLevel(BatteryLevel batterylevel);

    /**
     * Set the QRCode 
     */
    void setQRCode(QRCode *code);

    /**
     * Set the license information of the parking car.
     * Shown with the parking screen 
     */
    void setLicense(std::string license);
    
    /**
     * Set the parking end time.
     * Shown with the parking screen 
     */
    void setParkingEndTime(time_t parkingEndTime);


    /**
     * Show the qrcode on the display, for customer interaction
     */
    void showWelcomeScreen();

    /**
     * Show the parking screen, when the parking lot has been booked
     */
    void showParkingScreen();

private:
    float temperature;
    BatteryLevel batteryLevel;
    QRCode *qrCode;
    std::string license;
    time_t parkingEndTime;
    
    Thread thread;
    Paint painter;
    
    void paintWelcomeScreen();
    void paintParkingScreen();

    /**
     * Paint the batter level on the top right corner
     * This actually updates the bars 
     */
    void paintBatteryLevel();
    void paintTemperature();
};


#endif /* LICENSE_PLATE_DISPLAY_H */