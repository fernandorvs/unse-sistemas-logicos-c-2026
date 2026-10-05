// ============================================================
// Entrenador Lógico - Clase 2
// Alumno/a: (nombre y apellido)
// Qué hace: cuenta de 0 a 15, un número por segundo. Muestra
//           cada número en decimal y en hexa por el monitor
//           serie, y en binario en los LEDs L0 a L3.
// ============================================================

#include <Arduino.h>

const int L0 = 4;    // bit 0 (peso 1)
const int L1 = 16;   // bit 1 (peso 2)
const int L2 = 17;   // bit 2 (peso 4)
const int L3 = 18;   // bit 3 (peso 8)

int contador = 0;    // global: conserva su valor entre vueltas de loop()

void setup() {
  Serial.begin(115200);

  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);

  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico de Ana\n");
  Serial.printf("  Sistemas Logicos - UNSE 2026\n");
  Serial.printf("==============================\n");
  Serial.printf("Modo: contador binario de 4 bits\n\n");
}

void loop() {
  // Mostrar el número en binario: divisiones sucesivas por 2
  digitalWrite(L0, contador % 2);
  digitalWrite(L1, (contador / 2) % 2);
  digitalWrite(L2, (contador / 4) % 2);
  digitalWrite(L3, (contador / 8) % 2);

  Serial.printf("Decimal: %2d  Hexa: %X  -> LEDs %d%d%d%d\n",
                contador, contador,
                (contador / 8) % 2, (contador / 4) % 2,
                (contador / 2) % 2, contador % 2);

  // Avanzar: después de 15 viene 0 (aritmética módulo 16)
  contador = (contador + 1) % 16;

  delay(1000);
}
