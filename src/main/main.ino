#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "AdafruitIO_WiFi.h"
#include "DHT.h"
#include <ESP32Servo.h>
#include <ArduinoOTA.h>

// WiFi credentials
#define WIFI_SSID "UPE"
#define WIFI_PASS "redwifi.upe"

// Adafruit IO configuration
#define AIO_USERNAME  "Edudemos"
#define AIO_KEY       "aio_BYTA15enBYcJamhC4D8etZEv12h8"
AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *ldrFeed = io.feed("ldr");
AdafruitIO_Feed *microvoltsFeed = io.feed("microvolts"); // New feed for microvolts

//DHT Sensor setup
#define DHTPIN 32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED pin definitions
const int ledPin1Cold = 18;
const int ledPin2Good = 19;
const int ledPin3Heat = 21;

// LDR and Servo setup
const int ldrPin1 = 34;
const int ldrPin2 = 35;
const int servoPin = 33;
const int OFFSET = 75;
Servo myServo;

// Analog pin and resistors setup
const int analogPin = 39; 
const float R1 = 1000.0; // 1kΩ resistor
const float R2 = 1000.0; // 1kΩ resistor

// Interval for sensor readings
const unsigned long SENSOR_INTERVAL_5_MIN = 5 * 60 * 1000; // 5 minutes
const unsigned long SENSOR_INTERVAL_10_SEC = 10 * 1000; // 10 seconds

// Variable to store the last microvolts value
float lastMicrovolts = -1;

// Variables to store the last sensor read times
unsigned long lastSensorReadTime5Min = 0;
unsigned long lastSensorReadTime10Sec = 0;

// Function declarations

void setupWiFi();
void setupOTA();
void setupDHT();
void setupLEDsAndServo();
void handleOTA();
void updateSensorData();
void updateLDRAndServo();
void readVoltage();
void readSensorTask1(void * parameter);
void readSensorTask2(void * parameter);
void readVoltageTask(void * parameter);



void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate
  analogReadResolution(12); // ESP32 has a 12-bit ADC
  analogSetAttenuation(ADC_11db); // Set attenuation for higher sensitivity

  setupWiFi(); // Setup WiFi connection
  setupOTA(); // Setup OTA for remote updates
  setupDHT(); // Setup DHT sensor
  setupLEDsAndServo(); // Setup LED pins and servo

  // Create tasks
  xTaskCreatePinnedToCore(
    readSensorTask1,   // Task function
    "ReadSensorTask1", // Name of the task
    10000,             // Stack size
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    0                  // Core where the task should run
  );

  xTaskCreatePinnedToCore(
    readSensorTask2,   // Task function
    "ReadSensorTask2", // Name of the task
    10000,             // Stack size
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    1                  // Core where the task should run
  );

  xTaskCreatePinnedToCore(
    readVoltageTask,   // Task function
    "ReadVoltageTask", // Name of the task
    10000,             // Stack size
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    1                  // Core where the task should run
  );
}

//Handles OTA updates and maintains the connection to Adafruit IO.

void loop() {
  handleOTA(); // Handle OTA updates
  io.run(); // Maintain connection to Adafruit IO
}

void readSensorTask1(void * parameter) {
  for (;;) {
    Serial.println("----------------------- Measured Temperature / Humidity -----------------------  ");
    updateSensorData(); // Read and process sensor data 
    vTaskDelay(SENSOR_INTERVAL_5_MIN / portTICK_PERIOD_MS); // Delay for 5 minutes
  }
}

void readSensorTask2(void * parameter) {
  for (;;) {
    Serial.println("----------------------------- Measured LDR Motor -----------------------------  ");
    updateLDRAndServo(); // Read LDR values and control servo
    vTaskDelay(SENSOR_INTERVAL_5_MIN / portTICK_PERIOD_MS); // Delay for 5 minutes
  }
}

void readVoltageTask(void * parameter) {
  for (;;) {
    Serial.println("----------------------------- Measured DC Motor (V) ----------------------------  ");
    readVoltage(); // Read and process voltage
    vTaskDelay(SENSOR_INTERVAL_10_SEC / portTICK_PERIOD_MS); // Delay for 10 Seg
  }
}
void readVoltage() {
  
  // Take multiple readings to reduce noise
  const int numReadings = 1000;
  unsigned long sum = 0;

  for (int i = 0; i < numReadings; i++) {
    int reading = analogRead(analogPin);
    sum += reading;
  }

  float averageReading = sum / (float)numReadings;
  Serial.print("Average ADC reading: ");
  Serial.println(averageReading);  // Display the average value

  // Calculate voltage
  float voltage = (averageReading / 4095.0) * 3.3;  // For 12-bit ADC (0-4095)
  Serial.print("Measured voltage (V): ");
  Serial.println(voltage);  // Display the measured voltage

  
  /* 
   Adjust for voltage divider
   The input voltage is calculated using the voltage divider formula.
   R1 and R2 are the resistors used in the voltage divider.
   The formula used is: inputVoltage = voltage / (R2 / (R1 + R2))
*/
  float inputVoltage = voltage / (R2 / (R1 + R2));

  /* 
   Convert to microvolts
   The input voltage is converted to microvolts for higher precision.
   1 volt = 1,000,000 microvolts
*/
  float microvolts = inputVoltage * 1000000.0;
  Serial.print("Input voltage (µV): ");
  Serial.println(microvolts);

 /* 
   Apply a minimum threshold to filter noise
   Any microvolts value below the threshold is set to 0 to filter out noise.
*/
  const float threshold = 10.0;  // Threshold in microvolts
  if (microvolts < threshold) {
    microvolts = 0;
  }

  /* 
   Check if the microvolts value has changed
   If the new microvolts value is different from the last value, it is printed and sent to Adafruit IO.
*/
  if (microvolts != lastMicrovolts) {
    Serial.print("Measured voltage: ");
    Serial.print(microvolts, 2);  // Display with 2 decimal places
    Serial.println(" µV");

    // Send microvolts to Adafruit IO
    microvoltsFeed->save(microvolts);

    // Update the last microvolts value
    lastMicrovolts = microvolts;
  }
}

//  Connects the ESP32 to a WiFi network and Adafruit IO.
void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  // Connect to Adafruit IO
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  // Wait for the connection to be established
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
}

void setupOTA() {
  // Initialize ArduinoOTA for over-the-air updates
  ArduinoOTA.setHostname("ESP32-OTA");
  // ArduinoOTA.setPassword("admin"); // Uncomment to set a password for OTA updates
  // Define OTA event handlers
  ArduinoOTA.onStart([]() {
    Serial.println("Starting OTA update...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Update Finished");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.begin(); // Start the OTA service
}

void handleOTA() {
  ArduinoOTA.handle(); // Handle OTA updates
}

void setupDHT() {
  dht.begin(); // Start the DHT sensor
}

//  Sets up the LED pins and attaches the servo motor to its pin.
void setupLEDsAndServo() {
  pinMode(ledPin1Cold, OUTPUT);
  pinMode(ledPin2Good, OUTPUT);
  pinMode(ledPin3Heat, OUTPUT);
  myServo.attach(servoPin); // Attach servo to its pin
}
//  Reads the temperature and humidity values from the DHT sensor and sends them to Adafruit IO.
void updateSensorData() {
  // Read values from DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if readings are valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send values to Adafruit IO
  temperature->save(t);
  humidity->save(h);

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

//  Reads the LDR values from the two LDRs and controls the servo motor based on the difference between the values.
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