// Clase 6 - Ejemplo 2: Decodificador hexa -> 7 segmentos con una tabla
// El diagram.json de este proyecto ya tiene el display de 7 segmentos (cátodo común).
// Los segmentos están conectados a los LEDs: a=L0, b=L1, ..., g=L6, dp=L7.
// Muestra los dígitos 0, 1, ..., 9, A, b, C, d, E, F, uno por segundo.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

// La TABLA del decodificador: la posición es la entrada, el contenido la salida.
// Cada byte es  0b0gfedcba  (bit 0 = segmento a, ..., bit 6 = segmento g).
//
//      aaa
//     f   b
//      ggg
//     e   c
//      ddd
//
const uint8_t SEGMENTOS[16] = {
  0b00111111,  // 0: a b c d e f
  0b00000110,  // 1: b c
  0b01011011,  // 2: a b d e g
  0b01001111,  // 3: a b c d g
  0b01100110,  // 4: b c f g
  0b01101101,  // 5: a c d f g
  0b01111101,  // 6: a c d e f g
  0b00000111,  // 7: a b c
  0b01111111,  // 8: todos
  0b01101111,  // 9: a b c d f g
  0b01110111,  // A: a b c e f g
  0b01111100,  // b: c d e f g
  0b00111001,  // C: a d e f
  0b01011110,  // d: b c d e g
  0b01111001,  // E: a d e f g
  0b01110001   // F: a e f g
};

// Muestra un byte en los 8 LEDs (de la Clase 5)
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
  Serial.printf("Decodificador hexa -> 7 segmentos\n");
}

void loop() {
  for (int n = 0; n < 16; n++) {
    uint8_t patron = SEGMENTOS[n];   // ¡el decodificador entero es esta línea!
    mostrarByte(patron);
    Serial.printf("Entrada %2d (0x%X) -> segmentos 0x%02X\n", n, n, patron);
    delay(1000);
  }
}
