#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "AdafruitIO_WiFi.h"
#include "DHT.h"
#include <ESP32Servo.h>
#include <ArduinoOTA.h>

// WiFi credentials
#define WIFI_SSID "WorkinCholas"
#define WIFI_PASS "WiC!2023"

// Adafruit IO configuration
#define AIO_USERNAME  "Edudemos"
#define AIO_KEY       "aio_BYTA15enBYcJamhC4D8etZEv12h8"
AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *ldrFeed = io.feed("ldr"); 

// DHT Sensor setup
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

// Interval for sensor readings (5 minutes in milliseconds)
const unsigned long SENSOR_INTERVAL = 5 * 60 * 1000;
unsigned long lastSensorReadTime = 0;

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate

  setupWiFi(); // Setup WiFi connection
  setupOTA(); // Setup OTA for remote updates
  setupDHT(); // Setup DHT sensor
  setupLEDsAndServo(); // Setup LED pins and servo
}

void loop() {
  handleOTA(); // Handle OTA updates

  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorReadTime >= SENSOR_INTERVAL) {
    lastSensorReadTime = currentMillis;
    updateSensorData(); // Read and process sensor data
    updateLDRAndServo(); // Read LDR values and control servo
  }

  delay(1000); // Wait one second before the next loop iteration
}

void setupWiFi() {
  // Connect to the WiFi network
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

void setupLEDsAndServo() {
  pinMode(ledPin1Cold, OUTPUT);
  pinMode(ledPin2Good, OUTPUT);
  pinMode(ledPin3Heat, OUTPUT);
  myServo.attach(servoPin); // Attach servo to its pin
}

void updateSensorData() {
  io.run(); // Maintain connection to Adafruit IO

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

void updateLDRAndServo() {
  int valueLdrPin1 = analogRead(ldrPin1) - OFFSET;
  int valueLdrPin2 = analogRead(ldrPin2);
  int difference = abs(valueLdrPin1 - valueLdrPin2);
  int angle = map(difference, 0, 4095, 0, 180);

  // Move the servo to the calculated angle
  myServo.write(angle);
  int sum1, sum2 = 0; 
  const int numReadings = 10; // Number of readings to calculate the average

  for (int i = 0; i < numReadings; i++) {
    sum1 += analogRead(ldrPin1); // Sum the values read from the LDR
    sum2 += analogRead(ldrPin2); // Sum the values read from the LDR
    delay(50); // Small pause between readings
  }
  long sum = (sum1+sum2)/2;
  int average = sum / numReadings; // Calculate the average value

  // Send the average value to Adafruit IO
  ldrFeed->save(average);

  Serial.print("Average LDR value sent: ");
  Serial.println(average);

  // Print LDR values and servo angle
  Serial.print("LDR1 Value: ");
  Serial.print(valueLdrPin1);
  Serial.print(", LDR2 Value: ");
  Serial.print(valueLdrPin2);
  Serial.print(", Servo Angle: ");
  Serial.println(angle);
}