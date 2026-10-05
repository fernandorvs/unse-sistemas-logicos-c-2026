// Clase 7 - Ejercicio 1 (solución): flip-flops JK y T
// Reloj: pulsador C (flanco al apretar, con antirrebote).
//  - JK en L0 (Q) y L1 (Q negada): A = J, B = K (se leen como NIVEL en el flanco de C).
//      J K | Q siguiente
//      0 0 | Q        (mantiene)
//      0 1 | 0        (reset)
//      1 0 | 1        (set)
//      1 1 | !Q       (invierte)
//  - T en L7: T = A. En el flanco de C, si T = 1 invierte, si T = 0 mantiene.
//    (Un T es un JK con J = K = T.)

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

bool qJK = false;   // memoria del JK
bool qT = false;    // memoria del T

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("JK: J = A, K = B, reloj = C (L0 = Q, L1 = !Q).  T: T = A (L7)\n");
}

void loop() {
  if (seApreto(2)) {                          // flanco de reloj
    bool j = (digitalRead(BOTON_A) == LOW);
    bool k = (digitalRead(BOTON_B) == LOW);

    // Flip-flop JK
    if (j && !k) {
      qJK = true;
    } else if (!j && k) {
      qJK = false;
    } else if (j && k) {
      qJK = !qJK;
    }
    // si j y k son 0, no se hace nada: mantiene

    // Flip-flop T (con T = A)
    bool t = j;
    if (t) {
      qT = !qT;
    }

    Serial.printf("Reloj: J=%d K=%d -> Q=%d  |  T=%d -> Q=%d\n", j, k, qJK, t, qT);
  }

  digitalWrite(LEDS[0], qJK);
  digitalWrite(LEDS[1], !qJK);
  digitalWrite(LEDS[7], qT);
}
