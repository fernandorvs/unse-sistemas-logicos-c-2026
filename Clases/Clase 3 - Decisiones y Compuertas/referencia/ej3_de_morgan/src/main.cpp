// Clase 3 - Ejercicio 3 (solución): comprobar las leyes de De Morgan
//   1) !(A && B) == !A || !B      (NAND = OR de las negadas)
//   2) !(A || B) == !A && !B      (NOR  = AND de las negadas)
// Probar las 4 combinaciones de A y B apretando los pulsadores.
// L0 y L1 muestran los dos lados de la ley 1; L2 y L3, los de la ley 2.
// Si la ley se cumple, los LEDs de cada par siempre están iguales.

#include <Arduino.h>

const int L0 = 4;
const int L1 = 16;
const int L2 = 17;
const int L3 = 18;
const int BOTON_A = 32;
const int BOTON_B = 33;

void setup() {
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);
}

void loop() {
  bool a = !digitalRead(BOTON_A);
  bool b = !digitalRead(BOTON_B);

  bool izq1 = !(a && b);
  bool der1 = !a || !b;
  bool izq2 = !(a || b);
  bool der2 = !a && !b;

  digitalWrite(L0, izq1);
  digitalWrite(L1, der1);
  digitalWrite(L2, izq2);
  digitalWrite(L3, der2);

  Serial.printf("A=%d B=%d | !(A&&B)=%d  !A||!B=%d  ", a, b, izq1, der1);
  if (izq1 == der1) {
    Serial.printf("OK");
  } else {
    Serial.printf("NO COINCIDEN");
  }

  Serial.printf("  |  !(A||B)=%d  !A&&!B=%d  ", izq2, der2);
  if (izq2 == der2) {
    Serial.printf("OK\n");
  } else {
    Serial.printf("NO COINCIDEN\n");
  }

  delay(500);
}
