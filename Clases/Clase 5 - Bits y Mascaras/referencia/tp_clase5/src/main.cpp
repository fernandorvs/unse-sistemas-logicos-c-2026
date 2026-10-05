// ============================================================
// Entrenador Lógico - Clase 5
// Alumno/a: (nombre y apellido)
// Qué hace: los 8 LEDs son un registro de 8 bits (uint8_t).
//           A = rota el registro a la izquierda (circular)
//           B = invierte todos los bits (~)
//           C = suma 1
//           Siempre muestra el registro en L7..L0 e imprime su
//           valor en binario, hexadecimal y decimal.
// ============================================================

#include <Arduino.h>

// ---------- Pines ----------
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

// ---------- El registro ----------
uint8_t registro = 0b10110001;   // valor inicial: 0xB1 = 177

// ---------- Funciones de bits ----------

// Muestra los 8 bits de 'valor' en los LEDs (bit i -> Li)
void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

// Imprime los 8 bits, del 7 al 0 (printf no tiene %b)
void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

// Rotación circular a la izquierda: el bit 7 vuelve a entrar por el bit 0
uint8_t rotarIzquierda(uint8_t x) {
  return (x << 1) | (x >> 7);
}

// Imprime el registro y lo muestra en los LEDs
void actualizar(uint8_t valor) {
  mostrarByte(valor);
  Serial.printf("Bin: ");
  imprimirBinario(valor);
  Serial.printf("  Hex: %02X  Dec: %u\n", valor, valor);
}

void imprimirCartel() {
  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico de Ana\n");
  Serial.printf("  Sistemas Logicos - UNSE 2026\n");
  Serial.printf("  Clase 5: registro de 8 bits\n");
  Serial.printf("==============================\n");
  Serial.printf("A = rotar   B = invertir   C = sumar 1\n\n");
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);
  pinMode(BOTON_C, INPUT_PULLUP);

  imprimirCartel();
  actualizar(registro);
}

void loop() {
  bool a = !digitalRead(BOTON_A);   // apretado = LOW, por eso el !
  bool b = !digitalRead(BOTON_B);
  bool c = !digitalRead(BOTON_C);

  // PROVISORIO: si el botón está apretado, hacemos la acción y esperamos
  // 250 ms. Si se lo mantiene apretado, la acción se repite. En la Clase 7
  // lo arreglamos con detección de flanco.
  if (a) {
    registro = rotarIzquierda(registro);
    Serial.printf("[A] rotar    -> ");
    actualizar(registro);
    delay(250);
  } else if (b) {
    registro = ~registro;
    Serial.printf("[B] invertir -> ");
    actualizar(registro);
    delay(250);
  } else if (c) {
    registro++;                     // 255 + 1 vuelve a 0
    Serial.printf("[C] sumar 1  -> ");
    actualizar(registro);
    delay(250);
  }
}
