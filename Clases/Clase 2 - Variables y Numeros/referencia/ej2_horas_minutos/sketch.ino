// Clase 2 - Ejercicio 2 (solución): segundos -> horas:minutos:segundos
// Se usa / para "cuántos entran" y % para "cuánto sobra".
// Desafío incluido: el valor avanza 1 segundo por vuelta, como un reloj.

int totalSegundos = 3725;   // global: así conserva su valor entre vueltas de loop()

void setup() {
  Serial.begin(115200);
}

void loop() {
  int horas    = totalSegundos / 3600;          // 3600 s en una hora
  int minutos  = (totalSegundos % 3600) / 60;   // lo que sobra de las horas, en minutos
  int segundos = totalSegundos % 60;            // lo que sobra de los minutos

  // %02d = al menos 2 dígitos, rellenando con 0 a la izquierda
  Serial.printf("%d s = %02d:%02d:%02d\n", totalSegundos, horas, minutos, segundos);

  totalSegundos++;
  delay(1000);
}
