#include <DHT.h>

// DHT Sensor setup
const int DHTPin = 32; // Define the pin connected to the DHT11 sensor
#define DHTTYPE DHT11   // Define the type of DHT sensor
DHT dht(DHTPin, DHTTYPE); // Initialize the DHT sensor

// LED pin definitions
const int ledPin1Cold = 18;
const int ledPin2Good = 19;
const int ledPin3Heat = 21;

void setupDHT() {
  dht.begin(); // Starts the DHT sensor

  // Setup LED pins as outputs
  pinMode(ledPin1Cold, OUTPUT);
  pinMode(ledPin2Good, OUTPUT);
  pinMode(ledPin3Heat, OUTPUT);
}

void loopDHT() {
  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity(); // Read humidity
  float temperature = dht.readTemperature(); // Read temperature as Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Example LED control based on temperature
  if (temperature < 20) {
    digitalWrite(ledPin1Cold, HIGH); // Turn on LED 1 for temp < 20°C
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, LOW);
    Serial.println("Cold is HIGH");
  } else if (temperature >= 20 && temperature < 25) {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, HIGH); // Turn on LED 2 for 20°C <= temp < 25°C
    digitalWrite(ledPin3Heat, LOW);
    Serial.println("Good is HIGH");
  } else {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, HIGH); // Turn on LED 3 for temp >= 25°C
    Serial.println("Heat is HIGH");
  }

  // Print the humidity and temperature values to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temp: ");
  Serial.print(temperature);
  Serial.println(" Celsius");
}
