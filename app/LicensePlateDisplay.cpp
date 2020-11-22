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

#include "LicensePlateDisplay.h"

#include "boardmap.h"
#include "helpers.h"

#define COLORED     0
#define UNCOLORED   1


// RAM display buffer. Memory used 300 x 400 / 8 bytes
uint8_t image[EPD_HEIGHT*(EPD_WIDTH/8 + 1)];

// Private parking meter background image
extern const uint8_t bg_welcomescreen[];
extern const uint8_t bg_parkingscreen[];

// Epaper display driver
Epd disp(EPAPER_WS42_MOSI,
         EPAPER_WS42_MISO,
         EPAPER_WS42_SCLK,
         EPAPER_WS42_CS,
         EPAPER_WS42_DC,
         EPAPER_WS42_RST,
         EPAPER_WS42_BUSY,
         EPAPER_WS42_PWR);



LicensePlateDisplay::LicensePlateDisplay() :
              painter(image, EPD_WIDTH, EPD_HEIGHT),
              temperature(23.3),
              batteryLevel(BatteryLevel::MEDIUM),
              qrCode(nullptr),
              license("IO:TA2020"),
              parkingEndTime(1606004894)
{

}

void LicensePlateDisplay::enable()
{
   disp.Init();
}

void LicensePlateDisplay::disable()
{
    disp.Sleep();
}

void LicensePlateDisplay::setTemperature(float temperature)
{
    this->temperature = temperature;
}

void LicensePlateDisplay::setBatteryLevel(BatteryLevel batterylevel)
{
    this->batteryLevel = batteryLevel;
}

void LicensePlateDisplay::setQRCode(QRCode *qrCode)
{
    this->qrCode = qrCode;
}

void LicensePlateDisplay::setLicense(std::string license)
{
    this->license = license;
}
    
void LicensePlateDisplay::setParkingEndTime(time_t parkingEndTime)
{
    this->parkingEndTime = parkingEndTime;
}

void LicensePlateDisplay::showWelcomeScreen()
{
    // Clear frame and send the data
    disp.ClearFrame();
    disp.DisplayFrame();

    paintWelcomeScreen();

    // Now send the image data and refresh the display image
    disp.SetPartialWindowBlack(painter.GetImage(), 0, 0,
                               painter.GetWidth(), painter.GetHeight());
    disp.DisplayFrame();
}

void LicensePlateDisplay::showParkingScreen()
{
    // Clear frame and send the data
    disp.ClearFrame();
    disp.DisplayFrame();

    paintParkingScreen();

    // Now send the image data and refresh the display image
    disp.SetPartialWindowBlack(painter.GetImage(), 0, 0,
                               painter.GetWidth(), painter.GetHeight());
    disp.DisplayFrame();
}

void LicensePlateDisplay::paintWelcomeScreen()
{
    // Clear the display buffer
    painter.Clear(UNCOLORED);

    // Draw the background image
    memcpy(image, bg_welcomescreen, sizeof(image));
    // We have to invert the buffer image buffer
    for (int i = 0; i <= sizeof(image); i++) {
        image[i] = ~image[i];
    }

    // Draw the temperature value in the top right corner
    paintTemperature();

    // Draw the battery level in the top right corner
    paintBatteryLevel();

    if (nullptr != qrCode) {
        constexpr int kScale = 5;
        constexpr int xOrigin = 30;
        constexpr int yOrigin = 100;

        // Draw the QRCode
        for (uint8_t y = 0; y < qrCode->size; y++) {
            for (uint8_t x = 0; x < qrCode->size; x++) {
                painter.DrawFilledRectangle(xOrigin + (x * kScale),
                                            yOrigin + (y * kScale),
                                            xOrigin + (x * kScale) + kScale,
                                            yOrigin + (y * kScale) + kScale,
                                            (int)!qrcode_getModule(qrCode, x, y));
            }
        }
    }
} 

void LicensePlateDisplay::paintParkingScreen()
{
    // Clear the display buffer
    painter.Clear(UNCOLORED);

    // Draw the background image
    memcpy(image, bg_parkingscreen, sizeof(image));
    // We have to invert the buffer image buffer
    for (int i = 0; i <= sizeof(image); i++) {
        image[i] = ~image[i];
    }

    // Draw the temperature value in the top right corner
    paintTemperature();

    // Draw the battery level in the top right corner
    paintBatteryLevel();

    // Convert timestamp to string 
    char buffer[5];
    char oneCharBuffer[2] = "";
    std::tm * ptm = std::localtime(&parkingEndTime);

    // Draw the parking end time
    std::strftime(buffer, sizeof(buffer), "%H%M", ptm);

    oneCharBuffer[0] = buffer[0];
    painter.DrawStringAt(100, 115, oneCharBuffer, &Font4040, COLORED);
    oneCharBuffer[0] = buffer[1];
    painter.DrawStringAt(130, 115, oneCharBuffer, &Font4040, COLORED);
    painter.DrawStringAt(155, 115, ":", &Font4040, COLORED);
    oneCharBuffer[0] = buffer[2];
    painter.DrawStringAt(175, 115, oneCharBuffer, &Font4040, COLORED);
    oneCharBuffer[0] = buffer[3];
    painter.DrawStringAt(205, 115, oneCharBuffer, &Font4040, COLORED);

    // Draw the parking end date
    std::strftime(buffer, sizeof(buffer), "%d%m", ptm);

    oneCharBuffer[0] = buffer[0];
    painter.DrawStringAt(260, 140, oneCharBuffer, &Font4040, COLORED);
    oneCharBuffer[0] = buffer[1];
    painter.DrawStringAt(290, 140, oneCharBuffer, &Font4040, COLORED);
    painter.DrawStringAt(315, 140, ".", &Font4040, COLORED);
    oneCharBuffer[0] = buffer[2];
    painter.DrawStringAt(330, 140, oneCharBuffer, &Font4040, COLORED);
    oneCharBuffer[0] = buffer[3];
    painter.DrawStringAt(360, 140, oneCharBuffer, &Font4040, COLORED);

    // Draw the license plate 
    constexpr int ralign = 35;

    // Country identifier (DE right now)
    painter.DrawStringAt(5,   210, "!", &EuroFont9664, COLORED);

    // Area code
    oneCharBuffer[0] = license[0];
    painter.DrawStringAt(ralign,   210, oneCharBuffer, &EuroFont9664, COLORED);
    oneCharBuffer[0] = license[1];
    painter.DrawStringAt(ralign + 40,  210, oneCharBuffer, &EuroFont9664, COLORED);

    // Badge
    painter.DrawStringAt(ralign + 75,  210, ":", &EuroFont9664, COLORED);

    // Random letters  
    oneCharBuffer[0] = license[3];
    painter.DrawStringAt(ralign + 115, 210, oneCharBuffer, &EuroFont9664, COLORED);
    oneCharBuffer[0] = license[4];
    painter.DrawStringAt(ralign + 155, 210, oneCharBuffer, &EuroFont9664, COLORED);

    // Random Number
    oneCharBuffer[0] = license[5];
    painter.DrawStringAt(ralign + 200, 210, oneCharBuffer, &EuroFont9664, COLORED);
    oneCharBuffer[0] = license[6];
    painter.DrawStringAt(ralign + 240, 210, oneCharBuffer, &EuroFont9664, COLORED);
    oneCharBuffer[0] = license[7];
    painter.DrawStringAt(ralign + 280, 210, oneCharBuffer, &EuroFont9664, COLORED);
    oneCharBuffer[0] = license[8];
    painter.DrawStringAt(ralign + 320, 210, oneCharBuffer, &EuroFont9664, COLORED);

}

void LicensePlateDisplay::paintBatteryLevel()
{
    int colorBar1, colorBar2, colorBar3;

    switch (batteryLevel) {
        case BatteryLevel::FULL :
            colorBar1 = COLORED;
            colorBar2 = COLORED;
            colorBar3 = COLORED;
            break;
        case BatteryLevel::MEDIUM :
            colorBar1 = COLORED;
            colorBar2 = COLORED;
            colorBar3 = UNCOLORED;
            break;
        case BatteryLevel::LITTLE :
            colorBar1 = COLORED;
            colorBar2 = UNCOLORED;
            colorBar3 = UNCOLORED;
            break;
        case BatteryLevel::EMPTY :
        default :
            colorBar1 = UNCOLORED;
            colorBar2 = UNCOLORED;
            colorBar3 = UNCOLORED;
            break;
    }

    // Draw the 3 bars
    painter.DrawFilledRectangle(340, 30, 350, 41, colorBar1);
    painter.DrawFilledRectangle(352, 30, 362, 41, colorBar2);
    painter.DrawFilledRectangle(364, 30, 374, 41, colorBar3);
}

void LicensePlateDisplay::paintTemperature()
{
    char bufferTmp[10];
    char bufferShowValue[8];

    snprintf(bufferShowValue, sizeof(bufferShowValue), "%4sC", print_double(bufferTmp, temperature, 1));
    painter.DrawStringAt(243, 26, bufferShowValue, &Font24, COLORED);
}
