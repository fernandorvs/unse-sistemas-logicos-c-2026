// Clase 3 - Ejemplo 3: El ESP32 como compuertas lógicas
// Entradas: pulsadores A y B (apretado = 1).
// Salidas:  L0 = A AND B,  L1 = A OR B,  L2 = NOT A,  L3 = A NAND B

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
  // Apretado = LOW, así que "apretado" es lo mismo que "lectura == LOW"
  bool a = digitalRead(BOTON_A) == LOW;
  bool b = digitalRead(BOTON_B) == LOW;

  // Un bool vale 0 o 1: se lo podemos pasar directo a digitalWrite
  digitalWrite(L0, a && b);      // AND
  digitalWrite(L1, a || b);      // OR
  digitalWrite(L2, !a);          // NOT
  digitalWrite(L3, !(a && b));   // NAND

  Serial.printf("A=%d B=%d | AND=%d OR=%d NOT A=%d NAND=%d\n",
                a, b, a && b, a || b, !a, !(a && b));

  delay(300);
}
