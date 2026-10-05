// Clase 8 - Ejercicio 2 (solución): detector de la secuencia 1011
// Entrada bit a bit:  A = 0,  B = 1.
// Máquina de MOORE con 5 estados; detecta secuencias superpuestas
// (por ejemplo 1011011 detecta dos veces).
//
// Salidas:
//   L0        = 1 cuando se detectó 1011 (salida Moore)
//   L4 ... L7 = los últimos 4 bits ingresados (L4 = el más nuevo)
//
// Tabla de transiciones:
//   Estado   | entra 0 | entra 1 | salida
//   ---------+---------+---------+-------
//   S_NADA   | S_NADA  | S_1     |   0
//   S_1      | S_10    | S_1     |   0
//   S_10     | S_NADA  | S_101   |   0
//   S_101    | S_10    | S_1011  |   0
//   S_1011   | S_10    | S_1     |   1

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

enum Estado { S_NADA, S_1, S_10, S_101, S_1011 };
Estado estado = S_NADA;
uint8_t historial = 0;              // últimos 4 bits, solo para mostrar

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

// Clase 8: nombres para los índices de BOTONES, en vez de 0, 1 y 2
enum Boton { BOT_A, BOT_B, BOT_C };

// Muestra un byte en los 8 LEDs (Clase 5)
void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void imprimirEstado(Estado e) {
  switch (e) {
    case S_NADA: Serial.printf("S_NADA\n");                  break;
    case S_1:    Serial.printf("S_1\n");                     break;
    case S_10:   Serial.printf("S_10\n");                    break;
    case S_101:  Serial.printf("S_101\n");                   break;
    case S_1011: Serial.printf("S_1011  <-- DETECTADO!\n");  break;
  }
}

// Calcula el estado siguiente: es la tabla de transiciones escrita en C
Estado siguienteEstado(Estado actual, int bit) {
  switch (actual) {
    case S_NADA:
      if (bit == 1) return S_1;
      return S_NADA;
    case S_1:
      if (bit == 1) return S_1;
      return S_10;
    case S_10:
      if (bit == 1) return S_101;
      return S_NADA;
    case S_101:
      if (bit == 1) return S_1011;
      return S_10;                  // ...1010: lo último útil es "10"
    case S_1011:
      if (bit == 1) return S_1;     // ...10111: lo último útil es "1"
      return S_10;                  // ...10110: lo último útil es "10"
  }
  return S_NADA;                    // no debería llegar aquí nunca
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("Detector de 1011. A = 0, B = 1.\n");
  imprimirEstado(estado);
}

void loop() {
  bool flancoA = seApreto(BOT_A);
  bool flancoB = seApreto(BOT_B);

  if (flancoA || flancoB) {
    int bit = 0;
    if (flancoB) {
      bit = 1;
    }
    historial = ((historial << 1) | bit) & 0x0F;   // registro de desplazamiento (Clase 7)
    estado = siguienteEstado(estado, bit);
    Serial.printf("bit %d -> ", bit);
    imprimirEstado(estado);
  }

  // Salida Moore + historial
  uint8_t salida = historial << 4;
  if (estado == S_1011) {
    salida = salida | 0x01;
  }
  mostrarByte(salida);
}
