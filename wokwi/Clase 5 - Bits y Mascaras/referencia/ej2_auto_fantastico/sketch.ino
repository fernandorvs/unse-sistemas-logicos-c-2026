// Clase 5 - Ejercicio 2 (solución): efecto "auto fantástico"
// Un LED encendido va de L0 a L7 con << y vuelve de L7 a L0 con >>.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  Serial.printf("Auto fantastico\n");
}

void loop() {
  uint8_t luz = 0b00000001;   // L0

  // Ida: 7 pasos de L0 a L7
  for (int i = 0; i < 7; i++) {
    mostrarByte(luz);
    delay(80);
    luz <<= 1;                // igual que luz = luz << 1
  }

  // Vuelta: 7 pasos de L7 a L0
  for (int i = 0; i < 7; i++) {
    mostrarByte(luz);
    delay(80);
    luz >>= 1;                // igual que luz = luz >> 1
  }
  // 7 + 7 pasos: así L0 y L7 no se repiten dos veces seguidas
}
