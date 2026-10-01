// Clase 7 - Ejemplo 1: Variables globales, locales y static
// Tres contadores que se incrementan una vez por segundo.
// Observar en el monitor serie cuál "recuerda" su valor y cuál "se olvida".

int contadorGlobal = 0;          // GLOBAL: existe durante todo el programa

void contarLocal() {
  int contador = 0;              // LOCAL: nace en cada llamada y muere al salir
  contador++;
  Serial.printf("  local:  %d\n", contador);
}

void contarStatic() {
  static int contador = 0;       // STATIC: local (nadie de afuera la ve)
  contador++;                    // pero NO muere: conserva su valor entre llamadas
  Serial.printf("  static: %d\n", contador);
}

void contarGlobal() {
  contadorGlobal++;
  Serial.printf("  global: %d\n", contadorGlobal);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Global, local y static\n");
}

void loop() {
  Serial.printf("Vuelta de loop():\n");
  contarLocal();     // siempre imprime 1: se olvida
  contarStatic();    // 1, 2, 3, ...: recuerda
  contarGlobal();    // 1, 2, 3, ...: recuerda
  delay(1000);
}
