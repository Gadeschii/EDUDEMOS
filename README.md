# Arduino Temperature, Humidity, and Light Control Project

This Arduino project reads temperature and humidity values, controls LEDs based on temperature, and uses Light Dependent Resistors (LDRs) to control a servo motor. The project also sends average LDR values to Adafruit IO.

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

### Temperature and Humidity Reading


The code reads temperature (`t`) and humidity (`h`) values from the sensor and prints them to the Serial Monitor.

```cpp
Serial.print(" °C, Humidity: ");
Serial.print(h);
Serial.println(" %");


##LED Control Based on Temperature

The controlLEDsBasedOnTemperature function controls the LEDs based on the temperature value:

-   If the temperature is below 20°C, the "Cold" LED is turned on.
-   If the temperature is between 20°C and 25°C, the "Good" LED is turned on.
-   If the temperature is above 25°C, the "Heat" LED is turned on.

void controlLEDsBasedOnTemperature(float t) {
  if (t < 20) {
    digitalWrite(ledPin1Cold, HIGH);
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, LOW);
    Serial.println("Cold is HIGH");
  } else if (t >= 20 && t < 25) {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, HIGH);
    digitalWrite(ledPin3Heat, LOW);
    Serial.println("Good is HIGH");
  } else {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, HIGH);
    Serial.println("Heat is HIGH");
  }
}

 
##LDR and Servo Control

The updateLDRAndServo function reads values from two LDRs, calculates the difference, and maps this difference to an angle for the servo motor. It also calculates the average LDR value over multiple readings and sends this value to Adafruit IO.

void updateLDRAndServo() {
  int valueLdrPin1 = analogRead(ldrPin1) - OFFSET;
  int valueLdrPin2 = analogRead(ldrPin2);
  int difference = abs(valueLdrPin1 - valueLdrPin2);
  int angle = map(difference, 0, 4095, 0, 180);

  // Move the servo to the calculated angle
  myServo.write(angle);
  int sum1 = 0, sum2 = 0; 
  const int numReadings = 10; // Number of readings to calculate the average

  for (int i = 0; i < numReadings; i++) {
    sum1 += analogRead(ldrPin1); // Sum the values read from the LDR
    sum2 += analogRead(ldrPin2); // Sum the values read from the LDR
    delay(50); // Small pause between readings
  }
  long sum = (sum1 + sum2) / 2;
  int average = sum / numReadings; // Calculate the average value

  // Send the average value to Adafruit IO
  ldrFeed->save(average);

  Serial.print("Average LDR value sent: ");
  Serial.println(average);

  // Print LDR values and servo angle
  Serial.print("LDR1 Value: ");
  Serial.println(valueLdrPin1);
  Serial.print("LDR2 Value: ");
  Serial.println(valueLdrPin2);
  Serial.print("Servo Angle: ");
  Serial.println(angle);
}
 
##Usage
1. Connect the components to the Arduino as per the pin configuration.
2. Upload the code to the Arduino board.
3. Open the Serial Monitor to view temperature, humidity, and LDR values.
4. Observe the LEDs indicating the temperature range.
5. The servo motor will adjust based on the light difference detected by the LDRs.
6. Average LDR values will be sent to Adafruit IO for logging.


##Dependencies
- Adafruit IO Arduino Library
- Servo Library

##License
This project is licensed under the MIT License - see the LICENSE file for details.


##Acknowledgments
- Adafruit for their excellent libraries and tutorials.
- Arduino community for their support and resources.
