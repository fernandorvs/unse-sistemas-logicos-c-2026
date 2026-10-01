// Clase 8 - Ejemplo 4: Cerradura electrónica
// La clave son 4 pulsaciones de A y B (aquí: A B B A).
//
// Estados:
//   BLOQUEADA  -> esperando la primera tecla              (L0 rojo fijo)
//   INGRESANDO -> ya se apretaron 1, 2 o 3 teclas         (L0 + una barra verde)
//   ABIERTA    -> clave correcta, se cierra sola en 5 s   (L4..L7 verdes)
//   ALARMA     -> 3 claves mal seguidas, dura 10 s        (L0..L3 rojos titilando)
//
// Si en INGRESANDO pasan 5 s sin apretar nada, se cancela y vuelve a BLOQUEADA.
// En ABIERTA, el botón C cierra en el momento.

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

const int CLAVE[4] = {0, 1, 1, 0};          // 0 = A, 1 = B  ->  A B B A
const int MAX_INTENTOS = 3;
const unsigned long TIEMPO_ABIERTA = 5000;
const unsigned long TIEMPO_ALARMA = 10000;
const unsigned long TIEMPO_SIN_TECLAS = 5000;

enum Estado { BLOQUEADA, INGRESANDO, ABIERTA, ALARMA };
Estado estado = BLOQUEADA;
unsigned long inicioEstado = 0;

// Variables "extra" de la máquina (además del estado)
int teclas = 0;                   // cuántas teclas lleva el intento actual
bool claveBien = true;            // ¿todas las teclas hasta ahora coinciden?
int intentosFallidos = 0;
unsigned long ultimaTecla = 0;

// ---------- Detección de flancos con antirrebote (para los 3 botones) ----------
const int CANT_BOTONES = 3;
const int BOTONES[CANT_BOTONES] = {BOTON_A, BOTON_B, BOTON_C};  // 0 = A, 1 = B, 2 = C
const unsigned long ANTIRREBOTE_MS = 50;

bool apretadoAntes[CANT_BOTONES] = {false, false, false};  // memoria de cada botón
unsigned long ultimoCambio[CANT_BOTONES] = {0, 0, 0};      // cuándo cambió por última vez

// Devuelve true UNA sola vez por cada pulsación del botón (0 = A, 1 = B, 2 = C).
bool seApreto(int boton) {
  bool apretadoAhora = (digitalRead(BOTONES[boton]) == LOW);
  bool hayFlanco = false;

  if (apretadoAhora != apretadoAntes[boton]) {                 // ¿cambió?
    if (millis() - ultimoCambio[boton] >= ANTIRREBOTE_MS) {    // ¿y no es un rebote?
      ultimoCambio[boton] = millis();
      apretadoAntes[boton] = apretadoAhora;
      if (apretadoAhora) {
        hayFlanco = true;                                      // cambió de suelto a apretado
      }
    }
  }
  return hayFlanco;
}
// --------------------------------------------------------------------------------

// Clase 8: nombres para los índices de BOTONES, en vez de 0, 1 y 2
enum Boton { BOT_A, BOT_B, BOT_C };

void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

void cambiarEstado(Estado nuevo) {
  estado = nuevo;
  inicioEstado = millis();
  switch (estado) {
    case BLOQUEADA:
      teclas = 0;                 // al entrar a BLOQUEADA, el intento empieza de cero
      claveBien = true;
      Serial.printf("BLOQUEADA (intentos fallidos: %d)\n", intentosFallidos);
      break;
    case INGRESANDO:
      Serial.printf("INGRESANDO...\n");
      break;
    case ABIERTA:
      Serial.printf("ABIERTA! Se cierra sola en %lu s\n", TIEMPO_ABIERTA / 1000);
      break;
    case ALARMA:
      Serial.printf("*** ALARMA *** (%d intentos fallidos)\n", intentosFallidos);
      break;
  }
}

// Guarda una tecla (0 = A, 1 = B) y la compara con la clave
void registrarTecla(int tecla) {
  if (tecla != CLAVE[teclas]) {
    claveBien = false;            // no decimos cuál falló: ¡es una cerradura!
  }
  teclas++;
  ultimaTecla = millis();
  Serial.printf("  tecla %d de 4\n", teclas);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("Cerradura electronica. Ingresar la clave con A y B.\n");
  cambiarEstado(BLOQUEADA);
}

void loop() {
  bool flancoA = seApreto(BOT_A);
  bool flancoB = seApreto(BOT_B);
  bool flancoC = seApreto(BOT_C);
  unsigned long enEstado = millis() - inicioEstado;

  switch (estado) {
    case BLOQUEADA:
      mostrarByte(0x01);                          // L0 rojo fijo
      if (flancoA) {
        cambiarEstado(INGRESANDO);
        registrarTecla(0);
      } else if (flancoB) {
        cambiarEstado(INGRESANDO);
        registrarTecla(1);
      }
      break;

    case INGRESANDO:
      // L0 + una barra verde con las teclas ingresadas (1 a 3 LEDs desde L4)
      mostrarByte(0x01 | (((1 << teclas) - 1) << 4));
      if (flancoA) {
        registrarTecla(0);
      } else if (flancoB) {                       // else: nunca dos teclas en la misma vuelta
        registrarTecla(1);
      }
      if (teclas == 4) {                          // intento completo: decidir
        if (claveBien) {
          intentosFallidos = 0;
          cambiarEstado(ABIERTA);
        } else {
          intentosFallidos++;
          Serial.printf("Clave incorrecta\n");
          if (intentosFallidos >= MAX_INTENTOS) {
            cambiarEstado(ALARMA);
          } else {
            cambiarEstado(BLOQUEADA);
          }
        }
      } else if (millis() - ultimaTecla >= TIEMPO_SIN_TECLAS) {
        Serial.printf("Tiempo agotado, se cancela el intento\n");
        cambiarEstado(BLOQUEADA);
      }
      break;

    case ABIERTA:
      mostrarByte(0xF0);                          // L4..L7 verdes
      if (enEstado >= TIEMPO_ABIERTA || flancoC) {
        cambiarEstado(BLOQUEADA);
      }
      break;

    case ALARMA:
      // Titila cada 200 ms: (enEstado / 200) va 0, 1, 2, 3... y % 2 alterna 0 / 1
      if ((enEstado / 200) % 2 == 0) {
        mostrarByte(0x0F);
      } else {
        mostrarByte(0x00);
      }
      if (enEstado >= TIEMPO_ALARMA) {
        intentosFallidos = 0;
        cambiarEstado(BLOQUEADA);
      }
      break;
  }
}
