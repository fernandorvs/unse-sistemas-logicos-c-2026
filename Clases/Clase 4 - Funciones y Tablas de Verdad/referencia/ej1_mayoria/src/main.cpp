// Clase 4 - Ejercicio 1 (solución): función mayoría de 3 entradas
// La salida vale 1 cuando al menos dos de las tres entradas valen 1.
// Expresión: M = A·B + A·C + B·C

#include <Arduino.h>

bool mayoria(bool a, bool b, bool c) {
  return (a && b) || (a && c) || (b && c);
}

void setup() {
  Serial.begin(115200);

  Serial.printf("Tabla de verdad: MAYORIA\n");
  Serial.printf(" A | B | C | M\n");
  Serial.printf("---+---+---+---\n");

  // Tres bucles anidados: 2 x 2 x 2 = 8 filas
  for (int a = 0; a <= 1; a++) {
    for (int b = 0; b <= 1; b++) {
      for (int c = 0; c <= 1; c++) {
        Serial.printf(" %d | %d | %d | %d\n", a, b, c, mayoria(a, b, c));
      }
    }
  }
}

void loop() {
}
