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

#include "ParkingArea.h"

#include "boardmap.h"
#include "helpers.h"


ParkingArea::ParkingArea() : distanceSensor(HS_SR04_TRIGGER_PIN, HS_SR04_ECHO_PIN)
{

}

ParkingArea::~ParkingArea()
{

}

bool ParkingArea::isParkingAreaOccupied() const
{
    return isOccupied;
}

time_t ParkingArea::getTimeOfOccupation() const
{
    return timeOfOccupation;
}

float ParkingArea::getDistanceOfCar() const
{
    return distance;
}

void ParkingArea::print()
{
    char buffer[32];
    printf("HC-SR04: [dist] %7s cm\r\n", print_double(buffer, distance));
}


void ParkingArea::run()
{
    // Read in a new measurement value
    distanceSensor.readEcho();
    distance = distanceSensor.getCm();
}




