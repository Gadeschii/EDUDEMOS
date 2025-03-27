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
 
//Testing that the water level sensor and the code works
const int WaterSensorPin = 39; // Analog Value from VN PIN

void setup() {
Serial.begin(115200);
}
void loop() {
  int sensorValue; // Variable to store sensor values
  sensorValue = analogRead(WaterSensorPin); // Read the value from the soil moisture sensor
  //Check watersensor calibration first
  if (sensorValue >= 2000) { //These value represents de max value given 
    sensorValue = 1980; //This value would return a 99% value
  }
  float waterlevel = (sensorValue*100)/2000 ; //Calculation to return the %

  Serial.println(waterlevel); // Print value in serial monitor
  delay(100);
}

