// Clase 3 - Ejercicio 1 (solución): funciones de 3 entradas
//   L0 = MAYORÍA(A, B, C): vale 1 si 2 o más entradas están en 1
//        M = A·B + A·C + B·C
//   L1 = F = A·B + !C   (la función de la teoría)

#include <Arduino.h>

const int L0 = 4;
const int L1 = 16;
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

void setup() {
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);
  pinMode(BOTON_C, INPUT_PULLUP);
}

void loop() {
  bool a = !digitalRead(BOTON_A);   // apretado = 1
  bool b = !digitalRead(BOTON_B);
  bool c = !digitalRead(BOTON_C);

  // Suma de productos: el · es && y el + es ||
  bool mayoria = (a && b) || (a && c) || (b && c);
  bool f = (a && b) || !c;

  digitalWrite(L0, mayoria);
  digitalWrite(L1, f);

  Serial.printf("A=%d B=%d C=%d | Mayoria=%d  F=%d\n", a, b, c, mayoria, f);

  delay(500);
}
