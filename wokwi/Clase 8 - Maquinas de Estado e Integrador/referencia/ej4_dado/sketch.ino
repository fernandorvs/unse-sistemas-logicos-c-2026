// Clase 8 - Ejercicio 4 (bonus, solución): dado electrónico
// A = tirar el dado. Mientras "rueda" (1,5 s) los números cambian rápido;
// después queda fijo el resultado.
// El número se muestra en 7 segmentos (el diagram.json de este proyecto ya tiene el display)
// y por supuesto también se ve en los LEDs.
//
// Estados: ESPERANDO -> RODANDO -> MOSTRANDO -> (A) -> RODANDO ...

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

// Decodificador hexa -> 7 segmentos (cátodo común). bit 0 = a ... bit 6 = g (Clase 6)
// (el dado solo usa del 1 al 6, pero dejamos la tabla igual que en la Clase 6)
const uint8_t SEGMENTOS[16] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111,  // 9
  0b01110111,  // A
  0b01111100,  // b
  0b00111001,  // C
  0b01011110,  // d
  0b01111001,  // E
  0b01110001   // F
};
const uint8_t GUION = 0x40;                 // solo el segmento g: "-"

const unsigned long TIEMPO_RODANDO = 1500;
const unsigned long PASO_RODANDO = 80;      // cada cuánto cambia el número al rodar

enum Estado { ESPERANDO, RODANDO, MOSTRANDO };
Estado estado = ESPERANDO;
unsigned long inicioEstado = 0;
unsigned long ultimoPaso = 0;
int cara = 1;                               // número que se está mostrando (1 a 6)
int tiradas = 0;

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

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("Dado electronico. Apretar A para tirar.\n");
}

void loop() {
  bool flancoA = seApreto(BOT_A);
  unsigned long enEstado = millis() - inicioEstado;

  switch (estado) {
    case ESPERANDO:
      mostrarByte(GUION);
      if (flancoA) {
        estado = RODANDO;
        inicioEstado = millis();
      }
      break;

    case RODANDO:
      if (millis() - ultimoPaso >= PASO_RODANDO) {
        ultimoPaso = millis();
        cara = random(1, 7);                // número al azar entre 1 y 6
      }
      mostrarByte(SEGMENTOS[cara]);
      if (enEstado >= TIEMPO_RODANDO) {
        tiradas++;
        Serial.printf("Tirada %d: salio %d\n", tiradas, cara);
        estado = MOSTRANDO;
        inicioEstado = millis();
      }
      break;

    case MOSTRANDO:
      mostrarByte(SEGMENTOS[cara]);
      if (flancoA) {
        estado = RODANDO;
        inicioEstado = millis();
      }
      break;
  }
}
