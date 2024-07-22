// First, we need to include the ArduinoOTA library for OTA updates and the WiFi library for wireless connectivity
#include <ArduinoOTA.h>
#include <WiFi.h>

// Replace these with your network credentials
const char* ssid = "WorkinCholas";
const char* password = "WiC!2023";

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate
  setupMain(); // Setup main functionalities including WiFi connection
  setupAccion(); // Call to the new initialization function from accion.ino
}

void loop() {
  loopMain(); // Main loop function
  loopAccion(); // Call to the new loop function from accion.ino
}

void setupMain() {
  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) { // Wait for the connection to establish
    delay(500); // Delay to prevent spamming
    Serial.print("."); // Print dots to indicate the connection process
  }
  Serial.println("Connected to WiFi"); // Print when connected

  // Initialize ArduinoOTA with the hostname and optional password
  ArduinoOTA.setHostname("ESP32-OTA"); // Set the device's name for OTA updates
  // ArduinoOTA.setPassword("admin"); // Uncomment to set a password for OTA updates

  // Define OTA event handlers
  ArduinoOTA.onStart([]() {
    Serial.println("Starting OTA update..."); // Called when the OTA update starts
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Update Finished"); // Called when the OTA update ends
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100))); // Called to show OTA update progress
  });

  ArduinoOTA.begin(); // Start the OTA service
}

void loopMain() {
  ArduinoOTA.handle(); // Handle OTA updates
}
