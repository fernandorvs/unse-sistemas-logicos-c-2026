// Clase 7 - Ejercicio 2 (solución): registro de desplazamiento de 8 bits
// A = dato (se lee como nivel), B = reloj (flanco, con antirrebote), C = borrar.
// En cada flanco de B, todos los bits se corren un lugar hacia L7
// y en L0 entra el valor de A. El bit que estaba en L7 se pierde.

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

uint8_t registro = 0;   // los 8 flip-flops D del registro

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

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("Registro de desplazamiento: A = dato, B = reloj, C = borrar\n");
  mostrarByte(registro);
}

void loop() {
  if (seApreto(1)) {                               // flanco de reloj
    bool dato = (digitalRead(BOTON_A) == LOW);
    registro = (registro << 1) | dato;             // correr y meter el dato en el bit 0
    mostrarByte(registro);
    Serial.printf("Entra %d -> ", dato);
    imprimirBinario(registro);
    Serial.printf("\n");
  }

  if (seApreto(2)) {
    registro = 0;
    mostrarByte(registro);
    Serial.printf("Registro borrado\n");
  }
}
