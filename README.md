
# Arduino Temperature, Humidity, Light, and Voltage Control Project

This Arduino project reads temperature and humidity values, controls LEDs based on temperature, uses Light Dependent Resistors (LDRs) to control a servo motor, and measures the voltage of a DC
## Components Used

- ESP32 Board
- Temperature and Humidity Sensor (e.g., DHT11/DHT22)
- LEDs (3 different colors for Cold, Good, and Heat indicators)
- Light Dependent Resistors (LDRs)
- Servo Motor
- Adafruit IO for data logging

## Pin Configuration

- `ledPin1Cold`: Pin connected to the LED indicating cold temperature.
- `ledPin2Good`: Pin connected to the LED indicating good temperature.
- `ledPin3Heat`: Pin connected to the LED indicating high temperature.
- `ldrPin1`: Pin connected to the first LDR.
- `ldrPin2`: Pin connected to the second LDR.
- `myServo`: Pin connected to the servo motor.

## Code Explanation
## Method Explanations

### setup()

Initializes the serial communication, sets up the WiFi connection, OTA updates, DHT sensor, LEDs, and servo motor. It also creates FreeRTOS tasks for reading sensor data and voltage.

### loop()

Handles OTA updates and maintains the connection to Adafruit IO.

### readVoltage()

Reads the voltage from a DC motor using a voltage sensor, converts it to microvolts, and sends the data to Adafruit IO. It takes multiple readings to reduce noise and applies a minimum threshold to filter out noise.

### readSensorTask1(void * parameter)

A FreeRTOS task that periodically reads and processes temperature and humidity data from the DHT sensor. It sends the data to Adafruit IO and controls the LEDs based on the temperature.

### readSensorTask2(void * parameter)

A FreeRTOS task that periodically reads values from two LDRs, calculates the difference, and adjusts the servo motor based on the light difference. It also sends the average LDR values to Adafruit IO.

### readVoltageTask(void * parameter)

A FreeRTOS task that periodically reads and processes the voltage from a DC motor. It sends the voltage data to Adafruit IO.

### setupWiFi()

Connects the ESP32 to a WiFi network and maintains the connection to Adafruit IO.

### setupOTA()

Initializes ArduinoOTA for over-the-air updates and defines OTA event handlers.

### handleOTA()

Handles OTA updates by calling `ArduinoOTA.handle()`.

### setupDHT()

Initializes the DHT sensor.

### setupLEDsAndServo()

Sets up the pins for the LEDs and attaches the servo motor to its pin.

### updateSensorData()

Reads temperature and humidity values from the DHT sensor, sends the data to Adafruit IO, and controls the LEDs based on the temperature.

### updateLDRAndServo()

Reads values from two LDRs, calculates the difference, adjusts the servo motor based on the light difference, and sends the average LDR values to Adafruit IO.