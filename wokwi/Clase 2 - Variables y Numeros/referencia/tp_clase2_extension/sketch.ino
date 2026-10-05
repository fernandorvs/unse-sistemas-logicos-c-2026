// ============================================================
// Entrenador Lógico - Clase 2 (extensión)
// Alumno/a: (nombre y apellido)
// Qué hace: cuenta con un uint8_t (8 bits, 0 a 255). Lo muestra
//           en decimal y hexa y en los 8 LEDs. Al pasar de 255
//           vuelve solo a 0: es el DESBORDE (módulo 256).
// ============================================================

#include <Arduino.h>

const int L0 = 4;    // bit 0 (peso 1)
const int L1 = 16;   // bit 1 (peso 2)
const int L2 = 17;   // bit 2 (peso 4)
const int L3 = 18;   // bit 3 (peso 8)
const int L4 = 19;   // bit 4 (peso 16)
const int L5 = 21;   // bit 5 (peso 32)
const int L6 = 22;   // bit 6 (peso 64)
const int L7 = 23;   // bit 7 (peso 128)

uint8_t contador = 250;   // arrancamos cerca de 255 para ver el desborde enseguida

void setup() {
  Serial.begin(115200);

  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);
  pinMode(L6, OUTPUT);
  pinMode(L7, OUTPUT);

  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico de Ana\n");
  Serial.printf("  Sistemas Logicos - UNSE 2026\n");
  Serial.printf("==============================\n");
  Serial.printf("Modo: contador de 8 bits (uint8_t)\n\n");
}

void loop() {
  digitalWrite(L0, contador % 2);
  digitalWrite(L1, (contador / 2) % 2);
  digitalWrite(L2, (contador / 4) % 2);
  digitalWrite(L3, (contador / 8) % 2);
  digitalWrite(L4, (contador / 16) % 2);
  digitalWrite(L5, (contador / 32) % 2);
  digitalWrite(L6, (contador / 64) % 2);
  digitalWrite(L7, (contador / 128) % 2);

  Serial.printf("Decimal: %3u  Hexa: %02X\n", contador, contador);

  // Sin % 256: el uint8_t desborda solo (255 + 1 = 0)
  contador++;

  delay(500);
}
