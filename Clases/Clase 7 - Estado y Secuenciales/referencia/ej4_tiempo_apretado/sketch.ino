// Clase 7 - Ejercicio 4 (solución): medir cuánto tiempo estuvo apretado un botón
// Al apretar A se anota la hora (millis). Al soltarlo se resta y se imprime.
// Mientras está apretado, los LEDs hacen una barra que crece un LED cada 250 ms
// (sin delay: loop() sigue leyendo el botón todo el tiempo).

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const unsigned long ANTIRREBOTE_MS = 50;

bool apretadoAntes = false;        // memoria: estado anterior del botón
unsigned long ultimoCambio = 0;    // para el antirrebote
unsigned long inicio = 0;          // hora en que se apretó

void mostrarBarra(int cantidad) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], i < cantidad);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  Serial.printf("Mantener apretado A y luego soltarlo\n");
}

void loop() {
  unsigned long ahora = millis();
  bool apretadoAhora = (digitalRead(BOTON_A) == LOW);

  if (apretadoAhora != apretadoAntes && ahora - ultimoCambio >= ANTIRREBOTE_MS) {
    ultimoCambio = ahora;
    apretadoAntes = apretadoAhora;

    if (apretadoAhora) {
      // flanco al apretar: anotar la hora
      inicio = ahora;
    } else {
      // flanco al soltar: calcular la duración
      unsigned long duracion = ahora - inicio;
      Serial.printf("A estuvo apretado durante %lu ms (%lu,%03lu s)\n",
                    duracion, duracion / 1000, duracion % 1000);
      mostrarBarra(0);
    }
  }

  // Mientras está apretado: barra que crece con el tiempo
  if (apretadoAntes) {
    int cantidad = (ahora - inicio) / 250 + 1;
    if (cantidad > 8) {
      cantidad = 8;
    }
    mostrarBarra(cantidad);
  }
}
