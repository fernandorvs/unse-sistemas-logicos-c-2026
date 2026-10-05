// Clase 5 - Ejercicio 4 (solución): separar un byte en sus dos nibbles
//   nibble bajo = bits 0..3 -> máscara 0x0F (00001111)
//   nibble alto = bits 4..7 -> primero >> 4, después máscara 0x0F
// Cada nibble es un dígito hexadecimal.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

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
  uint8_t x = 0xB1;                        // 1011 0001

  uint8_t bajo = x & 0x0F;                 // 0000 0001 = 0x1
  uint8_t alto = (x >> 4) & 0x0F;          // 0000 1011 = 0xB
  uint8_t soloAlto = x & 0xF0;             // 1011 0000: el alto, en su lugar

  Serial.printf("x          = "); imprimirBinario(x);        Serial.printf("  0x%02X\n", x);
  Serial.printf("x & 0x0F   = "); imprimirBinario(bajo);     Serial.printf("  nibble bajo = %X\n", bajo);
  Serial.printf("(x>>4)&0x0F= "); imprimirBinario(alto);     Serial.printf("  nibble alto = %X\n", alto);
  Serial.printf("x & 0xF0   = "); imprimirBinario(soloAlto); Serial.printf("\n\n");

  // En los LEDs: primero el byte entero, después cada nibble (en L0..L3)
  mostrarByte(x);
  delay(1500);
  mostrarByte(bajo);
  delay(1500);
  mostrarByte(alto);
  delay(1500);
}
