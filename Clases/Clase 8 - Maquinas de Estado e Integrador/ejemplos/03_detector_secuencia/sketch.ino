// Clase 8 - Ejemplo 3: Detector de secuencia A, A, B (máquina de MOORE)
// Cada pulsación de A o B es un "símbolo" de entrada.
// Cuando las últimas tres pulsaciones fueron A, A, B se enciende L0.
//
// Diagrama de estados (Moore: la salida está DENTRO del estado):
//
//   ESPERANDO/0 --A--> VIO_A/0 --A--> VIO_AA/0 --B--> DETECTADO/1
//
//   Las otras flechas: ver la tabla de transiciones en el README.
//
// Salidas:
//   L0         = detectado (salida Moore de la máquina)
//   L4 ... L7  = una luz verde por estado (para "ver" en qué estado está)

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

enum Estado { ESPERANDO, VIO_A, VIO_AA, DETECTADO };
Estado estado = ESPERANDO;

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

void imprimirEstado(Estado e) {
  switch (e) {
    case ESPERANDO: Serial.printf("ESPERANDO\n");               break;
    case VIO_A:     Serial.printf("VIO_A\n");                   break;
    case VIO_AA:    Serial.printf("VIO_AA\n");                  break;
    case DETECTADO: Serial.printf("DETECTADO  <-- A, A, B!\n"); break;
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
  Serial.printf("Detector de A, A, B. Apretar A y B en cualquier orden.\n");
  imprimirEstado(estado);
}

void loop() {
  bool flancoA = seApreto(BOT_A);
  bool flancoB = seApreto(BOT_B);

  // ---------- Transiciones: solo cuando llega un símbolo ----------
  if (flancoA || flancoB) {
    if (flancoA) {
      Serial.printf("Entrada A -> ");
    } else {
      Serial.printf("Entrada B -> ");
    }

    switch (estado) {
      case ESPERANDO:
        if (flancoA) estado = VIO_A;       // empieza una posible secuencia
        else         estado = ESPERANDO;   // B suelta no sirve
        break;

      case VIO_A:
        if (flancoA) estado = VIO_AA;
        else         estado = ESPERANDO;   // "A B": hay que empezar de nuevo
        break;

      case VIO_AA:
        if (flancoA) estado = VIO_AA;      // "A A A": las dos últimas siguen siendo A A
        else         estado = DETECTADO;   // "A A B": ¡secuencia completa!
        break;

      case DETECTADO:
        if (flancoA) estado = VIO_A;       // la A nueva puede empezar otra secuencia
        else         estado = ESPERANDO;
        break;
    }
    imprimirEstado(estado);
  }

  // ---------- Salidas (Moore): dependen SOLO del estado ----------
  digitalWrite(LEDS[0], estado == DETECTADO);
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[4 + i], i == (int)estado);   // L4 = ESPERANDO ... L7 = DETECTADO
  }
}
