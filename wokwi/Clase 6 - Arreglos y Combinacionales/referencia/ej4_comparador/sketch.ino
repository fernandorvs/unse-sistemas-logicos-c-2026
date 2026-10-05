// Clase 6 - Ejercicio 4 (solución): comparador de 2 bits
// X (0..3) se cambia con A, Y (0..3) se cambia con B (patrón provisorio con delay).
// X se ve en L0-L1, Y en L2-L3.
// Salidas: L5 = X < Y,  L6 = X == Y,  L7 = X > Y.
// El comparador está hecho con una TABLA de 16 entradas (índice = Y*4 + X),
// y se verifica contra los operadores <, ==, > de C.

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;

// Cada salida: bit 0 = menor, bit 1 = igual, bit 2 = mayor
const uint8_t MENOR = 0b001;
const uint8_t IGUAL = 0b010;
const uint8_t MAYOR = 0b100;

// índice = Y*4 + X
const uint8_t COMPARADOR[16] = {
  // X=0    X=1    X=2    X=3
  IGUAL, MAYOR, MAYOR, MAYOR,   // Y=0
  MENOR, IGUAL, MAYOR, MAYOR,   // Y=1
  MENOR, MENOR, IGUAL, MAYOR,   // Y=2
  MENOR, MENOR, MENOR, IGUAL    // Y=3
};

int x = 0;
int y = 0;

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void actualizar() {
  uint8_t resultado = COMPARADOR[y * 4 + x];
  // armar el byte de los LEDs: X en bits 0-1, Y en bits 2-3, resultado en bits 5-7
  mostrarByte(x | (y << 2) | (resultado << 5));

  if (resultado == MENOR) {
    Serial.printf("X=%d Y=%d -> X < Y\n", x, y);
  } else if (resultado == IGUAL) {
    Serial.printf("X=%d Y=%d -> X == Y\n", x, y);
  } else {
    Serial.printf("X=%d Y=%d -> X > Y\n", x, y);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  // Verificar la tabla contra los operadores de C, para las 16 combinaciones
  int errores = 0;
  for (int yy = 0; yy < 4; yy++) {
    for (int xx = 0; xx < 4; xx++) {
      uint8_t esperado;
      if (xx < yy) {
        esperado = MENOR;
      } else if (xx == yy) {
        esperado = IGUAL;
      } else {
        esperado = MAYOR;
      }
      if (COMPARADOR[yy * 4 + xx] != esperado) {
        Serial.printf("Error en la tabla: X=%d Y=%d\n", xx, yy);
        errores++;
      }
    }
  }
  Serial.printf("Tabla verificada: %d errores\n", errores);
  actualizar();
}

void loop() {
  if (digitalRead(BOTON_A) == LOW) {
    x++;
    if (x > 3) {
      x = 0;
    }
    actualizar();
    delay(250);   // patrón provisorio (la Clase 7 lo mejora)
  }
  if (digitalRead(BOTON_B) == LOW) {
    y++;
    if (y > 3) {
      y = 0;
    }
    actualizar();
    delay(250);
  }
}
