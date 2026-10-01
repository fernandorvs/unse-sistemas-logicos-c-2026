// Clase 4 - Ejemplo 3: Tabla de verdad de la compuerta AND
// Dos bucles for anidados recorren todas las combinaciones de A y B.

bool compuertaAND(bool a, bool b) {
  return a && b;
}

void setup() {
  Serial.begin(115200);

  Serial.printf("Tabla de verdad: AND\n");
  Serial.printf(" A | B | S\n");
  Serial.printf("---+---+---\n");

  for (int a = 0; a <= 1; a++) {        // a vale 0 y después 1
    for (int b = 0; b <= 1; b++) {      // por CADA valor de a, b vale 0 y 1
      bool s = compuertaAND(a, b);
      Serial.printf(" %d | %d | %d\n", a, b, s);
    }
  }
}

void loop() {
  // La tabla se imprime una sola vez, en setup()
}
