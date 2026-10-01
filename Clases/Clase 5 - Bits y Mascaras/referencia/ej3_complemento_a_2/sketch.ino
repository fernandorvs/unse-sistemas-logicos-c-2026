// Clase 5 - Ejercicio 3 (solución): complemento a 2
// Para obtener el negativo de x en 8 bits: invertir todos los bits y sumar 1.
//   -x  =  ~x + 1
// Guardado en un uint8_t se ve como un número de 0 a 255.
// Guardado en un int8_t (entero CON signo de 8 bits, -128..127) se ve negativo.

void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

void setup() {
  Serial.begin(115200);

  Serial.printf("  x        ~x        ~x + 1    como int8_t\n");

  for (int x = 1; x <= 6; x++) {
    uint8_t invertido = ~x;
    uint8_t negativo = ~x + 1;
    int8_t conSigno = negativo;     // los MISMOS 8 bits, leídos con signo

    Serial.printf("%3d ", x);
    imprimirBinario(x);
    Serial.printf("  ");
    imprimirBinario(invertido);
    Serial.printf("  ");
    imprimirBinario(negativo);
    Serial.printf("  (%3u) -> %d\n", negativo, conSigno);
  }

  // Comprobación: x + (-x) da 0 (el acarreo del bit 8 se pierde)
  uint8_t x = 5;
  uint8_t menosX = ~x + 1;
  uint8_t suma = x + menosX;
  Serial.printf("\n5 + (~5 + 1) = %u en 8 bits\n", suma);
}

void loop() {
}
