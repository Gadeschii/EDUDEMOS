// File: /ESP32-Light-Follower/src/LDRServoControl/LDRServoControl.ino

#include <Servo.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 5     // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11
#define LED_PIN 13    // Pin where the LED is connected
DHT dht(DHTPIN, DHTTYPE);

#define WATER_SENSOR_PIN 6 // Pin where the water sensor is connected


class LDRServoControl {
  private:
    int servoPin;
    int ldrPin1;
    int ldrPin2;
    Servo servo;

  public:
    LDRServoControl(int servoPin, int ldrPin1, int ldrPin2) {
      this->servoPin = servoPin;
      this->ldrPin1 = ldrPin1;
      this->ldrPin2 = ldrPin2;
    }

    void setup() {
      servo.attach(servoPin);
      pinMode(ldrPin1, INPUT);
      pinMode(ldrPin2, INPUT);
      dht.begin();
      pinMode(WATER_SENSOR_PIN, INPUT);
      
    }

void loop() {
  int lightLevel1 = analogRead(ldrPin1);
  int lightLevel2 = analogRead(ldrPin2);
  int targetPosition;
  int currentPosition = servo.read(); // Reads the current position of the servo

  if (lightLevel1 > lightLevel2) {
    targetPosition = 0; // Move the servo towards the direction with more light
  } else {
    targetPosition = 180; // Move the servo towards the other direction
  }

  // Gradually moves the servo to the target position
  if (currentPosition < targetPosition) {
    for (int pos = currentPosition; pos <= targetPosition; pos += 1) { // Gradually increases
      servo.write(pos);
      delay(20); // Waits 20ms between each step to slow down the movement
    }
  } else if (currentPosition > targetPosition) {
    for (int pos = currentPosition; pos >= targetPosition; pos -= 1) { // Gradually decreases
      servo.write(pos);
      delay(20); // Waits 20ms between each step to slow down the movement
    }
  }
  
  // Read the light levels every 2 seconds
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read the water sensor
  int waterDetected = digitalRead(WATER_SENSOR_PIN);

  
  if (waterDetected == HIGH) {
    // turn on a LED leveling
    digitalWrite(LED_PIN, HIGH);
   
  } else {
    // turn off a LED leveling
    digitalWrite(LED_PIN, LOW);
    
  }

  delay(2000); // Waits 2 seconds before the next reading
}
};

// End of file