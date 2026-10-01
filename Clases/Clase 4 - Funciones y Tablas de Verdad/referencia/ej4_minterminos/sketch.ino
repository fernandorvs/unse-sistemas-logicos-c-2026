// Clase 4 - Ejercicio 4 (solución): mintérminos de una función
// Recorre las 8 filas de la tabla con UN solo for (fila 0..7),
// cuenta cuántas veces la salida vale 1 e imprime F = Σm(...).
// Función de ejemplo: la mayoría del Ejercicio 1.

bool funcionF(bool a, bool b, bool c) {
  return (a && b) || (a && c) || (b && c);
}

void setup() {
  Serial.begin(115200);

  Serial.printf("fila | A B C | F\n");
  Serial.printf("-----+-------+---\n");

  int unos = 0;
  for (int fila = 0; fila < 8; fila++) {
    // Sacamos los bits de la fila por divisiones (A es el de más peso)
    int a = (fila / 4) % 2;
    int b = (fila / 2) % 2;
    int c = fila % 2;
    bool f = funcionF(a, b, c);
    Serial.printf("  %d  | %d %d %d | %d\n", fila, a, b, c, f);
    if (f) {
      unos++;
    }
  }
  Serial.printf("La salida vale 1 en %d de las 8 filas.\n", unos);

  // Segunda pasada: imprimir la suma de mintérminos
  Serial.printf("F = Σm(");
  int impresos = 0;
  for (int fila = 0; fila < 8; fila++) {
    if (funcionF((fila / 4) % 2, (fila / 2) % 2, fila % 2)) {
      if (impresos > 0) {
        Serial.printf(",");       // coma ANTES de cada número, salvo el primero
      }
      Serial.printf("%d", fila);
      impresos++;
    }
  }
  Serial.printf(")\n");           // F = Σm(3,5,6,7)
}

void loop() {
}
