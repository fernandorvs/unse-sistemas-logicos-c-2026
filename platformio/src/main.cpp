// Clase 1 - Blink (versión PlatformIO)
// Hace parpadear el LED L0 (GPIO 4) una vez por segundo.

#include <Arduino.h>   // en PlatformIO esta línea es obligatoria

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  Serial.printf("Arranca el blink\n");
}

void loop() {
  digitalWrite(4, HIGH);
  Serial.printf("LED encendido\n");
  delay(500);

  digitalWrite(4, LOW);
  Serial.printf("LED apagado\n");
  delay(500);
}
