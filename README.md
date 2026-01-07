# A program to read NFC-enabled card data using a reader and send it to a server.

## Installation process 

## How to clone the project
  ```sh
  git clone https://github.com/anthon793/arduino-nfc-card-reader.git
  ```

## This project demonstrates how to use an ESP32 microcontroller to read NFC card data using an MFRC522 RFID reader and display the card UID on an LCD. The UID is also sent to a Flask server, which stores the data in a PostgreSQL database.

# Hardware Requirements
* ESP32 microcontroller
* MFRC522 RFID reader
* LCD with I2C interface
* Jumper wires
* Breadboard

# wiring Connections

## MFRC522
---------------

|Pin   |    ESP32 Pin |
|------|--------------|
|SDA   |    GPIO 5 |
|SCK   |    GPIO 18|
|MOSI  |    GPIO 23|
|MISO  |    GPIO 19|
|IRQ   |    unconnected
|GND   |    GND
|RST   |    GPIO 22
|3.3V  |    3.3V

Caution: You must power this device to 3.3V!

## I2C module
-------------

|I2C Character LCD |  Arduino|
|------------------|---------|
GND         	  |  GND
VCC        	  |  5V/VIN
SDA        	  |  GPIO 2
SCL         	  |  GPIO 15



# Software Requirements
* Arduino IDE
* Python
* Postgresql


# Arduino Setup
1. Install the following libraries on the Arduino IDE
* ArduinoJson
* MFRC522
* LiquidCrystal_I2C

2. Open the nfc.ino file in the Arduino IDE
3. Update the WIFI credentials and server URL in the nfc.ino file
4. upload the code to the ESP 32

# Python setup
1. Create a Virtual environment and activate it
2. Install the required python packages:
    ```sh
    pip install -r requirements.txt
3. Update the database confirguration in config.py
4. initialize the database by running the Flask application
   ```sh
   python server.py