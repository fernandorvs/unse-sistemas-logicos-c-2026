// Clase 6 - Ejercicio 1 (solución): decodificador BCD -> 7 segmentos con "E" de error
// El diagram.json de este proyecto ya tiene el display de 7 segmentos.
// BCD solo admite 0..9. Para las entradas 10..15 (códigos no válidos) muestra "E".
// Notar que NO hace falta ningún if: la decisión está guardada en la tabla.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

const uint8_t BCD_A_7SEG[16] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111,  // 9
  0b01111001,  // 10: no es BCD -> E
  0b01111001,  // 11: E
  0b01111001,  // 12: E
  0b01111001,  // 13: E
  0b01111001,  // 14: E
  0b01111001   // 15: E
};

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
}

void loop() {
  for (int n = 0; n < 16; n++) {
    mostrarByte(BCD_A_7SEG[n]);
    if (n <= 9) {
      Serial.printf("Entrada %2d -> digito %d\n", n, n);
    } else {
      Serial.printf("Entrada %2d -> E (no es BCD)\n", n);
    }
    delay(800);
  }
}
