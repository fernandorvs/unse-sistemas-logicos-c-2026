// Clase 2 - Ejercicio 3 (solución): un número fijo en 4 LEDs
// Primero hacer las divisiones sucesivas en papel, después verificar aquí.
//
//   11 / 2 = 5  resto 1   -> L0
//    5 / 2 = 2  resto 1   -> L1
//    2 / 2 = 1  resto 0   -> L2
//    1 / 2 = 0  resto 1   -> L3       11 = 1011 en binario = B en hexa

const int L0 = 4;
const int L1 = 16;
const int L2 = 17;
const int L3 = 18;

void setup() {
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);

  int n = 11;

  int b0 = n % 2;
  int b1 = (n / 2) % 2;
  int b2 = (n / 4) % 2;
  int b3 = (n / 8) % 2;

  digitalWrite(L0, b0);
  digitalWrite(L1, b1);
  digitalWrite(L2, b2);
  digitalWrite(L3, b3);

  Serial.printf("n = %d (hexa %X)\n", n, n);
  Serial.printf("Binario: %d%d%d%d\n", b3, b2, b1, b0);
  // Verificación con los pesos: 8*b3 + 4*b2 + 2*b1 + 1*b0 tiene que dar n
  Serial.printf("Verificacion: 8*%d + 4*%d + 2*%d + 1*%d = %d\n",
                b3, b2, b1, b0, 8 * b3 + 4 * b2 + 2 * b1 + b0);
}

void loop() {
}
