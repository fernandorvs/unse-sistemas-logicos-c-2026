// Clase 2 - Ejemplo 2: Operadores aritméticos y formatos de printf
// Ojo con la división entera: entre enteros, / descarta los decimales.

void setup() {
  Serial.begin(115200);

  int a = 17;
  int b = 5;

  Serial.printf("a = %d   b = %d\n", a, b);
  Serial.printf("a + b = %d\n", a + b);    // 22
  Serial.printf("a - b = %d\n", a - b);    // 12
  Serial.printf("a * b = %d\n", a * b);    // 85
  Serial.printf("a / b = %d\n", a / b);    // 3   (¡no 3,4!)
  Serial.printf("a %% b = %d\n", a % b);   // 2   (el resto; %% imprime un %)

  // Con float sí hay decimales
  float x = 17.0;
  float y = 5.0;
  Serial.printf("x / y = %.2f\n", x / y);  // 3.40

  // El mismo número en decimal y en hexadecimal
  int n = 255;
  Serial.printf("%d en hexa es %X\n", n, n);   // 255 en hexa es FF

  // ++ suma 1 a la variable
  n++;
  Serial.printf("Despues de n++, n vale %d (hexa %X)\n", n, n);
}

void loop() {
}
