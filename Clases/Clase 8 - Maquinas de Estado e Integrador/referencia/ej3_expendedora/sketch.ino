// Clase 8 - Ejercicio 3 (solución): máquina expendedora simple
// El producto cuesta $300.
//   A = moneda de $100     B = moneda de $200     C = cancelar (devuelve el crédito)
//
// Estados:
//   ESPERANDO   -> sin crédito, todo apagado
//   ACUMULANDO  -> hay crédito, se ve en L0..L3 (un LED rojo por cada $100)
//   ENTREGANDO  -> 3 s con L4..L7 verdes, informa el vuelto
//   DEVOLVIENDO -> 2 s con los rojos titilando, devuelve todo
//
// El crédito es una variable "extra" de la máquina: el estado dice QUÉ está
// haciendo la máquina, y el crédito dice CUÁNTO lleva.

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

const int PRECIO = 300;
const unsigned long TIEMPO_ENTREGA = 3000;
const unsigned long TIEMPO_DEVOLUCION = 2000;

enum Estado { ESPERANDO, ACUMULANDO, ENTREGANDO, DEVOLVIENDO };
Estado estado = ESPERANDO;
unsigned long inicioEstado = 0;
int credito = 0;

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
    case ESPERANDO:
      credito = 0;
      Serial.printf("Inserte monedas (producto $%d)\n", PRECIO);
      break;
    case ACUMULANDO:
      break;                        // el crédito se imprime en cada moneda
    case ENTREGANDO:
      Serial.printf("Entregando producto. Vuelto: $%d\n", credito - PRECIO);
      break;
    case DEVOLVIENDO:
      Serial.printf("Cancelado. Devolviendo $%d\n", credito);
      break;
  }
}

// Suma una moneda y la informa
void agregarMoneda(int valor) {
  credito = credito + valor;
  Serial.printf("  +$%d  -> credito $%d\n", valor, credito);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  cambiarEstado(ESPERANDO);
}

void loop() {
  bool flancoA = seApreto(BOT_A);
  bool flancoB = seApreto(BOT_B);
  bool flancoC = seApreto(BOT_C);
  unsigned long enEstado = millis() - inicioEstado;

  switch (estado) {
    case ESPERANDO:
      mostrarByte(0x00);
      if (flancoA) {
        agregarMoneda(100);
        cambiarEstado(ACUMULANDO);
      } else if (flancoB) {
        agregarMoneda(200);
        cambiarEstado(ACUMULANDO);
      }
      break;

    case ACUMULANDO:
      // Barra roja: credito/100 LEDs encendidos (1 -> 0x01, 2 -> 0x03 ...)
      mostrarByte((1 << (credito / 100)) - 1);
      if (flancoA) {
        agregarMoneda(100);
      } else if (flancoB) {
        agregarMoneda(200);
      }
      if (credito >= PRECIO) {
        cambiarEstado(ENTREGANDO);
      } else if (flancoC) {
        cambiarEstado(DEVOLVIENDO);
      }
      break;

    case ENTREGANDO:
      mostrarByte(0xF0);
      if (enEstado >= TIEMPO_ENTREGA) {
        cambiarEstado(ESPERANDO);
      }
      break;

    case DEVOLVIENDO:
      if ((enEstado / 200) % 2 == 0) {
        mostrarByte(0x0F);
      } else {
        mostrarByte(0x00);
      }
      if (enEstado >= TIEMPO_DEVOLUCION) {
        cambiarEstado(ESPERANDO);
      }
      break;
  }
}
