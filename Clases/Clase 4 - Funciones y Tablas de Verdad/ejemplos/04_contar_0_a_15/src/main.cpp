// Clase 4 - Ejemplo 4: Contar de 0 a 15 en binario con un for
// La función mostrarNumero(n) muestra n (0..15) en L0..L3
// usando divisiones sucesivas (como en la Clase 2), pero dentro de un bucle.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

// Muestra n en binario en L0 (bit de menos peso) ... L3
void mostrarNumero(int n) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], n % 2);   // el resto es el bit i
    n = n / 2;                      // pasamos al bit siguiente
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  for (int n = 0; n <= 15; n++) {   // <= 15 para que el 15 también se muestre
    mostrarNumero(n);
    Serial.printf("n = %2d  (hexa %X)\n", n, n);
    delay(400);
  }
  Serial.printf("--- vuelta completa ---\n");
}
