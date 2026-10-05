// Apellido, Nombre - Clase N
// Qué hace este programa: ...
//
// Plantilla de proyecto: por ahora hace parpadear el LED L0 (GPIO 4).

#include <Arduino.h>   // va siempre, en la primera línea del programa

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  Serial.printf("Arranca el programa\n");
}

void loop() {
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
  delay(500);
}
