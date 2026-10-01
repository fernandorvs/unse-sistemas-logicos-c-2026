// ============================================================
// Mi Entrenador Lógico - Clase 8 (versión de referencia de la cátedra)
// Alumno/a: (nombre y apellido)
// Qué hace: un menú con 7 modos que repasan todo el curso.
//   Botón C : pasa al modo siguiente (1 -> 2 -> ... -> 7 -> 1).
//   A y B   : entradas del modo activo (cada modo explica qué hacen).
// Al cambiar de modo, el número de modo se ve 0,8 s en los LEDs.
// Todo es NO bloqueante: no hay ni un delay() en loop().
// ============================================================

// ======================= 1. PINES Y CONSTANTES =======================
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;

const unsigned long CARTEL_MS = 800;         // cuánto se ve el número de modo
const unsigned long PASO_BINARIO_MS = 500;   // velocidad del modo BINARIO

// Numeración de compuertas de la Clase 4: 0=AND 1=OR 2=XOR 3=NAND 4=NOR 5=XNOR
// (6 = A y 7 = B no son compuertas: dejan pasar la entrada)
const int CANTIDAD_COMPUERTAS = 6;

// Decodificador hexa -> 7 segmentos (cátodo común). bit 0 = a ... bit 6 = g (Clase 6)
const uint8_t SEGMENTOS[16] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111,  // 9
  0b01110111,  // A
  0b01111100,  // b
  0b00111001,  // C
  0b01011110,  // d
  0b01111001,  // E
  0b01110001   // F
};

// Los modos del menú. CANTIDAD_MODOS queda último: vale 7.
enum Modo {
  MODO_BINARIO,      // 1: contador automático en binario
  MODO_COMPUERTAS,   // 2: A y B entran a 6 compuertas a la vez
  MODO_TABLA,        // 3: tablas de verdad por el monitor serie
  MODO_REGISTRO,     // 4: rotar e invertir un byte
  MODO_7SEG,         // 5: decodificador a 7 segmentos
  MODO_CONTADOR,     // 6: contador up/down con flancos
  MODO_SEMAFORO,     // 7: máquina de estados de Moore
  CANTIDAD_MODOS
};

// Estados del semáforo y sus luces (L0 = rojo, L1 = "amarillo", L4 = verde)
enum EstadoSemaforo { SEM_VERDE, SEM_AMARILLO, SEM_ROJO };
const unsigned long TIEMPO_VERDE = 4000;
const unsigned long TIEMPO_AMARILLO = 1500;
const unsigned long TIEMPO_ROJO = 4000;
const uint8_t LUZ_ROJA = 0x01;               // L0
const uint8_t LUZ_AMARILLA = 0x02;           // L1
const uint8_t LUZ_VERDE = 0x10;              // L4

// ======================= 2. VARIABLES GLOBALES =======================
// Menú: el modo activo es el ESTADO de la máquina del menú
Modo modo = MODO_BINARIO;
unsigned long inicioModo = 0;                // cuándo entramos al modo actual

// Variables de cada modo. TODAS se reinician en entrarModo().
uint8_t binario = 0;                         // modo 1
bool binarioPausado = false;
unsigned long ultimoPasoBinario = 0;
int entradasAnteriores = -1;                 // modo 2 (-1 = "todavía no imprimí")
int compuertaElegida = 0;                    // modo 3
uint8_t registro = 0;                        // modo 4
int digito = 0;                              // modo 5
uint8_t contador = 0;                        // modo 6
EstadoSemaforo estadoSemaforo = SEM_VERDE;   // modo 7
unsigned long inicioSemaforo = 0;

// ======================= 3. FUNCIONES UTILITARIAS =======================

// Bloque copiado TAL CUAL de la Clase 7 (seApreto y su memoria por botón)
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

// Muestra un byte en los 8 LEDs: bit 0 en L0 ... bit 7 en L7 (Clase 5)
void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

// Imprime los 8 bits, del 7 al 0 (printf no tiene %b) (Clase 5)
void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}

// Imprime un byte en decimal, hexa y binario, y salta de línea
void imprimirByte(uint8_t valor) {
  Serial.printf("%3d = 0x%02X = ", valor, valor);
  imprimirBinario(valor);
  Serial.printf("\n");
}

// Rotación circular a la izquierda: el bit 7 vuelve a entrar por el bit 0 (Clase 5)
uint8_t rotarIzquierda(uint8_t x) {
  return (x << 1) | (x >> 7);
}

// La evaluar() de la Clase 4, ahora con switch. Con return no hace falta break.
bool evaluar(int compuerta, bool a, bool b) {
  switch (compuerta) {
    case 0:  return a && b;       // AND
    case 1:  return a || b;       // OR
    case 2:  return a != b;       // XOR
    case 3:  return !(a && b);    // NAND
    case 4:  return !(a || b);    // NOR
    case 5:  return a == b;       // XNOR
    case 6:  return a;            // la entrada A, tal cual
    default: return b;            // 7: la entrada B, tal cual
  }
}

// Imprime el nombre de la compuerta (Clase 4, ahora con switch)
void imprimirNombre(int compuerta) {
  switch (compuerta) {
    case 0:  Serial.printf("AND");  break;
    case 1:  Serial.printf("OR");   break;
    case 2:  Serial.printf("XOR");  break;
    case 3:  Serial.printf("NAND"); break;
    case 4:  Serial.printf("NOR");  break;
    case 5:  Serial.printf("XNOR"); break;
    case 6:  Serial.printf("A");    break;
    default: Serial.printf("B");    break;
  }
}

// Imprime la tabla de verdad (2 entradas) de la compuerta pedida (Clase 4)
void imprimirTabla(int compuerta) {
  Serial.printf("\nTabla de ");
  imprimirNombre(compuerta);
  Serial.printf("\n A | B | S\n");
  Serial.printf("---+---+---\n");
  for (int a = 0; a <= 1; a++) {
    for (int b = 0; b <= 1; b++) {
      Serial.printf(" %d | %d | %d\n", a, b, evaluar(compuerta, a, b));
    }
  }
}

// ======================= 4. LOS MODOS =======================

// Modo 1 - BINARIO: cuenta solo cada 0,5 s. A = pausa/sigue, B = vuelve a 0.
void modoBinario(bool flancoA, bool flancoB) {
  if (flancoA) {
    binarioPausado = !binarioPausado;
    Serial.printf("Pausado: %d\n", binarioPausado);
  }
  if (flancoB) {
    binario = 0;
    Serial.printf("Vuelve a 0\n");
  }
  if (!binarioPausado && millis() - ultimoPasoBinario >= PASO_BINARIO_MS) {
    ultimoPasoBinario = millis();
    binario++;                                   // 255 + 1 desborda a 0
    imprimirByte(binario);
  }
  mostrarByte(binario);
}

// Modo 2 - COMPUERTAS: A y B (mantenidos) son las entradas.
// Como en la Clase 4: cada LED muestra la compuerta con su mismo número.
// L0..L5 = AND, OR, XOR, NAND, NOR, XNOR.  L6 = A, L7 = B.
// Usa el NIVEL de los botones (no el flanco): por eso lee los pines directo.
void modoCompuertas() {
  bool a = (digitalRead(BOTON_A) == LOW);
  bool b = (digitalRead(BOTON_B) == LOW);
  uint8_t salidas = 0;
  for (int i = 0; i < 8; i++) {
    if (evaluar(i, a, b)) {
      salidas = salidas | (1 << i);              // encender el bit de esa compuerta
    }
  }
  mostrarByte(salidas);

  int entradas = a * 2 + b;                      // 0..3: para imprimir solo si cambian
  if (entradas != entradasAnteriores) {
    entradasAnteriores = entradas;
    Serial.printf("A=%d B=%d -> LEDs ", a, b);
    imprimirBinario(salidas);
    Serial.printf("\n");
  }
}

// Modo 3 - TABLA DE VERDAD: al entrar imprime todas las tablas.
// A = compuerta siguiente, B = anterior. Los LEDs muestran el número (1 a 6).
void modoTabla(bool flancoA, bool flancoB) {
  if (flancoA) {
    compuertaElegida = (compuertaElegida + 1) % CANTIDAD_COMPUERTAS;
    imprimirTabla(compuertaElegida);
  }
  if (flancoB) {
    compuertaElegida = (compuertaElegida + CANTIDAD_COMPUERTAS - 1) % CANTIDAD_COMPUERTAS;
    imprimirTabla(compuertaElegida);
  }
  mostrarByte(compuertaElegida + 1);
}

// Modo 4 - REGISTRO: A = rotar a la izquierda, B = invertir todos los bits.
void modoRegistro(bool flancoA, bool flancoB) {
  if (flancoA) {
    registro = rotarIzquierda(registro);
  }
  if (flancoB) {
    registro = ~registro;
  }
  if (flancoA || flancoB) {
    imprimirByte(registro);
  }
  mostrarByte(registro);
}

// Modo 5 - 7 SEGMENTOS: A = dígito siguiente, B = anterior (0 a F).
void modo7Seg(bool flancoA, bool flancoB) {
  if (flancoA) {
    digito = (digito + 1) % 16;
  }
  if (flancoB) {
    digito = (digito + 15) % 16;                 // +15 es lo mismo que -1 en módulo 16
  }
  if (flancoA || flancoB) {
    Serial.printf("Digito %X -> segmentos gfedcba = ", digito);
    imprimirBinario(SEGMENTOS[digito]);
    Serial.printf("\n");
  }
  mostrarByte(SEGMENTOS[digito]);
}

// Modo 6 - CONTADOR: A = +1, B = -1 (de 8 bits: 255 + 1 = 0, 0 - 1 = 255).
void modoContador(bool flancoA, bool flancoB) {
  if (flancoA) {
    contador++;
  }
  if (flancoB) {
    contador--;
  }
  if (flancoA || flancoB) {
    imprimirByte(contador);
  }
  mostrarByte(contador);
}

// Modo 7 - SEMÁFORO: máquina de Moore temporizada con millis().
void cambiarSemaforo(EstadoSemaforo nuevo) {
  estadoSemaforo = nuevo;
  inicioSemaforo = millis();
  switch (nuevo) {
    case SEM_VERDE:    Serial.printf("Semaforo: VERDE\n");    break;
    case SEM_AMARILLO: Serial.printf("Semaforo: AMARILLO\n"); break;
    case SEM_ROJO:     Serial.printf("Semaforo: ROJO\n");     break;
  }
}

void modoSemaforo() {
  unsigned long enEstado = millis() - inicioSemaforo;
  switch (estadoSemaforo) {
    case SEM_VERDE:
      mostrarByte(LUZ_VERDE);                    // salida (Moore)
      if (enEstado >= TIEMPO_VERDE) {            // transición
        cambiarSemaforo(SEM_AMARILLO);
      }
      break;
    case SEM_AMARILLO:
      mostrarByte(LUZ_AMARILLA);
      if (enEstado >= TIEMPO_AMARILLO) {
        cambiarSemaforo(SEM_ROJO);
      }
      break;
    case SEM_ROJO:
      mostrarByte(LUZ_ROJA);
      if (enEstado >= TIEMPO_ROJO) {
        cambiarSemaforo(SEM_VERDE);
      }
      break;
  }
}

// ======================= 5. EL MENÚ =======================

// Entra a un modo: lo anuncia, arranca el cartel y REINICIA sus variables.
void entrarModo(Modo nuevo) {
  modo = nuevo;
  inicioModo = millis();
  Serial.printf("\n===== Modo %d de %d: ", modo + 1, (int)CANTIDAD_MODOS);
  switch (modo) {
    case MODO_BINARIO:
      Serial.printf("BINARIO (A: pausa/sigue, B: vuelve a 0) =====\n");
      binario = 0;
      binarioPausado = false;
      ultimoPasoBinario = millis();
      break;
    case MODO_COMPUERTAS:
      Serial.printf("COMPUERTAS (mantener A y B) =====\n");
      Serial.printf("L0=AND L1=OR L2=XOR L3=NAND L4=NOR L5=XNOR L6=A L7=B\n");
      entradasAnteriores = -1;
      break;
    case MODO_TABLA:
      Serial.printf("TABLA DE VERDAD (A: siguiente, B: anterior) =====\n");
      for (int c = 0; c < CANTIDAD_COMPUERTAS; c++) {
        imprimirTabla(c);
      }
      compuertaElegida = 0;
      break;
    case MODO_REGISTRO:
      Serial.printf("REGISTRO (A: rotar, B: invertir) =====\n");
      registro = 0x01;
      break;
    case MODO_7SEG:
      Serial.printf("7 SEGMENTOS (A: +1, B: -1) =====\n");
      digito = 0;
      break;
    case MODO_CONTADOR:
      Serial.printf("CONTADOR (A: +1, B: -1) =====\n");
      contador = 0;
      break;
    case MODO_SEMAFORO:
      Serial.printf("SEMAFORO (automatico) =====\n");
      cambiarSemaforo(SEM_VERDE);
      break;
    default:
      break;
  }
}

// ======================= 6. SETUP Y LOOP =======================
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  for (int i = 0; i < CANT_BOTONES; i++) {
    pinMode(BOTONES[i], INPUT_PULLUP);
  }
  Serial.printf("=== Entrenador Logico de Ana - SL UNSE 2026 ===\n");
  Serial.printf("    C = cambiar de modo\n");
  entrarModo(MODO_BINARIO);
}

void loop() {
  // 1) Leer los tres botones UNA sola vez por vuelta
  bool flancoA = seApreto(BOT_A);
  bool flancoB = seApreto(BOT_B);
  bool flancoC = seApreto(BOT_C);

  // 2) Máquina de estados del menú: C pasa al modo siguiente (7 vuelve a 1)
  if (flancoC) {
    entrarModo((Modo)((modo + 1) % CANTIDAD_MODOS));
  }

  // 3) Durante el cartel se ve el número de modo; después, el modo activo
  if (millis() - inicioModo < CARTEL_MS) {
    mostrarByte(modo + 1);
  } else {
    switch (modo) {
      case MODO_BINARIO:    modoBinario(flancoA, flancoB);   break;
      case MODO_COMPUERTAS: modoCompuertas();                break;
      case MODO_TABLA:      modoTabla(flancoA, flancoB);     break;
      case MODO_REGISTRO:   modoRegistro(flancoA, flancoB);  break;
      case MODO_7SEG:       modo7Seg(flancoA, flancoB);      break;
      case MODO_CONTADOR:   modoContador(flancoA, flancoB);  break;
      case MODO_SEMAFORO:   modoSemaforo();                  break;
      default:              entrarModo(MODO_BINARIO);        break;   // nunca debería pasar
    }
  }
}
