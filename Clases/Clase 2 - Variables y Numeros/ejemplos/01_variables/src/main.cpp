// Clase 2 - Ejemplo 1: Variables
// Declarar variables, asignarles valores e imprimirlas con printf.

#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  int edad = 19;             // declarar Y asignar en la misma línea
  int anio;                  // declarar (la cajita existe, pero no le pusimos nada)
  anio = 2026;               // asignar: guardar 2026 en la cajita "anio"

  Serial.printf("Tengo %d anios\n", edad);
  Serial.printf("Estamos en el %d\n", anio);
  Serial.printf("Naci en el %d\n", anio - edad);

  edad = edad + 1;           // leer la cajita, sumarle 1 y guardar el resultado
  Serial.printf("El anio que viene voy a tener %d\n", edad);

  uint8_t nota = 8;          // uint8_t: entero de 8 bits, de 0 a 255
  float temperatura = 27.5;  // float: número con coma (decimal)
  Serial.printf("Nota: %u   Temperatura: %f grados\n", nota, temperatura);
  Serial.printf("Temperatura con 1 decimal: %.1f grados\n", temperatura);
}

void loop() {
}
