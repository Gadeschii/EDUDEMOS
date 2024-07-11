# ESP32 Light Follower

This project uses an ESP32 board, a servo motor, and two LDR sensors to create a light-following system. The servo motor moves in the direction of the sensor with the highest light level.

## Project Structure

```
ESP32-Light-Follower
├── src
│   ├── main.ino
│   └── LDRServoControl
│       └── LDRServoControl.ino
├── lib
│   └── README.md
├── .gitignore
└── README.md
```

- `src/main.ino`: This file is the main Arduino sketch file. It contains the setup and loop functions for the ESP32 board. It includes the `LDRServoControl` library and initializes the servo and LDR sensors. It also implements the logic to control the servo based on the light levels from the LDR sensors.

- `src/LDRServoControl/LDRServoControl.ino`: This file is a library that provides functions to control the servo based on the light levels from the LDR sensors. It includes the `Servo` library and defines a class `LDRServoControl` with methods to read the light levels from the LDR sensors and move the servo accordingly.

- `lib/README.md`: This file contains information about the libraries used in the project.

- `.gitignore`: This file specifies which files and directories should be ignored by Git version control.

- `README.md`: This file contains the documentation for the project.

## Getting Started

To get started with the ESP32 Light Follower project, follow these steps:

1. Connect the servo motor and LDR sensors to the ESP32 board according to the circuit diagram.

2. Open the `src/main.ino` file in the Arduino IDE.

3. Install any required libraries mentioned in the `lib/README.md` file.

4. Upload the sketch to the ESP32 board.

5. Power on the system and observe how the servo motor moves in the direction of the sensor with the highest light level.

For more detailed instructions and troubleshooting tips, refer to the documentation in the `README.md` file.

## License

This project is licensed under the [MIT License](LICENSE).
```

Please note that the content provided is a template and may need to be customized based on your specific project requirements.