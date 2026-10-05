// Clase 1 - Ejercicio 2 (solución): SOS en código Morse con L0
// S = 3 puntos (cortos), O = 3 rayas (largas).

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
}

void loop() {
  Serial.printf("S");
  digitalWrite(4, HIGH); delay(200); digitalWrite(4, LOW); delay(200);
  digitalWrite(4, HIGH); delay(200); digitalWrite(4, LOW); delay(200);
  digitalWrite(4, HIGH); delay(200); digitalWrite(4, LOW); delay(600);

  Serial.printf("O");
  digitalWrite(4, HIGH); delay(600); digitalWrite(4, LOW); delay(200);
  digitalWrite(4, HIGH); delay(600); digitalWrite(4, LOW); delay(200);
  digitalWrite(4, HIGH); delay(600); digitalWrite(4, LOW); delay(600);

  Serial.printf("S\n");
  digitalWrite(4, HIGH); delay(200); digitalWrite(4, LOW); delay(200);
  digitalWrite(4, HIGH); delay(200); digitalWrite(4, LOW); delay(200);
  digitalWrite(4, HIGH); delay(200); digitalWrite(4, LOW); delay(200);

  delay(2000);   // pausa larga antes de repetir
}
