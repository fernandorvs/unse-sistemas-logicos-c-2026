// Clase 6 - Ejemplo 3: Decodificador 2 -> 4, de dos formas
// Entradas: pulsadores A (bit 0) y B (bit 1). Salidas: L0..L3 (una sola encendida).
// Forma 1: con if (como si escribiéramos las ecuaciones).
// Forma 2: con una tabla (arreglo). Las dos dan lo mismo.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;

// Tabla del decodificador: entrada (0..3) -> salida one-hot
const uint8_t DECO_2A4[4] = {
  0b0001,   // entrada 0 -> Y0 (L0)
  0b0010,   // entrada 1 -> Y1 (L1)
  0b0100,   // entrada 2 -> Y2 (L2)
  0b1000    // entrada 3 -> Y3 (L3)
};

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

// Forma 1: decodificador con if
uint8_t decoConIf(bool a, bool b) {
  if (!b && !a) {
    return 0b0001;
  } else if (!b && a) {
    return 0b0010;
  } else if (b && !a) {
    return 0b0100;
  } else {
    return 0b1000;
  }
}

// Forma 2: decodificador con tabla
uint8_t decoConTabla(bool a, bool b) {
  int entrada = b * 2 + a;    // B es el bit de más peso
  return DECO_2A4[entrada];
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  // Verificación: las dos formas tienen que dar lo mismo para las 4 entradas
  Serial.printf(" B A | con if | con tabla\n");
  for (int b = 0; b <= 1; b++) {
    for (int a = 0; a <= 1; a++) {
      Serial.printf(" %d %d |  0x%X   |   0x%X\n", b, a, decoConIf(a, b), decoConTabla(a, b));
    }
  }
}

void loop() {
  bool a = (digitalRead(BOTON_A) == LOW);   // apretado = 1
  bool b = (digitalRead(BOTON_B) == LOW);
  mostrarByte(decoConTabla(a, b));          // probar cambiar por decoConIf(a, b)
}
