// Clase 3 - Ejemplo 2: Decisiones con números
// Un contador de 0 a 19 y, para cada número, el programa decide:
//   - si es par o impar               (if / else)
//   - si es chico, mediano o grande   (if / else if / else)

int n = 0;   // global: conserva su valor entre vueltas de loop()

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.printf("%2d es ", n);

  if (n % 2 == 0) {            // ¡dos signos igual para comparar!
    Serial.printf("par    ");
  } else {
    Serial.printf("impar  ");
  }

  if (n < 5) {
    Serial.printf("y chico\n");
  } else if (n < 15) {         // aquí ya sabemos que n >= 5
    Serial.printf("y mediano\n");
  } else {                     // n >= 15
    Serial.printf("y grande\n");
  }

  n = (n + 1) % 20;
  delay(700);
}
