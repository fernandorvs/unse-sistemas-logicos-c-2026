// Clase 5 - Ejemplo 1: Literales binarios y hexadecimales
// Un uint8_t tiene 8 bits: uno por cada LED.
// El mismo número se puede escribir en decimal, binario (0b...) o hexa (0x...).

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

// Muestra los 8 bits de 'valor' en los 8 LEDs (bit i -> LED Li)
void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);   // bit i de valor
  }
}

// Imprime los 8 bits de 'valor', del bit 7 al bit 0 (printf no tiene %b)
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

  uint8_t a = 177;           // decimal
  uint8_t b = 0b10110001;    // binario: el mismo número
  uint8_t c = 0xB1;          // hexadecimal: el mismo número otra vez

  Serial.printf("a = %u, b = %u, c = %u\n", a, b, c);
  Serial.printf("En binario: ");
  imprimirBinario(a);
  Serial.printf("\nEn hexa: 0x%02X\n", a);
}

void loop() {
  // Mostramos algunos patrones en los LEDs
  mostrarByte(0b10110001);   // 0xB1
  delay(1000);
  mostrarByte(0b11110000);   // nibble alto (verdes) encendido
  delay(1000);
  mostrarByte(0b00001111);   // nibble bajo (rojos) encendido
  delay(1000);
  mostrarByte(0xAA);         // 10101010: uno sí, uno no
  delay(1000);
  mostrarByte(0x55);         // 01010101: al revés
  delay(1000);
}
