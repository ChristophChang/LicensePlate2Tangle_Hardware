# LicensePlate2Tangle_Hardware
License Plate with EPaper Display and LoRaWAN for the IOTA LicensePlate2Tangle Project

## The project LicensePlate2Tangle
This parking sign hardware is part of the LicensePlate2Tangle project,
which also includes a server side application with the business logic for booking and payment options
with IOTA crypto currency.
An additional Android App allows for direct interaction with the parking sign and server communication. 

* LicensePlate2Tangle Project:
https://github.com/Limi11/IOTLicensePlate2Tangle

* LicensePlate2Tangle Server: 
https://github.com/Limi11/LicensePlate2Tangle_Server

* LicensePlate2Tangle Android App: 
https://www.google.com/url?q=https://github.com/Jomai95/IOTA_App

* LicensePlate2Tangle Parking Sign:
https://github.com/ChristophChang/LicensePlate2Tangle_Hardware


LicensePlate2Tangle is participating in the IOT2Tangle Hackathon
https://hackathon.iot2tangle.io/


## The Team
The project team consists of three dedicated developers with current task division as follows:

Milan Liebsch - Server application development

Johannes Maisch - Android App development

Christoph Chang - Parking sign development


## The LicensePlate2Tangle Parking Sign - An Overview
The parking sign is a LoRaWAN enabled electronic device, to display the car license information
on an energy efficient epaper display. It also allows for direct customer interaction for 
booking a parking spot and paying with IOTA, by an automatic generated QR code.

The license plate information end reservation end time will be transmitted over LoRaWAN to 
the parking sign and shown to the customer.

Communications:
* LoRa communication using the TTN network with many gateways available 
* Passing the data from TTN to a AWS server
* QR code for Android App interaction
* EPaper display for customer interaction

Furthermore the parking sign has a variety of different environment sensors,
and allows for data transmission over LoRaWAN to a IOTA streams gateway.
The sensor data provided are:

Sensor data:
* Temperature
* Humidity
* Air pressure
* Distance measurement sign to car
* Battery level (not supported yet) 


## Instruction how build your own parking sign
1. [Get the materials](https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/Order-the-parts)
2. [Build your your parking sign hardware](https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/Build-the-hardware)
3. [Get the software and run it on the device](https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/Get-the-software-running)
4. [Integrate it to The Things Network and the server](https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/TTN-LoRaWAN-network-and-LicensePlate2Tangle-server-connection)


## Project progress for the IOT2TangleHackathon
Especially for the IOT2Tangle Hackathon we provide additional information here,
to showcase what was accomplished so far:

The following milestones have been reached, with parking sign hardware
Additional information regarding the server and android application can be accessed 
with our project main page)

Reached parking sign milestones:
* Define a system architecture: By defining the hardware and communication technology 
* Research for fitting components
* Get a bill of materials ready to order the parts
* Find a fitting pinout by checking the schematics and pcbs
* Assemble the hardware: Nucleo board, LoRa hat, Epaper, distance sensor and housing
* Start with testing the single hardware components

Regarding the reached firmware milestone and overcome problems:
* Find driver or in our case adapt the Arduino STM LoRaWAN driver for Mbed os :
https://os.mbed.com/users/ChrisChang/code/I-NUCLEO-LRWAN1-mbedPort/
* Control the epaper display
* Control the I-Nucleo-LRWAN1 board with AT-commands
* Fix 3rd party lib driver issues (epaper, LRWAN1)
* Integrated a fitting Euro font: https://www.autokennzeichen.info/kennzeichen-schriftart.htm
* Find a qr code generator and integrate it
* Write a gui to draw the parking sign views
* Connect the device with an over the air authentication (OTAA) to The Things Network (TTN) : https://www.thethingsnetwork.org/
* Send valid sensor data to the TTN network
* Parse incoming messages to showcase information on the epaper

Regarding the hardware assembly:
* The display has been integrate with the parking sign plate
* The electronics have been integrated with the housing 

Milestones open or not completed: 
* Receive sensor data from a server -> is implemented and has worked, however there seems to be a bug, for indicating the message was received (might be the LoRaWAN module, the driver, or a problem with the application)
* Distance sensor works, however is not integrated, since a more intelligent car recognition needs more time (e.g. use a Kalman filter, etc.)

![Electronic assembly](https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/hw_components.jpg)

![Lora TTN gateway connection](https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/hw_lora_connection.png)

<img src="https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/hw_welcome_view.jpg" alt="drawing" height="450"/>

<img src="https://github.com/ChristophChang/LicensePlate2Tangle_Hardware/wiki/hw_parking_screen.jpg" alt="drawing" height="450"/>



