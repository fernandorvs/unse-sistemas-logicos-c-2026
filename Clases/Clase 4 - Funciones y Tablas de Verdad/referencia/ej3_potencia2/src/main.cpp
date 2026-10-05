// Clase 4 - Ejercicio 3 (solución): función potencia2(n)
// Devuelve 2 elevado a la n, multiplicando por 2 n veces.
// Son los PESOS de cada bit: L0 pesa 1, L1 pesa 2, L2 pesa 4, ...

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

int potencia2(int n) {
  int resultado = 1;              // 2 elevado a la 0 = 1
  for (int i = 0; i < n; i++) {
    resultado = resultado * 2;
  }
  return resultado;
}

// Versión de mostrarNumero que usa los pesos: bit i = (n / 2^i) % 2
void mostrarNumero(int n) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], (n / potencia2(i)) % 2);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

  Serial.printf("Pesos de cada bit:\n");
  for (int i = 0; i < 8; i++) {
    Serial.printf("  L%d pesa 2^%d = %d\n", i, i, potencia2(i));
  }

  // Bonus: probar mostrarNumero con pesos
  mostrarNumero(10);              // 1010 -> L3 y L1 encendidos
  Serial.printf("Mostrando 10 en L0..L3 (1010)\n");
}

void loop() {
}
