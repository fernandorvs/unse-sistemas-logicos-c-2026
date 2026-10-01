// Clase 6 - Ejercicio 3 (solución): conversor binario <-> Gray
// Compara la TABLA (lo que se haría con una ROM) con la FÓRMULA (lo que se haría con compuertas XOR).
// Después cuenta de 0 a 15: L0..L3 muestran el código Gray y L4..L7 el binario.
// Observar que en Gray cambia UN SOLO LED por paso.

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

// binario -> Gray
const uint8_t GRAY[16] = {0, 1, 3, 2, 6, 7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8};

// Gray -> binario (la tabla "al revés": BIN_DE_GRAY[GRAY[n]] == n)
const uint8_t BIN_DE_GRAY[16] = {0, 1, 3, 2, 7, 6, 4, 5, 15, 14, 12, 13, 8, 9, 11, 10};

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void imprimir4Bits(uint8_t valor) {
  for (int i = 3; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

// binario -> Gray con fórmula: cada bit Gray es el XOR de dos bits vecinos
uint8_t grayConFormula(uint8_t n) {
  return n ^ (n >> 1);
}

// Gray -> binario con fórmula: XOR de todos los desplazamientos
uint8_t binarioConFormula(uint8_t g) {
  uint8_t b = 0;
  while (g != 0) {
    b = b ^ g;
    g = g >> 1;
  }
  return b;
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

  Serial.printf(" n | bin  | Gray tabla | Gray formula | vuelta tabla | vuelta formula\n");
  int errores = 0;
  for (int n = 0; n < 16; n++) {
    uint8_t gT = GRAY[n];
    uint8_t gF = grayConFormula(n);
    uint8_t bT = BIN_DE_GRAY[gT];
    uint8_t bF = binarioConFormula(gF);

    Serial.printf("%2d | ", n);
    imprimir4Bits(n);
    Serial.printf(" |    ");
    imprimir4Bits(gT);
    Serial.printf("    |     ");
    imprimir4Bits(gF);
    Serial.printf("     |      %2d      |      %2d\n", bT, bF);

    if (gT != gF || bT != n || bF != n) {
      errores++;
    }
  }
  Serial.printf("Diferencias entre tabla y formula: %d\n\n", errores);
}

void loop() {
  for (int n = 0; n < 16; n++) {
    uint8_t g = GRAY[n];
    mostrarByte((n << 4) | g);   // binario en L4..L7, Gray en L0..L3
    delay(700);
  }
}
