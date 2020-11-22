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

#include "Sensors.h"

#include "boardmap.h"

#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "LSM303AGRAccSensor.h"
#include "LSM303AGRMagSensor.h"

#include "helpers.h"


uint8_t flag[] = { 
    0x00,       // NoSensors 
    0xFF,       // AllSensors 
    0x01,       // Humidity,
    0x02,       // Temperature1,
    0x04,       // Pressure,
    0x08,       // Temperature2,
    0x10,       // Accelerometer,
    0x20        // Magnetometer
};

Sensors::Sensors() :
    enableFlags(0x07)
{
    i2cLoraShield = new DevI2C(LRWAN1_I2C_SDA, LRWAN1_I2C_SCL);

    hts221_humTemp = new HTS221Sensor(i2cLoraShield);
    lps22hb_pressTemp = new LPS22HBSensor(i2cLoraShield);
    lsm303agr_acc = new LSM303AGRAccSensor(i2cLoraShield);
    lsm303agr_mag = new LSM303AGRMagSensor(i2cLoraShield);
}

Sensors::~Sensors()
{
    delete hts221_humTemp;
    delete lps22hb_pressTemp;
    delete lsm303agr_acc;
    delete lsm303agr_mag;
}

void Sensors::enable(SensorType sensor)
{
    // Update the internal enable flags
    enableFlags |= flag[sensor];

    switch (sensor)
    {
    case Humidity :
    case Temperature1 :
        hts221_humTemp->enable(); 
        break;
    case Pressure :
    case Temperature2 :
        lps22hb_pressTemp->enable();
        break;
    case Accelerometer :
        lsm303agr_acc->enable();
        break;
    case Magnetometer :
        lsm303agr_mag->enable();
        break;
    case AllSensors :
        hts221_humTemp->enable(); 
        lps22hb_pressTemp->enable();
        lsm303agr_acc->enable();
        lsm303agr_mag->enable();
    default:
        break;
   }
}

void Sensors::disable(SensorType sensor)
{
    // Update the internal enable flags
    enableFlags &= ~flag[sensor];

    switch (sensor)
    {
    case Humidity :
    case Temperature1 :
        // Can only be disabled, if both values are not requested 
        if( 0x00 == (enableFlags & (flag[Humidity] | flag[Temperature1]))) {
            hts221_humTemp->disable(); 
        }
        break;
    case Pressure :
    case Temperature2 :
        // Can only be disabled, if both values are not requested 
        if( 0x00 == (enableFlags & (flag[Pressure] | flag[Temperature2]))) {
            lps22hb_pressTemp->disable();
        }
        break;
    case Accelerometer :
        lsm303agr_acc->disable();
        break;
    case Magnetometer :
        lsm303agr_mag->disable();
        break;
    case AllSensors :
        hts221_humTemp->disable(); 
        lps22hb_pressTemp->disable();
        lsm303agr_acc->disable();
        lsm303agr_mag->disable();
        break;
    default:
        break;
   }
}

void Sensors::read()
{
    // Only read enabled sensors to save energy
    if((enableFlags & flag[Humidity]) == flag[Humidity]) {
        hts221_humTemp->get_humidity(&humidity);
    }

    if((enableFlags & flag[Temperature1]) == flag[Temperature1]) {
        hts221_humTemp->get_temperature(&temperature1);
    }

    if((enableFlags & flag[Pressure]) == flag[Pressure]) {
        lps22hb_pressTemp->get_pressure(&pressure);
    }

    if((enableFlags & flag[Temperature2]) == flag[Temperature2]) {
        lps22hb_pressTemp->get_temperature(&temperature2);
    }

    if((enableFlags & flag[Accelerometer]) == flag[Accelerometer]) {
        lsm303agr_acc->get_x_axes(accelerometerAxes);
    }

    if((enableFlags & flag[Magnetometer]) == flag[Magnetometer]) {
        lsm303agr_mag->get_m_axes(magnetometerAxes);
    }
}

std::string Sensors::toJSON()
{
    char buffer[32];
    char valueBuffer[10];
    std::string json("{");
    json.append("\"s\:\"0xff\",");
   
    if((enableFlags & flag[Humidity]) == flag[Humidity]) {
        snprintf(buffer, sizeof(buffer), "\"h\":%3s,", print_double(valueBuffer, humidity, 1));
        json.append(buffer);
    }

    if((enableFlags & flag[Temperature1]) == flag[Temperature1]) {
        snprintf(buffer, sizeof(buffer), "\"t\":%3s,", print_double(valueBuffer, temperature1, 1));
        json.append(buffer);
    }

    if((enableFlags & flag[Pressure]) == flag[Pressure]) {
        snprintf(buffer, sizeof(buffer), "\"p\":%3s,", print_double(valueBuffer, pressure, 0));
        json.append(buffer);
    }

    if((enableFlags & flag[Temperature2]) == flag[Temperature2]) {
      snprintf(buffer, sizeof(buffer), "\"t2\":%3s,", print_double(valueBuffer, temperature2, 1));
        json.append(buffer);
    }

    // Delete the last comma, if the string is not empty
    if(!json.empty()) {
        json.pop_back();
    }
    json.append("}");

    return json;
}

void Sensors::print()
{
    char valueBuffer1[10];
    char valueBuffer2[10];

    printf("\r\n");
//    printf("HTS221: [temp] %7s C,   [hum] %s%%\r\n", print_double(valueBuffer1, temperature1), print_double(valueBuffer2, humidity));
//    printf("LPS22HB: [temp] %7s C, [press] %s mbar\r\n", print_double(valueBuffer1, temperature2), print_double(valueBuffer2, pressure));
//    printf("LSM303AGR [mag/mgauss]:  %6ld, %6ld, %6ld\r\n", magnetometerAxes[0], magnetometerAxes[1], magnetometerAxes[2]);
//    printf("LSM303AGR [acc/mg]:  %6ld, %6ld, %6ld\r\n", accelerometerAxes[0], accelerometerAxes[1], accelerometerAxes[2]);
}

