// Clase 6 - Ejemplo 4: Multiplexor 4 -> 1
// Datos D0..D3: un arreglo de 4 bits, que se ven en L0..L3.
// Selector: pulsadores A (S0) y B (S1).  Salida Y: L7.
// La salida copia el dato elegido por el selector: Y = D[sel].

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;

bool datos[4] = {0, 1, 1, 0};   // D0, D1, D2, D3: cambiarlos y probar

int selAnterior = -1;           // para imprimir solo cuando cambia el selector

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  // Mostrar los datos de entrada en L0..L3
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], datos[i]);
  }
  Serial.printf("MUX 4->1. Datos: D0=%d D1=%d D2=%d D3=%d\n", datos[0], datos[1], datos[2], datos[3]);
}

void loop() {
  bool a = (digitalRead(BOTON_A) == LOW);
  bool b = (digitalRead(BOTON_B) == LOW);

  int sel = b * 2 + a;            // también vale: (b << 1) | a
  bool y = datos[sel];            // ¡el multiplexor es esta línea!
  digitalWrite(LEDS[7], y);

  if (sel != selAnterior) {
    Serial.printf("sel = %d (B=%d A=%d) -> Y = D%d = %d\n", sel, b, a, sel, y);
    selAnterior = sel;
  }
}
