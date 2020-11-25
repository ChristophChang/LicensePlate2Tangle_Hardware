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

#ifndef PARKING_AREA_H
#define PARKING_AREA_H

#include "mbed.h"
#include <string>

#include "HCSR04.h"

/**
  * Parking area information 
  * Check if a park has occupied the parking spot with the HC-SR04 ultrasonic distance sensor
  */
class ParkingArea
{
public:
    ParkingArea();
    ~ParkingArea();

    /**
     * Check if a car is parking on the parking area 
     */
    bool isParkingAreaOccupied() const;

    /**
     * Get time of occupation [s]
     */
    time_t getTimeOfOccupation() const;

    /**
     * Get the distance of the car in [m] 
     */
    float getDistanceOfCar() const;

    /**
     * Print the distance of the car and time 
     */
    void print();

private:
    constexpr static int kMinimumDistanceCm = 100;  // 0.1m
    constexpr static int kMaximumDistanceCm = 2000; // 2m

    bool isOccupied;
    bool objectDetected; 

    float distance;

    time_t timeStartOfOccupation;
    time_t timeOfOccupation;

    HCSR04 distanceSensor;

    Timer parkingTimer;
    Timeout parkingDetectTimeout;

    void run();
};


#endif /* PARKING_AREA_H */