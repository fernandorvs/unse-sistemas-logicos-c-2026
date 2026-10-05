// Clase 5 - Ejemplo 3: Barrido con desplazamientos
// Un solo bit en 1 que "camina" de L0 a L7 usando <<.
// Compararlo con el barrido de la Clase 4: aquí no hay digitalWrite en el loop,
// solo una variable que se desplaza y mostrarByte().

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
  uint8_t patron = 0b00000001;     // arranca con L0 encendido
  for (int i = 0; i < 8; i++) {
    mostrarByte(patron);
    imprimirBinario(patron);
    Serial.printf("  = %3u\n", patron);   // 1, 2, 4, 8, ... ¡se duplica!
    delay(200);
    patron = patron << 1;          // corre todos los bits un lugar a la izquierda
  }
  // Después de 8 desplazamientos el 1 "se cae" por la izquierda y patron vale 0
  Serial.printf("Despues de 8 shifts: %u\n\n", patron);
}
