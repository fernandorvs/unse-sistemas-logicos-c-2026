// Clase 6 - Ejercicio 2 (solución): codificador de prioridad 8 -> 3
// Es el "inverso" del decodificador: recibe 8 líneas y devuelve el número
// (0..7) de la línea de MÁS peso que está en 1.
// La entrada se muestra en los LEDs; la salida se imprime.

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

// Entradas de prueba
const int CANT_PRUEBAS = 6;
const uint8_t PRUEBAS[CANT_PRUEBAS] = {0b00000001, 0b00000110, 0b00101000, 0b10000001, 0b01111111, 0b00000000};

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

// Devuelve la posición del bit en 1 de más peso, o -1 si no hay ninguno.
int codificarPrioridad(uint8_t entrada) {
  for (int i = 7; i >= 0; i--) {       // buscamos desde arriba (bit 7) hacia abajo
    if ((entrada >> i) & 1) {
      return i;                        // el primero que encontramos es el de más prioridad
    }
  }
  return -1;                           // ninguna entrada activa
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  for (int p = 0; p < CANT_PRUEBAS; p++) {
    uint8_t entrada = PRUEBAS[p];
    int salida = codificarPrioridad(entrada);
    mostrarByte(entrada);

    Serial.printf("Entrada ");
    imprimirBinario(entrada);
    if (salida >= 0) {
      // la salida en binario de 3 bits
      Serial.printf(" -> salida %d (%d%d%d)\n", salida, (salida >> 2) & 1, (salida >> 1) & 1, salida & 1);
    } else {
      Serial.printf(" -> ninguna entrada activa\n");
    }
    delay(1500);
  }
}
