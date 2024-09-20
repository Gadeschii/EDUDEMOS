#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "AdafruitIO_WiFi.h"
#include "DHT.h"
#include <ESP32Servo.h>
#include <ArduinoOTA.h>
#include "config.h"

// Pin definitions and constants
#define DHTPIN 32
#define DHTTYPE DHT11
#define ledPin1Cold 18
#define ledPin2Good 19
#define ledPin3Heat 21
#define ldrPin1 34
#define ldrPin2 35
#define servoPin 25
#define analogPin 39
#define SENSOR_INTERVAL_5_MIN 300000
#define SENSOR_INTERVAL_10_SEC 10000
#define OFFSET 350

// // WiFi credentials
// #define WIFI_SSID "your_wifi_ssid"
// #define WIFI_PASS "your_wifi_password"

// Adafruit IO credentials
// #define AIO_USERNAME "your_aio_username"
// #define AIO_KEY "your_aio_key"

// Voltage divider resistances definitions
const float R1 = 1000.0; // 1kΩ resistor
const float R2 = 1000.0; // 1kΩ resistor

// Global variables
DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

// Variables to store LDR sensor readings
int ldrValue1 = 0;
int ldrValue2 = 0;
int averageLdrValue = 0;

// Servo configuration variables
int servoPos = 90;  // Initial servo position in the middle (90 degrees)
int servoStep = 5;  // Servo movement increment/decrement
int tolerance = 200; // Tolerance to avoid small movements

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *ldrFeed = io.feed("ldr");
AdafruitIO_Feed *microvoltsFeed = io.feed("microvolts");
float lastMicrovolts = 0;

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
    4096,             // Stack size
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    0                  // Core where the task should run
  );

  xTaskCreatePinnedToCore( 
    readSensorTask2,   // Task function
    "ReadSensorTask2", // Name of the task
    4096,             // Stack size
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    1                  // Core where the task should run
  );

  xTaskCreatePinnedToCore(
    readVoltageTask,   // Task function
    "ReadVoltageTask", // Name of the task
    4096,             // Stack size
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    1                  // Core where the task should run
  );
}

void loop() {
  handleOTA(); // Handle OTA updates
  io.run(); // Maintain connection to Adafruit IO
}

void setupWiFi() {
  int retries = 0;
  const int maxRetries = 5;

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED && retries < maxRetries) {
    delay(1000);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
  } else {
    Serial.println("\nWiFi connection failed. Restarting...");
    ESP.restart();
  }

  connectToAdafruitIO();
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

void setupLEDsAndServo() {
  pinMode(ledPin1Cold, OUTPUT);
  pinMode(ledPin2Good, OUTPUT);
  pinMode(ledPin3Heat, OUTPUT);
  // Attach the servo to the corresponding pin
  myServo.attach(servoPin);

  // Set the servo to the initial position (90 degrees, in the middle)
  myServo.write(servoPos);
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
    Serial.println("----------------------------- Measured LDR and Servo -----------------------------  ");
    updateLDRAndServo(); // Read LDR values and control servo
    vTaskDelay(SENSOR_INTERVAL_10_SEC / portTICK_PERIOD_MS); // Delay for 10 seconds
  }
}

void readVoltageTask(void * parameter) {
  for (;;) {
    Serial.println("----------------------------- Measured DC Motor (V) ----------------------------  ");
    readVoltage(); // Read and process voltage
    vTaskDelay(SENSOR_INTERVAL_10_SEC / portTICK_PERIOD_MS); // Delay for 10 seconds
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

  // Adjust for voltage divider
  float inputVoltage = voltage / (R2 / (R1 + R2));

  // Convert to microvolts
  float microvolts = inputVoltage * 1000000.0;
  Serial.print("Input voltage (µV): ");
  Serial.println(microvolts);

  // Apply a minimum threshold to filter noise
  const float threshold = 10.0;  // Threshold in microvolts
  if (microvolts < threshold) {
    microvolts = 0;
  }

  // Check if the microvolts value has changed
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
  // Read LDR values
  ldrValue1 = analogRead(ldrPin1);
  ldrValue2 = analogRead(ldrPin2) - OFFSET;

  // Invert readings so low values mean darkness and high values mean light
  ldrValue1 = 4095 - ldrValue1;
  ldrValue2 = 4095 - ldrValue2;
  averageLdrValue = (ldrValue1 + ldrValue2) / 2;

  // Print readings for debugging
  Serial.print("LDR 1: ");
  Serial.print(ldrValue1);
  Serial.print(" | LDR 2: ");
  Serial.println(ldrValue2);
  Serial.print("Average LDR: ");
  Serial.println(averageLdrValue);

  // Send the average value to Adafruit IO
  ldrFeed->save(averageLdrValue);

  // Compare LDR values and move the servo based on the difference
  if (ldrValue1 > ldrValue2 + tolerance) {
    if (servoPos > 0) {
      servoPos -= servoStep;  // Move the servo to the left
      Serial.println("Moving left");
    }
  } else if (ldrValue2 > ldrValue1 + tolerance) {
    if (servoPos < 180) {
      servoPos += servoStep;  // Move the servo to the right
      Serial.println("Moving right");
    }
  }

  // Ensure the servo stays within the [0, 180] range
  servoPos = constrain(servoPos, 0, 180);

  // Move the servo to the new position only if it has changed
  myServo.write(servoPos);

  // Print the servo angle
  Serial.print("Servo angle: ");
  Serial.println(servoPos);

  // Pause to give the servo time to move
  delay(30);  // Increase the delay for more response time for the servo
}
