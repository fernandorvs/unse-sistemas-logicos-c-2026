// Clase 4 - Ejemplo 1: Barrido con un bucle for
// El mismo barrido del Ejercicio 3 de la Clase 1, pero en 5 líneas.
// Ahora recorre los 8 LEDs: L0 -> L1 -> ... -> L7 -> L0 ...

#include <Arduino.h>

// Lista (arreglo) con los pines de los 8 LEDs.
// LEDS[0] es L0 (GPIO 4), LEDS[1] es L1 (GPIO 16), ... LEDS[7] es L7 (GPIO 23).
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

void setup() {
  Serial.begin(115200);

  // Un solo for configura los 8 pines como salida
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  Serial.printf("Barrido con for\n");
}

void loop() {
  // i arranca en 0, se repite mientras i < 8, y en cada vuelta suma 1
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], HIGH);
    delay(150);
    digitalWrite(LEDS[i], LOW);
  }
}
