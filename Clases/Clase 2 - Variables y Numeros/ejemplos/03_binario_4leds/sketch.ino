// Clase 2 - Ejemplo 3: Un número en binario en 4 LEDs
// Método de divisiones sucesivas: cada bit es el resto de dividir por 2.
//   bit 0 (peso 1) = n % 2
//   bit 1 (peso 2) = (n / 2) % 2
//   bit 2 (peso 4) = (n / 4) % 2
//   bit 3 (peso 8) = (n / 8) % 2

const int L0 = 4;    // bit 0, el de menos peso
const int L1 = 16;
const int L2 = 17;
const int L3 = 18;   // bit 3, el de más peso

void setup() {
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);

  int n = 13;        // probar cambiarlo por cualquier número de 0 a 15

  // Cada cuenta da 0 o 1, que es justo LOW o HIGH
  digitalWrite(L0, n % 2);
  digitalWrite(L1, (n / 2) % 2);
  digitalWrite(L2, (n / 4) % 2);
  digitalWrite(L3, (n / 8) % 2);

  Serial.printf("n = %d\n", n);
  Serial.printf("Bits (L3 L2 L1 L0): %d %d %d %d\n",
                (n / 8) % 2, (n / 4) % 2, (n / 2) % 2, n % 2);
}

void loop() {
}
