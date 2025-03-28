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
 
//Prueba funcionamiento del servo motor
#include <ESP32Servo.h> // Incluir la librería necesaria para controlar el servomotor 
#define servoPin 25 // Definimos el pin donde está conectado el servomotor 
// Creamos un objeto de tipo Servo para controlarlo 
Servo myServo; 
// Bloque de inicialización 
void setup() { 
// Asociamos el servo que creamos al pin donde está conectado 
myServo.attach(servoPin); 
// Colocamos el servo en una posición inicial de 0º 
myServo.write(0); 
} 
// Bucle principal del código 
void loop() { 
myServo.write(0); // Movemos el servomotor a 0º 
delay(1000); // Esperamos 1 segundo = 1000 milisegundos 
myServo.write(90); // Movemos el servomotor a 90º 
delay(1000); // Esperamos 1 segundo 
myServo.write(180); // Movemos el servomotor a 180º 
delay(1000); // Esperamos 1 segundo 
} 
