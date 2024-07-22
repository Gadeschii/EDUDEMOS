# ESP32 WiFi and OTA Light Follower

This project enhances the ESP32 light follower system by adding WiFi connectivity and Over-The-Air (OTA) update capabilities. The system uses a servo motor and two LDR sensors to track the light source. With the integration of WiFi, the device can be controlled remotely, and firmware updates can be applied without physical access.

## Project Structure

```
ESP32-WiFi-OTA-Light-Follower
├── src
│   ├── main
│   │   ├── accion.ino
│   │   ├── DHT11.ino
│   │   └── main.ino
├── lib
│   └── README.md
├── .gitignore
└── README.md
```

- [`src/main/main.ino`]
 The main Arduino sketch file for the ESP32 board. It initializes WiFi connectivity, sets up OTA updates, and includes the logic for the light-following functionality using the servo and LDR sensors.

- `src/ESP32/accion.ino`: Provides functions to control the servo based on the light levels detected by the LDR sensors. It utilizes the `Servo` library for servo control.

- `src/ESP32/wifi_ota.ino`: Contains the setup and loop functions necessary for establishing WiFi connectivity and handling OTA updates. This allows for remote management and firmware updates of the device.

- [`lib/README.md`]
 Describes the external libraries used in the project, including `WiFi.h` for WiFi functionality and `ArduinoOTA.h` for OTA updates.

- `.gitignore`: Lists the files and directories that are to be ignored by Git, such as build artifacts and sensitive configuration files.

- [`README.md`] Provides documentation for the project, including an overview, project structure, and setup instructions.

## Features

- **Light Following**: The servo motor adjusts its position to follow the brightest light source detected by the LDR sensors.
- **WiFi Connectivity**: Enables remote control and data transmission over the internet.
- **OTA Updates**: Allows for wireless firmware updates, making it easier to deploy new features or fixes.
## DHT11 Integration

The ESP32 WiFi and OTA Light Follower project includes integration with the DHT11 sensor, which is used to measure the ambient temperature and humidity. These environmental readings can be useful for adjusting the behavior of the device based on the current conditions.

### How It Works

- **Temperature and Humidity Measurement**: The DHT11 sensor periodically measures the ambient temperature and humidity. These measurements can be used to make decisions within the system or be sent over WiFi for remote monitoring.


## Setup and Usage

1. **WiFi Configuration**: Update `wifi_ota.ino` with your WiFi network credentials.
2. **OTA Configuration**: Optionally, set a password for OTA updates in `wifi_ota.ino`.
3. **Compile and Upload**: Use the Arduino IDE to compile and upload the sketch to your ESP32 device.
4. **Control and Update**: Once connected to WiFi, the device can be controlled remotely, and OTA updates can be initiated from the Arduino IDE.

5. **Sensor Connection**: Connect the DHT11 sensor to your ESP32 board. The data pin of the DHT11 should be connected to pin 32, as defined in `src/main/DHT11.ino`.

6. **Reading Data**: The `loopDHT()` function in `src/main/DHT11.ino` is responsible for reading the humidity and temperature from the DHT11 sensor. It prints these values to the serial monitor.

7. **Integration with Main Loop**: Ensure that the `setupDHT()` function is called in your main setup routine, and `loopDHT()` is called within your main loop to continuously read and update the temperature and humidity readings.

This project is a starting point for developing IoT applications with the ESP32 that require remote control and maintenance.