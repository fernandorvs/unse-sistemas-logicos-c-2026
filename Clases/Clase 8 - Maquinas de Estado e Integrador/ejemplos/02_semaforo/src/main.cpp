// Clase 8 - Ejemplo 2: Semáforo (máquina de MOORE temporizada)
// Tres estados: VERDE -> AMARILLO -> ROJO -> VERDE ...
// Las salidas dependen SOLO del estado (Moore).
// Las transiciones ocurren cuando pasa el tiempo (millis, sin delay).
//
// LEDs de la placa (no hay LED amarillo: lo representamos con L1):
//   L0 (rojo)  = luz ROJA
//   L1 (rojo)  = luz "AMARILLA"
//   L4 (verde) = luz VERDE

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int LUZ_ROJA = LEDS[0];
const int LUZ_AMARILLA = LEDS[1];
const int LUZ_VERDE = LEDS[4];

// Cuánto dura cada estado (en milisegundos)
const unsigned long TIEMPO_VERDE = 5000;
const unsigned long TIEMPO_AMARILLO = 2000;
const unsigned long TIEMPO_ROJO = 5000;

// ---------- Paso 1: los estados del diagrama, con nombre ----------
enum Estado { VERDE, AMARILLO, ROJO };

// ---------- Paso 2: la variable que recuerda el estado ----------
Estado estado = VERDE;              // estado inicial (la flecha "inicio" del diagrama)
unsigned long inicioEstado = 0;     // cuándo entramos al estado actual

// Enciende las tres luces según lo que se pida
void luces(bool roja, bool amarilla, bool verde) {
  digitalWrite(LUZ_ROJA, roja);
  digitalWrite(LUZ_AMARILLA, amarilla);
  digitalWrite(LUZ_VERDE, verde);
}

void imprimirEstado(Estado e) {
  switch (e) {
    case VERDE:    Serial.printf("[%6lu ms] VERDE\n", millis());    break;
    case AMARILLO: Serial.printf("[%6lu ms] AMARILLO\n", millis()); break;
    case ROJO:     Serial.printf("[%6lu ms] ROJO\n", millis());     break;
  }
}

// Toda transición pasa por aquí: cambia el estado y reinicia el cronómetro
void cambiarEstado(Estado nuevo) {
  estado = nuevo;
  inicioEstado = millis();
  imprimirEstado(estado);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  Serial.printf("Semaforo (Moore)\n");
  cambiarEstado(VERDE);
}

void loop() {
  // Cuánto tiempo llevamos en el estado actual
  unsigned long enEstado = millis() - inicioEstado;

  // ---------- Paso 3: un case por cada círculo del diagrama ----------
  switch (estado) {
    case VERDE:
      luces(false, false, true);                 // SALIDA del estado
      if (enEstado >= TIEMPO_VERDE) {            // TRANSICIÓN (flecha que sale)
        cambiarEstado(AMARILLO);
      }
      break;

    case AMARILLO:
      luces(false, true, false);
      if (enEstado >= TIEMPO_AMARILLO) {
        cambiarEstado(ROJO);
      }
      break;

    case ROJO:
      luces(true, false, false);
      if (enEstado >= TIEMPO_ROJO) {
        cambiarEstado(VERDE);
      }
      break;
  }

  // Aquí podría ir cualquier otra cosa: el loop NUNCA se queda trabado.
}
