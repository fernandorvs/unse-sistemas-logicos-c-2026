// Clase 7 - Ejemplo 3: Flip-flops D y T en software, con antirrebote
//  - Flip-flop D en L0: A es el DATO, B es el RELOJ.
//    En cada flanco de B, L0 copia lo que vale A en ese instante.
//  - Flip-flop T en L7: C es el RELOJ (con T = 1).
//    En cada flanco de C, L7 se invierte.

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

bool qD = false;   // salida del flip-flop D (memoria)
bool qT = false;   // salida del flip-flop T (memoria)

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("FF D: A = dato, B = reloj (L0).  FF T: C = reloj (L7)\n");
}

void loop() {
  // Flip-flop D: en el flanco de B se copia el dato A
  if (seApreto(1)) {
    qD = (digitalRead(BOTON_A) == LOW);   // el dato se lee como NIVEL
    Serial.printf("Flanco de reloj B: D = %d -> Q = %d\n", qD, qD);
  }

  // Flip-flop T: en el flanco de C, Q se invierte
  if (seApreto(2)) {
    qT = !qT;
    Serial.printf("Flanco de reloj C: Q del T = %d\n", qT);
  }

  digitalWrite(LEDS[0], qD);
  digitalWrite(LEDS[7], qT);
}
