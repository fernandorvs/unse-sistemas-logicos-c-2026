// Clase 5 - Ejemplo 2: AND, OR, XOR y NOT bit a bit entre dos bytes
// Cada operador aplica la compuerta a los 8 pares de bits a la vez.

// Imprime los 8 bits de 'valor', del bit 7 al bit 0 (printf no tiene %b)
void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

void setup() {
  Serial.begin(115200);

  uint8_t x = 0b11001100;
  uint8_t y = 0b10101010;

  // Guardamos cada resultado en un uint8_t para quedarnos con 8 bits
  uint8_t r_and = x & y;     // AND: 1 solo donde los dos tienen 1
  uint8_t r_or = x | y;      // OR:  1 donde alguno tiene 1
  uint8_t r_xor = x ^ y;     // XOR: 1 donde son distintos
  uint8_t r_not = ~x;        // NOT: invierte todos los bits de x

  Serial.printf("x      = "); imprimirBinario(x);     Serial.printf("  0x%02X\n", x);
  Serial.printf("y      = "); imprimirBinario(y);     Serial.printf("  0x%02X\n", y);
  Serial.printf("--------------------------\n");
  Serial.printf("x & y  = "); imprimirBinario(r_and); Serial.printf("  0x%02X\n", r_and);
  Serial.printf("x | y  = "); imprimirBinario(r_or);  Serial.printf("  0x%02X\n", r_or);
  Serial.printf("x ^ y  = "); imprimirBinario(r_xor); Serial.printf("  0x%02X\n", r_xor);
  Serial.printf("~x     = "); imprimirBinario(r_not); Serial.printf("  0x%02X\n", r_not);

  // La diferencia con los operadores LÓGICOS de la Clase 3:
  Serial.printf("--------------------------\n");
  Serial.printf("5 & 2  = %d  (bit a bit: 101 & 010 = 000)\n", 5 & 2);
  Serial.printf("5 && 2 = %d  (logico: verdadero Y verdadero)\n", 5 && 2);
  Serial.printf("~5     = %d (bit a bit: invierte TODOS los bits del int)\n", ~5);
  Serial.printf("!5     = %d  (logico: NO verdadero = falso)\n", !5);
}

void loop() {
}
