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
 
//Prueba funcionamiento LEDS Azul, verde y rojo
#define ledPin1Cold 5 // Definimos el pin donde está conectado el LED Azul 
#define ledPin2Good 18 // Definimos el pin donde está conectado el LED Verde 
#define ledPin3Heat 19 // Definimos el pin donde está conectado el LED Rojo 
// Bloque de inicialización 
void setup() { 
// Configuramos el modo de funcionamiento de los pines de los LED como salida 
pinMode(ledPin1Cold, OUTPUT); 
pinMode(ledPin2Good, OUTPUT); 
pinMode(ledPin3Heat, OUTPUT); 
} 
// Bucle principal del código 
void loop() {
digitalWrite(ledPin1Cold, LOW); // Apagamos el LED Azul 
digitalWrite(ledPin2Good, LOW); // Apagamos el LED Verde 
digitalWrite(ledPin3Heat, LOW); // Apagamos el LED Rojo 
delay(1000); // Esperamos 1 segundo = 1000 milisegundos 
digitalWrite(ledPin1Cold, HIGH); // Encendemos el LED Azul 
digitalWrite(ledPin2Good, HIGH); // Encendemos el LED Verde 
digitalWrite(ledPin3Heat, HIGH); // Encendemos el LED Rojo 
delay(1000); // Esperamos 1 segundo 
}
