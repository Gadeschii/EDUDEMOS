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
 
//Prueba funcionamiento del motor DC

// Bloque de inicialización 
void setup() { 
// Inicializamos el Monitor Serial 
Serial.begin(115200); 
} 
// Bucle principal del código 
void loop() { 
// Tomamos la medida del pin que va a la salida del partidor de tensión 
int medicion = analogRead(39); 
// Mostramos la medida por el monitor serial 
Serial.println(medicion); 
}
