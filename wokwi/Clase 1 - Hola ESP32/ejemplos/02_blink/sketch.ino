// Clase 1 - Ejemplo 2: Blink
// Hace parpadear el LED L0 (GPIO 4) una vez por segundo.

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);                // el pin 4 va a ser una SALIDA
  Serial.printf("Arranca el blink\n");
}

void loop() {
  digitalWrite(4, HIGH);             // L0 encendido (3,3 V en el pin)
  Serial.printf("LED encendido\n");
  delay(500);                        // esperar 500 ms

  digitalWrite(4, LOW);              // L0 apagado (0 V en el pin)
  Serial.printf("LED apagado\n");
  delay(500);
}
