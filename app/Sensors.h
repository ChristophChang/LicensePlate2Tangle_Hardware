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


#ifndef SENSORS_H
#define SENSORS_H

#include "mbed.h"
#include <string>


class DevI2C;
class HTS221Sensor;
class LPS22HBSensor;
class LSM303AGRAccSensor;
class LSM303AGRMagSensor;


// Available sensor / parameter
enum SensorType {
    NoSensors = 0,
    AllSensors,
    Humidity,
    Temperature1,
    Pressure,
    Temperature2,
    Accelerometer,
    Magnetometer
} ;


/**
  * All environment sensors of the parking meter.
  * Could be used for localized weather information.
  */
class Sensors
{
public:
    Sensors();
    ~Sensors();

    /**
     * Enable this sensor
     */
    void enable(SensorType sensor = AllSensors);

    /**
     * Disable this sensor
     */
    void disable(SensorType sensor = AllSensors);

    /**
     * Do a read on the enabled sensors to get new sensor values
     */
    void read();

    /**
     * Get sensor data, of the external sensors
     */
    float getValueFloat(SensorType sensor);

    /**
     * Convert the enabled sensor data to
     * the JSON format
     */
    std::string toJSON();

    /**
     * Print the enabled sensor data to the console
     */
    void print();

private:
    uint8_t enableFlags;

    // Read in data will be store in these variables
    float humidity;                     // [%]
    float temperature1;                 // [C]
    float pressure;                     // [mbar]
    float temperature2;                 // [C]
    int32_t accelerometerAxes[3];       // [mg]
    int32_t magnetometerAxes[3];        // [mgauss]

    DevI2C *i2cLoraShield;
    HTS221Sensor *hts221_humTemp;
    LPS22HBSensor *lps22hb_pressTemp;
    LSM303AGRAccSensor *lsm303agr_acc;
    LSM303AGRMagSensor *lsm303agr_mag;
};


#endif /* SENSORS_H */
