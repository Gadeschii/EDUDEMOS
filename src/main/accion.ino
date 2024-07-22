#include <Arduino.h>
#include <ESP32Servo.h>

const int ldrPin1 = 34; // Assuming this is the pin for the first LDR
const int ldrPin2 = 35; // Assuming this is the pin for the second LDR
const int servoPin = 33; // Defines the pin to which the servo is connected
const int OFFSET = 75;

Servo myServo; // Creates a Servo object

void setupAccion() {
  Serial.begin(115200); // Starts serial communication at 115200 baud
  pinMode(ldrPin1, INPUT); // Sets the first LDR pin as input
  pinMode(ldrPin2, INPUT); // Sets the second LDR pin as input
  myServo.attach(servoPin); // Associates the Servo object with the servo pin
}

void loopAccion() {
  int valueLdrPin1 = analogRead(ldrPin1) - OFFSET; // Reads the value from the first LDR
  int valueLdrPin2 = analogRead(ldrPin2); // Reads the value from the second LDR

  // Calculates the difference between the LDR values
  int difference = abs(valueLdrPin1 - valueLdrPin2);

  // Maps the difference to a range of 0 to 180 degrees
  int angle = map(difference, 0, 4095, 0, 180); // Assuming the reading range of the LDRs is 0 to 4095

  // Moves the servo to the calculated angle
  myServo.write(angle);

  // Sends the read values and the calculated angle to the serial monitor
  Serial.print("LDR1 Value: ");
  Serial.print(valueLdrPin1);
  Serial.print(", LDR2 Value: ");
  Serial.print(valueLdrPin2);
  Serial.print(", Servo Angle: ");
  Serial.println(angle);

  delay(1000); // Waits one second before the next reading
}
