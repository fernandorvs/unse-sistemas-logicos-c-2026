// Clase 7 - Ejemplo 2: Detectar NIVEL vs detectar FLANCO
// Mantener apretado el pulsador A y observar los dos contadores:
//  - porNivel suma en CADA vuelta de loop() mientras está apretado (¡miles!)
//  - porFlanco suma UNA vez por cada vez que se lo aprieta
// Todavía SIN antirrebote: a veces un solo apretón puede contar 2 o 3 flancos.

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;

unsigned long porNivel = 0;       // cuenta vueltas con el botón apretado
int porFlanco = 0;                // cuenta pulsaciones

bool apretadoAntes = false;       // MEMORIA: cómo estaba el botón en la vuelta anterior

void setup() {
  Serial.begin(115200);
  pinMode(LEDS[0], OUTPUT);
  pinMode(BOTON_A, INPUT_PULLUP);
  Serial.printf("Apretar A varias veces\n");
}

void loop() {
  bool apretadoAhora = (digitalRead(BOTON_A) == LOW);

  // Detección por NIVEL: "¿está apretado?"
  if (apretadoAhora) {
    porNivel++;
  }

  // Detección por FLANCO: "¿recién se apretó?" (antes suelto, ahora apretado)
  if (apretadoAhora && !apretadoAntes) {
    porFlanco++;
    Serial.printf("Flanco! porFlanco = %d   porNivel = %lu\n", porFlanco, porNivel);
  }

  // Flanco al soltar (antes apretado, ahora suelto)
  if (!apretadoAhora && apretadoAntes) {
    Serial.printf("  (soltado)\n");
  }

  apretadoAntes = apretadoAhora;  // anotar para la próxima vuelta
  digitalWrite(LEDS[0], apretadoAhora);
}
