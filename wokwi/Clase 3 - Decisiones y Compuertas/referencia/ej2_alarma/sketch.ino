// Clase 3 - Ejercicio 2 (solución): alarma de una casa
//   B mantenido = sistema ARMADO (en una casa real sería una llave)
//   A apretado  = PUERTA ABIERTA (sensor de la puerta)
//   C apretado  = botón de PÁNICO (dispara la alarma siempre)
// Salidas:
//   L4 (verde)  = sistema armado
//   L0..L3 (rojos) parpadean cuando suena la alarma

#include <Arduino.h>

const int L0 = 4;
const int L1 = 16;
const int L2 = 17;
const int L3 = 18;
const int L4 = 19;
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

void setup() {
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);
  pinMode(BOTON_C, INPUT_PULLUP);
}

void loop() {
  bool puertaAbierta = digitalRead(BOTON_A) == LOW;
  bool armada        = digitalRead(BOTON_B) == LOW;
  bool panico        = digitalRead(BOTON_C) == LOW;

  digitalWrite(L4, armada);

  if (panico || (armada && puertaAbierta)) {
    if (panico) {
      Serial.printf("*** ALARMA: boton de panico ***\n");
    } else {
      Serial.printf("*** ALARMA: puerta abierta ***\n");
    }
    // Parpadeo de los 4 LEDs rojos
    digitalWrite(L0, HIGH);
    digitalWrite(L1, HIGH);
    digitalWrite(L2, HIGH);
    digitalWrite(L3, HIGH);
    delay(150);
    digitalWrite(L0, LOW);
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(L3, LOW);
    delay(150);
  } else if (armada) {
    Serial.printf("Armada. Todo tranquilo.\n");
    delay(300);
  } else {
    Serial.printf("Desarmada.\n");
    delay(300);
  }
}
