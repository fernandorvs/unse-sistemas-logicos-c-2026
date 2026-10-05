// Clase 8 - Ejemplo 1: enum y switch (el esqueleto de un menú)
// El botón C recorre tres modos. Cada modo hace algo distinto con los LEDs.
// Observar cómo el enum le pone NOMBRE a cada número y cómo el switch
// elige qué código ejecutar según el modo.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

// ---------- Los modos, con nombre ----------
enum Modo {
  MODO_BARRA,       // vale 0: A agrega un LED a la barra
  MODO_PARPADEO,    // vale 1: los 8 LEDs titilan solos
  MODO_APAGADO,     // vale 2: todo apagado
  CANTIDAD_MODOS    // vale 3: truco para saber cuántos modos hay
};

Modo modo = MODO_BARRA;             // el modo activo (¡siempre inicializado!)

uint8_t barra = 0;                  // variable del modo BARRA
bool encendidos = false;            // variable del modo PARPADEO
unsigned long ultimoParpadeo = 0;

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

// Imprime el nombre del modo: un switch con un case por cada valor del enum
void imprimirModo(Modo m) {
  switch (m) {
    case MODO_BARRA:
      Serial.printf("Modo BARRA: apretar A para sumar un LED\n");
      break;
    case MODO_PARPADEO:
      Serial.printf("Modo PARPADEO: los LEDs titilan solos\n");
      break;
    case MODO_APAGADO:
      Serial.printf("Modo APAGADO\n");
      break;
    default:                                   // cualquier otro valor
      Serial.printf("Modo desconocido (%d)\n", (int)m);
      break;
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
  Serial.printf("C cambia de modo. Hay %d modos.\n", (int)CANTIDAD_MODOS);
  imprimirModo(modo);
}

void loop() {
  // 1) Leer los botones UNA sola vez por vuelta
  bool flancoA = seApreto(BOT_A);
  bool flancoC = seApreto(BOT_C);

  // 2) C pasa al modo siguiente: 0 -> 1 -> 2 -> 0 ...
  //    (Modo) convierte el número entero de vuelta al tipo Modo.
  if (flancoC) {
    modo = (Modo)((modo + 1) % CANTIDAD_MODOS);
    imprimirModo(modo);
  }

  // 3) Ejecutar SOLO el código del modo activo
  switch (modo) {
    case MODO_BARRA:
      if (flancoA) {
        if (barra == 0xFF) {
          barra = 0;                     // llena: vuelve a empezar
        } else {
          barra = (barra << 1) | 1;      // agrega un 1 a la derecha
        }
      }
      mostrarByte(barra);
      break;

    case MODO_PARPADEO:
      if (millis() - ultimoParpadeo >= 300) {   // cada 300 ms, sin delay
        ultimoParpadeo = millis();
        encendidos = !encendidos;
      }
      if (encendidos) {
        mostrarByte(0xFF);
      } else {
        mostrarByte(0x00);
      }
      break;

    case MODO_APAGADO:
      mostrarByte(0x00);
      break;

    default:
      modo = MODO_BARRA;                 // por las dudas: volver a un modo válido
      break;
  }
}
