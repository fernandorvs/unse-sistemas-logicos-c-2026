// Clase 5 - Ejercicio 1 (solución): contar bits en 1 y bit de paridad
// contarUnos(x) recorre los 8 bits con una máscara y cuenta los que valen 1.
// Paridad PAR: el bit de paridad se elige para que el total de unos sea par.
// Lo usamos para armar un byte = 7 bits de dato + bit de paridad en el bit 7 (L7).

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

// Cuenta cuántos bits valen 1
int contarUnos(uint8_t x) {
  int unos = 0;
  for (int i = 0; i < 8; i++) {
    if ((x >> i) & 1) {
      unos++;
    }
  }
  return unos;
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

  Serial.printf("Dato     unos  paridad  Byte enviado\n");
}

void loop() {
  // Probamos con datos de 7 bits (0 a 127)
  for (int dato = 0; dato < 128; dato = dato + 7) {
    int unos = contarUnos(dato);
    int paridad = unos % 2;                     // 1 si hay un número impar de unos
    uint8_t enviado = dato | (paridad << 7);    // el bit de paridad va en el bit 7

    imprimirBinario(dato);
    Serial.printf("   %d       %d     ", unos, paridad);
    imprimirBinario(enviado);
    Serial.printf("  (%d unos)\n", contarUnos(enviado));   // siempre par

    mostrarByte(enviado);
    delay(700);
  }
  Serial.printf("\n");
}
