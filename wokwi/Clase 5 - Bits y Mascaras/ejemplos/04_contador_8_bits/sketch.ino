// Clase 5 - Ejemplo 4: Contador de 0 a 255 en los 8 LEDs
// La variable es un uint8_t: al pasar de 255 vuelve sola a 0 (desborde, Clase 2).

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

uint8_t contador = 0;   // global: conserva su valor entre vueltas de loop()

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  mostrarByte(contador);
  imprimirBinario(contador);
  Serial.printf("  0x%02X  %3u\n", contador, contador);
  contador++;              // 255 + 1 = 0 en un uint8_t
  delay(100);

  // ⚠️ Ojo: un for así NUNCA termina, porque un uint8_t siempre es <= 255:
  //   for (uint8_t n = 0; n <= 255; n++) { ... }
  // Para recorrer 0..255 con un for, usar int:
  //   for (int n = 0; n <= 255; n++) { ... }
}
