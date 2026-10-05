// Clase 1 - Ejemplo 1: Hola mundo
// Imprime un mensaje en el monitor serie.

#include <Arduino.h>

void setup() {
  // setup() se ejecuta UNA sola vez, al encender el ESP32
  Serial.begin(115200);              // abre la comunicación con la PC
  Serial.printf("Hola, mundo!\n");   // \n = salto de línea
  Serial.printf("Soy un ESP32 y estoy aprendiendo C.\n");
}

void loop() {
  // loop() se repite para siempre. Por ahora no hace nada.
}
