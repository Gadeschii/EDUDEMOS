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
 
//Prueba funcionamiento placa solar
#define ledPin1Cold 5 // Definimos el pin donde está conectado el LED Azul 
#define detectorPin 36 // Definimos el pin donde está conectado el Detector de 
Tensión 
// Bloque de inicialización 
void setup() { 
// Configuramos el modo de funcionamiento del pin del LED como salida 
pinMode(ledPin1Cold, OUTPUT); 
} 
// Bucle principal del código 
void loop() { 
// Tomamos la lectura del Módulo Detector 
bool tension_detectada = digitalRead(detectorPin);
// Comprobamos si hay tensión o no 
if(tension_detectada) { // Sí hay tensión 
digitalWrite(ledPin1Cold, HIGH); // Encendemos el LED Azul 
} else { // No hay tensión 
digitalWrite(ledPin1Cold, LOW); // Apagamos el LED Azul 
} 
}