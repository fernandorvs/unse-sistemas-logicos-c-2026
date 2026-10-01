// ============================================================
// Entrenador Lógico - Clase 1
// Alumno/a: (nombre y apellido)
// Qué hace: se presenta por el monitor serie y enciende los
//           LEDs L0 a L3 de a uno, avisando cuál está prendido.
// ============================================================

void setup() {
  Serial.begin(115200);

  pinMode(4, OUTPUT);    // L0
  pinMode(16, OUTPUT);   // L1
  pinMode(17, OUTPUT);   // L2
  pinMode(18, OUTPUT);   // L3

  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico de Ana\n");
  Serial.printf("  Sistemas Logicos - UNSE 2026\n");
  Serial.printf("==============================\n");
}

void loop() {
  digitalWrite(4, HIGH);
  Serial.printf("Encendido: L0\n");
  delay(500);

  digitalWrite(16, HIGH);
  Serial.printf("Encendido: L1\n");
  delay(500);

  digitalWrite(17, HIGH);
  Serial.printf("Encendido: L2\n");
  delay(500);

  digitalWrite(18, HIGH);
  Serial.printf("Encendido: L3\n");
  delay(500);

  digitalWrite(4, LOW);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  digitalWrite(18, LOW);
  Serial.printf("Todos apagados\n\n");
  delay(1000);
}
