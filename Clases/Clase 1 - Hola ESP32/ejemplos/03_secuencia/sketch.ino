// Clase 1 - Ejemplo 3: Secuencia de dos LEDs
// L0 y L1 se encienden alternadamente, como las luces de un paso a nivel.

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);    // L0
  pinMode(16, OUTPUT);   // L1
}

void loop() {
  digitalWrite(4, HIGH);
  digitalWrite(16, LOW);
  delay(400);

  digitalWrite(4, LOW);
  digitalWrite(16, HIGH);
  delay(400);
}
