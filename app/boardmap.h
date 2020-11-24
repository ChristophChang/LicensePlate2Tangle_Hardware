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

#ifndef BOARD_MAP_H 
#define BOARD_MAP_H 

// Boardmap for NUCLEO-L152RE
// TODO:
// Find the right place to define SPI_3
// or try to use another SPI for the Epaper.
// Required in mbed-os\targets\TARGET_STM\TARGET_STM32L1\spi_api.c

// Console configuration
#define CONSOLE_TX              USBTX 
#define CONSOLE_RX              USBRX
#define CONSOLE_BAUD            115200 

// HS_SR04 ultra sound distance sensor
#define HS_SR04_ECHO_PIN        PD_2
#define HS_SR04_TRIGGER_PIN     PC_11

// I2C bus for I-NUCLEO-LRWAN1 shield - Senors: HTS221, LPS22HB, LSM303AGR
#define LRWAN1_I2C_SDA          I2C_SDA
#define LRWAN1_I2C_SCL          I2C_SCL

// 4.2 Waveshare epaper configuration  
#define EPAPER_WS42_MOSI        PA_12 // PC_12  
#define EPAPER_WS42_MISO        NC
#define EPAPER_WS42_SCLK        PA_5// PC_10
#define EPAPER_WS42_CS          PA_15
#define EPAPER_WS42_DC          PC_2
#define EPAPER_WS42_RST         PC_3
#define EPAPER_WS42_BUSY        PB_7
#define EPAPER_WS42_PWR         NC

// I-NUCLEO-LRWAN1 UART configuration
#define LRWAN1_UART_TX          PA_9
#define LRWAN1_UART_RX          PA_10
#define LRWAN1_UART_BAUD        115200

// LoRaWAN credentials
#define LORAWAN_BAND            LORA_BAND_EU_868

#define LORAWAN_APP_KEY         "06EB704DA820830B830E0DE331380D5E"
#define LORAWAN_APP_EUI         "70B3D57ED0037C11"

#define QRCODE_APP_ID           "LicensePlate2Tangle,uid=E24F43FFFE44C3FC"

#endif /* BOARD_MAP_H */