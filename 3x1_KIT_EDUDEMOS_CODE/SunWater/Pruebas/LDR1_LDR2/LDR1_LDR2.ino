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
 
//Prueba funcionamiento LDR1 Y LDR2
#define ldrPin1 34 // Definimos el pin donde está conectado la primera LDR 
#define ldrPin2 35 // Definimos el pin donde está conectado la segunda LDR 
// Bloque de inicialización 
void setup() { 
// Inicializamos el monitor serial 
Serial.begin(115200); 
} 
// Bucle principal del código 
void loop() { 
// Tomamos las lecturas de los pines donde se encuentran las LDR 
int lectura_ldr_1 = analogRead(ldrPin1); 
int lectura_ldr_2 = analogRead(ldrPin2); 
// Mostramos el primer valor por el monitor Serial 
Serial.print("Luminosidad 1: "); 
Serial.println(lectura_ldr_1); 
// Mostramos el segundo valor por el monitor Serial 
Serial.print("Luminosidad 2: "); 
Serial.println(lectura_ldr_2); 
}
