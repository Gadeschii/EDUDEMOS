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
 
//Prueba funcionamiento sensor de Humedad y Temperatura
#include <DHT.h> // Incluimos la librería necesaria para controlar de DHT 
#define ledPin1Cold 5 // Definimos el pin donde está conectado el LED Azul 
#define ledPin2Good 18 // Definimos el pin donde está conectado el LED Verde 
#define ledPin3Heat 19 // Definimos el pin donde está conectado el LED Rojo 
#define DHTPIN 32 // Definimos el pin donde está conectado el DHT 
#define DHTTYPE DHT11 // Definimos el tipo de sensor DHT que vamos a utilizar 
// Creamos un objeto de tipo DHT con la configuración correspondiente 
DHT myDHT11(DHTPIN, DHTTYPE); 
// Bloque de inicialización 
void setup() { 
// Configuramos el modo de funcionamiento de los pines de los LED como salida 
pinMode(ledPin1Cold, OUTPUT); 
pinMode(ledPin2Good, OUTPUT); 
pinMode(ledPin3Heat, OUTPUT); 
// Inicializamos el monitor Serial 
Serial.begin(115200); 
// Inicializamos el sensor DHT 
myDHT11.begin(); 
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
//Obtenemos las medidas del sensor DHT 
float temperatura = myDHT11.readTemperature(); 
float humedad = myDHT11.readHumidity(); 
// Mostramos los valores por monitor Serial 
Serial.print("Temperatura: "); 
Serial.println(temperatura); 
Serial.print("Humedad: "); 
Serial.println(humedad); 
}
