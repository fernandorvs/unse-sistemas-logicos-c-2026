// Clase 4 - Ejercicio 2 (solución): verificar las leyes de De Morgan
//   Ley 1: !(A && B) == (!A || !B)
//   Ley 2: !(A || B) == (!A && !B)
// Se prueban TODAS las combinaciones con dos for anidados.

void setup() {
  Serial.begin(115200);

  int errores = 0;

  Serial.printf("Ley 1: NOT(A AND B) = (NOT A) OR (NOT B)\n");
  for (int a = 0; a <= 1; a++) {
    for (int b = 0; b <= 1; b++) {
      bool izquierda = !(a && b);
      bool derecha = !a || !b;
      if (izquierda == derecha) {
        Serial.printf("  A=%d B=%d -> %d  %d  OK\n", a, b, izquierda, derecha);
      } else {
        Serial.printf("  A=%d B=%d -> %d  %d  ERROR\n", a, b, izquierda, derecha);
        errores++;
      }
    }
  }

  Serial.printf("Ley 2: NOT(A OR B) = (NOT A) AND (NOT B)\n");
  for (int a = 0; a <= 1; a++) {
    for (int b = 0; b <= 1; b++) {
      bool izquierda = !(a || b);
      bool derecha = !a && !b;
      if (izquierda == derecha) {
        Serial.printf("  A=%d B=%d -> %d  %d  OK\n", a, b, izquierda, derecha);
      } else {
        Serial.printf("  A=%d B=%d -> %d  %d  ERROR\n", a, b, izquierda, derecha);
        errores++;
      }
    }
  }

  if (errores == 0) {
    Serial.printf("De Morgan se cumple en todas las combinaciones.\n");
  } else {
    Serial.printf("Hubo %d errores.\n", errores);
  }
}

void loop() {
}
