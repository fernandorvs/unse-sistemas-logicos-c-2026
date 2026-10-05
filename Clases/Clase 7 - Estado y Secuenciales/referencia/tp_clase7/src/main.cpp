// ============================================================
// Entrenador Lógico - Clase 7
// Alumno/a: (nombre y apellido)
// Qué hace: "El entrenador cuenta". Contador binario de 8 bits
//           que se ve en los LEDs L0..L7.
//           A = +1, B = -1 (flancos con antirrebote, sin delay).
//           C alterna entre modo MANUAL y AUTO.
//           En AUTO cuenta solo, +1 cada 500 ms (con millis),
//           y A y B siguen respondiendo al instante.
//           Imprime cada cambio en decimal, hexa y binario.
// ============================================================

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

const unsigned long INTERVALO_AUTO = 500;   // ms entre cuentas en modo AUTO

uint8_t contador = 0;            // 8 bits: después de 255 viene 0 (desborde)
bool modoAuto = false;           // false = MANUAL, true = AUTO
unsigned long ultimaCuenta = 0;  // cuándo contó solo por última vez

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

// Muestra el contador en los LEDs e informa por el monitor serie
void mostrarContador() {
  mostrarByte(contador);
  Serial.printf("Contador: %3u (0x%02X) ", contador, contador);
  imprimirBinario(contador);
  if (modoAuto) {
    Serial.printf(" [AUTO]\n");
  } else {
    Serial.printf(" [MANUAL]\n");
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

  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico - Clase 7\n");
  Serial.printf("  A: +1   B: -1   C: MANUAL/AUTO\n");
  Serial.printf("==============================\n");
  mostrarContador();
}

void loop() {
  if (seApreto(0)) {             // A: sumar
    contador++;
    mostrarContador();
  }

  if (seApreto(1)) {             // B: restar
    contador--;
    mostrarContador();
  }

  if (seApreto(2)) {             // C: cambiar de modo
    modoAuto = !modoAuto;
    ultimaCuenta = millis();     // el modo AUTO empieza a contar desde ahora
    if (modoAuto) {
      Serial.printf("Modo AUTO\n");
    } else {
      Serial.printf("Modo MANUAL\n");
    }
  }

  // En AUTO: si pasó el intervalo, contar y anotar la hora
  if (modoAuto && millis() - ultimaCuenta >= INTERVALO_AUTO) {
    ultimaCuenta = millis();
    contador++;
    mostrarContador();
  }
}
