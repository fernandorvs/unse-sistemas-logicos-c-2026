// ============================================================
// Entrenador Lógico - Clase 6
// Alumno/a: (nombre y apellido)
// Qué hace: "El entrenador decodifica". Un contador de 0 a 15
//           se muestra en el display de 7 segmentos usando una
//           tabla (arreglo) como decodificador.
//           A suma 1, B resta 1 (con vuelta: 15 -> 0 y 0 -> 15).
//           El diagram.json de este proyecto ya tiene el display.
//           En la placa real se ve el patrón en los 8 LEDs.
// ============================================================

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;

// Decodificador hexa -> 7 segmentos (cátodo común). bit 0 = a ... bit 6 = g
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

int valor = 0;   // el contador (0..15)

void mostrarByte(uint8_t dato) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (dato >> i) & 1);
  }
}

void imprimirBinario(uint8_t dato) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (dato >> i) & 1);
  }
}

// Muestra el valor en el display y lo informa por el monitor serie
void mostrarValor() {
  uint8_t patron = SEGMENTOS[valor];   // la entrada es el índice, la salida el contenido
  mostrarByte(patron);
  Serial.printf("Valor: %d (0x%X) -> segmentos 0b", valor, valor);
  imprimirBinario(patron);
  Serial.printf("\n");
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico - Clase 6\n");
  Serial.printf("  A: +1   B: -1\n");
  Serial.printf("==============================\n");
  mostrarValor();
}

void loop() {
  if (digitalRead(BOTON_A) == LOW) {
    valor = valor + 1;
    if (valor > 15) {
      valor = 0;
    }
    mostrarValor();
    delay(250);   // patrón provisorio: la Clase 7 lo arregla
  }

  if (digitalRead(BOTON_B) == LOW) {
    valor = valor - 1;
    if (valor < 0) {
      valor = 15;
    }
    mostrarValor();
    delay(250);
  }
}
