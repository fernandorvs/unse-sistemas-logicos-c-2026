// Clase 2 - Ejercicio 1 (solución): calculadora de dos variables
// Cambiar los valores de a y b y volver a correr.

#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  int a = 23;
  int b = 4;

  Serial.printf("=== Calculadora ===\n");
  Serial.printf("%d + %d = %d\n", a, b, a + b);
  Serial.printf("%d - %d = %d\n", a, b, a - b);
  Serial.printf("%d * %d = %d\n", a, b, a * b);
  Serial.printf("%d / %d = %d  (resto %d)\n", a, b, a / b, a % b);

  // Para tener decimales, guardamos los valores en variables float
  float fa = a;
  float fb = b;
  Serial.printf("%d / %d = %.3f  (con float)\n", a, b, fa / fb);

  // Comprobación: cociente * divisor + resto = dividendo
  Serial.printf("Comprobacion: %d * %d + %d = %d\n", a / b, b, a % b, (a / b) * b + a % b);
}

void loop() {
}
