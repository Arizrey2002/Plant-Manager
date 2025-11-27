# Plant-Manager
“Plant Manager”, is an Arduino one based project that allows the user to take care of a selected type of plant and check its status via Telegram chat. 

This document summarizes the project, to see the complete guide of the programming side check the “Programming Document - Plant Manager”. And to learn how to use it check the “User Guide - Plant Manager”.


Objectives
To better understand this project we must first analyze the problem of taking care of a plant. Nowadays it is common to have a plant at home, and sooner than later we learn that most plants required certain conditions to grow healthy, but we as individuals cannot measure with precision the temperature, humidity and light level without equipment, that makes it inconvenient for most people, and it is also an even greater problem when we are away from our home. 

This project attempts to:
Measure several ambient variables that impact our plants development (Light, Humidity and temperature).
Create a system that allows us to be informed of the water level so as to refill our water tank.
Display with visual aid the plant status.
Create several plant profiles to personalize its care.
Connect it to a communication service so as to have all this information at any time.

Components
In order to monitorize the environment of the plant the project counts with several sensor to evaluate several ambiental conditions that are relevant to the plants health, the sensors and equipment selected to utilize in this project are:
Arduino One R3 
DHT11 (Humidity and Temperature sensor)
YL69 (Ground Humidity sensor)
SEN2600 (Water level sensor)
KY018 (Light level sensor)
HCSR04 (Proximity sensor)
ESP8266-01 (WiFi module)
ESP8226-01-Adapter (Module that simplifies Arduino One connectivity with ESP8266-01)
ESP8226-01-USB (USB to program ESP8226-01)
LCD display
LCD 12C-adapter (Module that reduces the quantity of cables used in the LCD connection)
8x8 Led Matrix
2 buttons
Connection Cables
Protoboard
