// Clase 8 - Ejercicio 1 (solución): semáforo con botón peatonal
//
// Autos:   L0 = rojo, L1 = "amarillo", L4 = verde
// Peatón:  L3 = rojo peatón, L7 = verde peatón, L2 = "ESPERE" (pedido registrado)
// Botón A: pulsador del peatón.
//
// Sin peatones, el verde de los autos dura 10 s.
// Si el peatón aprieta A, el verde se ACORTA: termina apenas se cumple el
// verde mínimo (3 s). El efecto del botón depende del estado Y de la entrada:
// por eso decimos que este semáforo tiene comportamiento de MEALY
// (la luz "ESPERE" responde a la entrada en el mismo instante, sin cambiar de estado).

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

const unsigned long VERDE_MINIMO = 3000;
const unsigned long VERDE_MAXIMO = 10000;
const unsigned long TIEMPO_AMARILLO = 2000;
const unsigned long TIEMPO_CRUCE = 5000;
const unsigned long TIEMPO_TITILA = 2000;

// Cada luz es un bit del byte que mandamos a mostrarByte
const uint8_t AUTO_ROJO = 0x01;       // L0
const uint8_t AUTO_AMARILLO = 0x02;   // L1
const uint8_t ESPERE = 0x04;          // L2
const uint8_t PEATON_ROJO = 0x08;     // L3
const uint8_t AUTO_VERDE = 0x10;      // L4
const uint8_t PEATON_VERDE = 0x80;    // L7

enum Estado { AUTOS_VERDE, AUTOS_AMARILLO, PEATON_CRUZA, PEATON_TITILA };
Estado estado = AUTOS_VERDE;
unsigned long inicioEstado = 0;
bool pedido = false;                  // ¿algún peatón apretó el botón?

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

void cambiarEstado(Estado nuevo) {
  estado = nuevo;
  inicioEstado = millis();
  switch (estado) {
    case AUTOS_VERDE:    Serial.printf("Autos: VERDE\n");               break;
    case AUTOS_AMARILLO: Serial.printf("Autos: AMARILLO\n");            break;
    case PEATON_CRUZA:   Serial.printf("Peaton: CRUCE\n");              break;
    case PEATON_TITILA:  Serial.printf("Peaton: cruzar rapido (titila)\n"); break;
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("Semaforo peatonal. A = boton del peaton.\n");
  cambiarEstado(AUTOS_VERDE);
}

void loop() {
  bool flancoA = seApreto(BOT_A);
  unsigned long enEstado = millis() - inicioEstado;

  switch (estado) {
    case AUTOS_VERDE:
      if (flancoA && !pedido) {
        pedido = true;
        Serial.printf("  Pedido peatonal registrado\n");
      }
      if (pedido) {
        mostrarByte(AUTO_VERDE | PEATON_ROJO | ESPERE);
      } else {
        mostrarByte(AUTO_VERDE | PEATON_ROJO);
      }
      // Transición: por tiempo máximo, o por pedido una vez cumplido el mínimo
      if (enEstado >= VERDE_MAXIMO || (pedido && enEstado >= VERDE_MINIMO)) {
        cambiarEstado(AUTOS_AMARILLO);
      }
      break;

    case AUTOS_AMARILLO:
      mostrarByte(AUTO_AMARILLO | PEATON_ROJO);
      if (enEstado >= TIEMPO_AMARILLO) {
        cambiarEstado(PEATON_CRUZA);
      }
      break;

    case PEATON_CRUZA:
      pedido = false;                               // el pedido ya se atendió
      mostrarByte(AUTO_ROJO | PEATON_VERDE);
      if (enEstado >= TIEMPO_CRUCE) {
        cambiarEstado(PEATON_TITILA);
      }
      break;

    case PEATON_TITILA:
      if ((enEstado / 250) % 2 == 0) {              // titila cada 250 ms
        mostrarByte(AUTO_ROJO | PEATON_VERDE);
      } else {
        mostrarByte(AUTO_ROJO);
      }
      if (enEstado >= TIEMPO_TITILA) {
        cambiarEstado(AUTOS_VERDE);
      }
      break;
  }
}
