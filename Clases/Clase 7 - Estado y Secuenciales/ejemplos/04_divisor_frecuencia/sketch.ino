// Clase 7 - Ejemplo 4: Divisor de frecuencia / contador asincrónico (ripple)
// L0 cambia cada 250 ms (parpadea a 2 Hz).
// Cada LED siguiente cambia cuando el anterior BAJA de 1 a 0,
// como una cadena de flip-flops T: L1 parpadea a 1 Hz, L2 a 0,5 Hz, ...
// Mirados juntos, L0..L6 cuentan en binario.
// Mientras tanto, L7 copia el pulsador A AL INSTANTE: con delay() no se podría.

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;

const unsigned long MEDIO_PERIODO = 250;   // ms entre cambios de L0
unsigned long ultimoTic = 0;               // cuándo cambió L0 por última vez

bool etapa[7] = {false, false, false, false, false, false, false};  // los 7 flip-flops

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  Serial.printf("Divisor de frecuencia ripple en L0..L6. Apretar A: L7\n");
}

void loop() {
  // Tarea 1: el reloj del contador, sin delay()
  if (millis() - ultimoTic >= MEDIO_PERIODO) {
    ultimoTic = millis();                  // anotar la hora

    // La primera etapa siempre cambia; la siguiente solo si esta bajó de 1 a 0
    int i = 0;
    bool propagar = true;
    while (propagar && i < 7) {
      etapa[i] = !etapa[i];
      digitalWrite(LEDS[i], etapa[i]);
      propagar = (etapa[i] == false);     // flanco descendente -> sigue la cadena
      i++;
    }
  }

  // Tarea 2: atender el pulsador, en cada vuelta de loop()
  digitalWrite(LEDS[7], digitalRead(BOTON_A) == LOW);
}
