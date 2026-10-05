// Clase 7 - Ejercicio 3 (solución): contador módulo 10 (BCD)
// A = +1, B = -1, C = reset (flancos con antirrebote).
// Cuenta 0, 1, ..., 9, 0, 1, ... y se ve en binario en L0..L3.
// Cuando pasa de 9 a 0 genera un "acarreo": L7 se invierte (como el reloj
// de un segundo contador BCD, el de las decenas).

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

// ---------- Detección de flancos con antirrebote (para los 3 botones) ----------
const int CANT_BOTONES = 3;
const int BOTONES[CANT_BOTONES] = {BOTON_A, BOTON_B, BOTON_C};  // 0 = A, 1 = B, 2 = C
const unsigned long ANTIRREBOTE_MS = 50;

bool apretadoAntes[CANT_BOTONES] = {false, false, false};  // memoria de cada botón
unsigned long ultimoCambio[CANT_BOTONES] = {0, 0, 0};      // cuándo cambió por última vez

// Devuelve true UNA sola vez por cada pulsación del botón (0 = A, 1 = B, 2 = C).
bool seApreto(int boton) {
  bool apretadoAhora = (digitalRead(BOTONES[boton]) == LOW);
  bool hayFlanco = false;

  if (apretadoAhora != apretadoAntes[boton]) {                 // ¿cambió?
    if (millis() - ultimoCambio[boton] >= ANTIRREBOTE_MS) {    // ¿y no es un rebote?
      ultimoCambio[boton] = millis();
      apretadoAntes[boton] = apretadoAhora;
      if (apretadoAhora) {
        hayFlanco = true;                                      // cambió de suelto a apretado
      }
    }
  }
  return hayFlanco;
}
// --------------------------------------------------------------------------------

int cuenta = 0;          // 0..9
bool acarreo = false;    // se invierte en cada vuelta 9 -> 0

void mostrar() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], (cuenta >> i) & 1);
  }
  digitalWrite(LEDS[7], acarreo);
  Serial.printf("Cuenta BCD: %d (%d%d%d%d)\n", cuenta,
                (cuenta >> 3) & 1, (cuenta >> 2) & 1, (cuenta >> 1) & 1, cuenta & 1);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("Contador BCD: A = +1, B = -1, C = reset\n");
  mostrar();
}

void loop() {
  if (seApreto(0)) {
    cuenta++;
    if (cuenta > 9) {          // después de 9 vuelve a 0 (nunca llega a 10)
      cuenta = 0;
      acarreo = !acarreo;
      Serial.printf("Acarreo!\n");
    }
    mostrar();
  }

  if (seApreto(1)) {
    cuenta--;
    if (cuenta < 0) {
      cuenta = 9;
    }
    mostrar();
  }

  if (seApreto(2)) {
    cuenta = 0;
    acarreo = false;
    mostrar();
  }
}
