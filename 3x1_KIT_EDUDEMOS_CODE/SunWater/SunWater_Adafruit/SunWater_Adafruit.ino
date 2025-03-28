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
#include <ESP32Servo.h>
#include "DHT.h"

// Pin definitions and constants
#define ldrPin1 34 // set ldr 1 Analog input pin of East ldr as an integer
#define ldrPin2 35 // set ldr 2 Analog input pin of West ldr as an integer
#define DHTPIN 32
#define DHTTYPE DHT11
#define ledPin1Cold 5
#define ledPin2Good 18
#define ledPin3Heat 19
#define SolarPin 36

/************************ Adafruit IO Config *******************************/

#define IO_USERNAME "IO_USERNAME"
#define IO_KEY "IO_KEY"

/******************************* WIFI **************************************/

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASS "WIFI_PASS"

// Global variables
DHT dht(DHTPIN, DHTTYPE);
Servo myservo;
int valldr1 = 0 ;
int valldr2 = 0 ;
int averageLdrValue = 0;
int servopos = 90; // initial position of the Horizontal movement controlling servo motor
int tolerance = 20; // allowable tolerance setting - so solar servo motor isn't constantly in motion
float lastMicrovolts = 0;
const int WaterSensorPin = 39; // // Analog Value from VN PIN
int sensorValue = 0; // Analog Value filter

//store the sensors measurements in Adafruit
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *waterLevelFeed = io.feed("waterLevel");
AdafruitIO_Feed *ldrFeed = io.feed("ldr");
AdafruitIO_Feed *microvoltsMotorDcFeed = io.feed("microvoltsMotorDc");
AdafruitIO_Feed *microvoltsSolarFeed = io.feed("microvoltsSolar");
AdafruitIO_Feed *ledHighFeed =  io.feed("ledHighFeed");
AdafruitIO_Feed *ledGoodFeed =  io.feed("ledGoodFeed");
AdafruitIO_Feed *ledColdFeed =  io.feed("ledColdFeed");
//AdafruitIO_Feed *wifiFeed =  io.feed("wifiFeed");

void setup(){
Serial.begin(115200);
analogReadResolution(12); // ESP32 has a 12-bit ADC
analogSetAttenuation(ADC_11db); // Set attenuation for higher sensitivity
pinMode(4, INPUT);

dht.begin(); // Start the DHT sensor

pinMode(ledPin1Cold, OUTPUT);
pinMode(ledPin2Good, OUTPUT);
pinMode(ledPin3Heat, OUTPUT);

myservo.attach(25); // attaches the servo on digital pin 2 to the horizontal movement servo motor 

pinMode(ldrPin1, INPUT); //set West ldr pin as an input
pinMode(ldrPin2, INPUT); //set East ldr pin as an input
myservo.write(servopos); // write the starting position of the horizontal movement servo motor 
    
delay(1000); // 1 second delay to allow the solar panel to move to its staring position before comencing solar tracking
}

void run(){
 io.run(); // Maintain connection to Adafruit IO
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

void loop(){   
  Serial.println("----------------------------- Measured Solar (V) ----------------------------  ");
  int solarVoltagedV = analogRead ( SolarPin); //dV
  int solarVoltage = solarVoltagedV * 10;

  Serial.print("Voltage of Solar Panel: ");
  Serial.println(solarVoltage);  

  // Calculate voltage
  float voltage = (solarVoltage / 4095.0) * 3.3;  // For 12-bit ADC (0-4095)
  Serial.print("Measured voltage (V): ");
  Serial.println(voltage);  // Display the measured voltage

  // Convert to microvolts
  float microvoltsSolar = voltage * 1000000.0;
  Serial.print("Input voltage (µV): ");
  Serial.println(microvoltsSolar);

  // Apply a minimum threshold to filter noise
  const float threshold = 10.0;  // Threshold in microvolts
  if (microvoltsSolar < threshold) {
    microvoltsSolar = 0;
  }

  // Check if the microvolts value has changed
  if (microvoltsSolar != lastMicrovolts) {
    Serial.print("Send voltage: ");
    Serial.print(microvoltsSolar, 2);  // Display with 2 decimal places
    Serial.println(" µV");

    // Send microvolts to Adafruit IO
    microvoltsSolarFeed->save(microvoltsSolar);

    // Update the last microvolts value
    lastMicrovolts = microvoltsSolar;
  } 

  Serial.println("----------------------- Measured Temperature / Humidity -----------------------  ");
   // Read values from DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if readings are valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print sensor values
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Example LED control based on temperature
  if (t < 20) {
    digitalWrite(ledPin1Cold, HIGH);
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, LOW); 
      // Send to the corresponding feed (Cold LED)
    ledColdFeed->save(1);  // 1 means the LED is ON
    ledGoodFeed->save(0);  // OFF
    ledHighFeed->save(0);  // OFF

    Serial.println("Cold is HIGH");
    
  } else if (t >= 20 && t < 25) {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, HIGH);
    digitalWrite(ledPin3Heat, LOW);
    // Send to the corresponding feed (Good LED)
    ledColdFeed->save(0);  // OFF
    ledGoodFeed->save(1);  // ON
    ledHighFeed->save(0);  // OFF

    Serial.println("Good is HIGH");
  } else {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, HIGH);
    // Send to the corresponding feed (Heat LED)
    ledColdFeed->save(0);  // OFF
    ledGoodFeed->save(0);  // OFF
    ledHighFeed->save(1);  // ON

    Serial.println("Heat is HIGH");
  }

  Serial.println("----------------------------- Measured Water Level -----------------------------  ");
  int sensorValue; // Variable to store sensor values
  sensorValue = analogRead(WaterSensorPin); // Read the value from the soil moisture sensor
  //Check watersensor calibration first
  if (sensorValue >= 2000) { //These value represents de max value given 
    sensorValue = 1980; //This value would return a 99% value
  }
  float waterlevel = (sensorValue*100)/2000 ; //Calculation to return the %
  
  Serial.print("Water level: "); // Print value in serial monitor
  Serial.print(waterlevel); // Print value in serial monitor
  Serial.println(" %"); // Print value in serial monitor

  // Send the average value to Adafruit IO
  waterLevelFeed->save(waterlevel);

  Serial.println("----------------------------- Measured LDR and Servo -----------------------------  ");
    // Read LDR values
  valldr1 = analogRead(ldrPin1); // read the value of ldr 1
  valldr2 = analogRead(ldrPin2); // read the value of ldr 2

  // Invert readings so low values mean darkness and high values mean light
 
  averageLdrValue = (valldr1 + valldr2) / 2;

  // Print readings for debugging
  Serial.print("LDR 1: ");
  Serial.print(valldr1);
  Serial.print(" | LDR 2: ");
  Serial.println(valldr2);
  Serial.print("Average LDR: ");
  Serial.println(averageLdrValue);

  // Send the average value to Adafruit IO
  ldrFeed->save(averageLdrValue);
      
if((abs(valldr1 - valldr2) <= tolerance) || (abs(valldr2 - valldr1) <= tolerance)) {
//no servo motor horizontal movement will take place if the ldr value is within the allowable tolerance
}else {
if(valldr1 > valldr2) // if ldr1 senses more light than ldr2 
{
servopos = servopos+10; // decrement the 90 degree poistion of the horizontal servo motor - this will move the panel position Eastward
}
if(valldr1 < valldr2) // if ldr2 senses more light than ldr1
{
servopos = servopos-10; // increment the 90 degree position of the horizontal motor - this will move the panel position Westward
 }
}
     
if(servopos > 180) {servopos = 180;} // reset the horizontal postion of the motor to 180 if it tries to move past this point
if(servopos < 0) {servopos = 0;} // reset the horizontal position of the motor to 0 if it tries to move past this point
myservo.write(servopos); // write the starting position to the horizontal motor

  // Print the servo angle
  Serial.print("Servo angle: ");
  Serial.println(servopos);
  delay(2500);
}


