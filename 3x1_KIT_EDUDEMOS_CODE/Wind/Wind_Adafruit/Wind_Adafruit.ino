/*                                                                                                           *
 *  Thank you for building EduDemoS!                                                                         *
 *                                                                                                           *
 *  This file is part of the EduDemoS Project and is licensed under the Creative Commons Attribution         *
 *  NonCommercial ShareAlike 4.0 International license (CC BY-NC-SA 4.0). For full license details, see the  *
 *  license.txt file or visit https://creativecommons.org/licenses/by-nc-sa/4.0/                             *     
 *                                                                                                           *
 *  EduDemoS is co-funded by the European Union. You can find more information at edudemos.eu                *                                                         
 *                                                                                                           *
 *  +-----------------------------------------------------------------------------------------------------+  *            
 */

//include the necesary libraries
#include <WiFi.h>
#include <ArduinoJson.h>
#include "AdafruitIO_WiFi.h"

// Pin definitions and constants
#define DcMotorPin 39

/************************ Adafruit IO Config *******************************/
#define IO_USERNAME "IO_USERNAME"
#define IO_KEY "IO_KEY"

/******************************* WIFI **************************************/
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASS "WIFI_PASS"

// Global variables
float lastMicrovolts = 0;
// Voltage divider resistances definitions
const float R1 = 1000.0; // 1kΩ resistor
const float R2 = 1000.0; // 1kΩ resistor

//store the sensors measurements in Adafruit
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *microvoltsMotorDcFeed = io.feed("microvoltsMotorDc");
//AdafruitIO_Feed *wifiFeed =  io.feed("wifiFeed");

void setup(){
Serial.begin(115200);
analogReadResolution(12); // ESP32 has a 12-bit ADC
analogSetAttenuation(ADC_11db); // Set attenuation for higher sensitivity
pinMode(4, INPUT);
}

void connectToAdafruitIO() {
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  int retries = 0;
  const int maxRetries = 5;

  while (io.status() < AIO_CONNECTED && retries < maxRetries) {
    Serial.print(".");
    delay(500);
    retries++;
  }

  if (io.status() < AIO_CONNECTED) {
    Serial.println("\nFailed to connect to Adafruit IO. Restarting...");
    ESP.restart();
  }

  Serial.println("\nConnected to Adafruit IO");
}

void loop (){
io.run(); // Maintain connection to Adafruit IO
//read DC Motor Voltage
  Serial.println("----------------------------- Measured DC Motor (V) ----------------------------  ");
  // Take multiple readings to reduce noise
  const int numReadings = 1000;
  unsigned long sum = 0;

  for (int i = 0; i < numReadings; i++) {
    int reading = analogRead(DcMotorPin);
    sum += reading;
  }

  float averageReading = sum / (float)numReadings;
  Serial.print("Average ADC reading: ");
  Serial.println(averageReading);  // Display the average value

  // Calculate voltage
  float voltage = (averageReading / 4095.0) * 3.3;  // For 12-bit ADC (0-4095)
  Serial.print("Measured voltage (V): ");
  Serial.println(voltage);  // Display the measured voltage

  // Adjust for voltage divider
  float inputVoltage = voltage / (R2 / (R1 + R2));

  // Convert to microvolts
  float microvoltsMotorDc = inputVoltage * 1000000.0;
  Serial.print("Input voltage (µV): ");
  Serial.println(microvoltsMotorDc);

  // Apply a minimum threshold to filter noise
  const float threshold = 10.0;  // Threshold in microvolts
  if (microvoltsMotorDc < threshold) {
    microvoltsMotorDc = 0;
  }

  // Check if the microvolts value has changed
  if (microvoltsMotorDc != lastMicrovolts) {
    Serial.print("Measured voltage: ");
    Serial.print(microvoltsMotorDc, 2);  // Display with 2 decimal places
    Serial.println(" µV");

    // Send microvolts to Adafruit IO
    microvoltsMotorDcFeed->save(microvoltsMotorDc);

    // Update the last microvolts value
    lastMicrovolts = microvoltsMotorDc;
  }
}
