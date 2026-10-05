// Clase 1 - Ejercicio 3 (solución): barrido de 4 LEDs
// Se enciende un LED por vez: L0 -> L1 -> L2 -> L3 -> L0 ...
// Notar cuánto se repite el código: en la Clase 4 se resuelve con un bucle.

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);    // L0
  pinMode(16, OUTPUT);   // L1
  pinMode(17, OUTPUT);   // L2
  pinMode(18, OUTPUT);   // L3
}

void loop() {
  digitalWrite(4, HIGH);  delay(200); digitalWrite(4, LOW);
  digitalWrite(16, HIGH); delay(200); digitalWrite(16, LOW);
  digitalWrite(17, HIGH); delay(200); digitalWrite(17, LOW);
  digitalWrite(18, HIGH); delay(200); digitalWrite(18, LOW);
}
