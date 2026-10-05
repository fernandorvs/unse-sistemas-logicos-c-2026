// Clase 2 - Ejemplo 4: Desborde de un uint8_t
// Un uint8_t tiene 8 bits: solo puede guardar de 0 a 255.
// Si vale 255 y le sumamos 1, vuelve a 0 (como un cuentakilómetros).

#include <Arduino.h>

uint8_t contador = 250;   // declarada AFUERA de setup() y loop(): no se borra

void setup() {
  Serial.begin(115200);
  Serial.printf("Contando con un uint8_t desde 250...\n");
}

void loop() {
  Serial.printf("Decimal: %3u   Hexa: %02X\n", contador, contador);
  contador++;             // 250, 251, ..., 255, 0, 1, 2 ...
  delay(500);
}
