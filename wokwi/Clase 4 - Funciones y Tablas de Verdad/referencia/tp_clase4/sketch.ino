// ============================================================
// Entrenador Lógico - Clase 4
// Alumno/a: (nombre y apellido)
// Qué hace: al arrancar imprime las tablas de verdad de AND, OR,
//           XOR y NAND, y la de F = A·B + !C (3 variables).
//           Después sigue en modo compuertas (como el TP3):
//             L0 = AND    L1 = OR     L2 = XOR    L3 = NAND
//             L4 = NOR    L5 = XNOR   L6 = A      L7 = B
//           y cada medio segundo imprime el estado completo.
// ============================================================

#include <Arduino.h>

// ---------- Pines ----------
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;

// ---------- Compuertas ----------
// Cada compuerta tiene un número:
//   0 = AND    1 = OR     2 = XOR    3 = NAND
//   4 = NOR    5 = XNOR   6 = A      7 = B
// Ese número es también el LED donde se muestra en el modo compuertas.
// (6 y 7 no son compuertas: dejan pasar la entrada, como en el TP3.)
// No las podemos llamar and, or, xor: son palabras reservadas.

bool compuertaAND(bool a, bool b) { return a && b; }
bool compuertaOR(bool a, bool b)  { return a || b; }
bool compuertaXOR(bool a, bool b) { return a != b; }
bool compuertaNAND(bool a, bool b) { return !(a && b); }
bool compuertaNOR(bool a, bool b)  { return !(a || b); }
bool compuertaXNOR(bool a, bool b) { return a == b; }

// Evalúa la compuerta número 'compuerta' con las entradas a y b
bool evaluar(int compuerta, bool a, bool b) {
  if (compuerta == 0) {
    return compuertaAND(a, b);
  } else if (compuerta == 1) {
    return compuertaOR(a, b);
  } else if (compuerta == 2) {
    return compuertaXOR(a, b);
  } else if (compuerta == 3) {
    return compuertaNAND(a, b);
  } else if (compuerta == 4) {
    return compuertaNOR(a, b);
  } else if (compuerta == 5) {
    return compuertaXNOR(a, b);
  } else if (compuerta == 6) {
    return a;                     // la entrada A, tal cual
  } else {
    return b;                     // la entrada B, tal cual
  }
}

// Imprime el nombre de la compuerta número 'compuerta'
void imprimirNombre(int compuerta) {
  if (compuerta == 0) {
    Serial.printf("AND");
  } else if (compuerta == 1) {
    Serial.printf("OR");
  } else if (compuerta == 2) {
    Serial.printf("XOR");
  } else if (compuerta == 3) {
    Serial.printf("NAND");
  } else if (compuerta == 4) {
    Serial.printf("NOR");
  } else if (compuerta == 5) {
    Serial.printf("XNOR");
  } else if (compuerta == 6) {
    Serial.printf("A");
  } else {
    Serial.printf("B");
  }
}

// Imprime la tabla de verdad (2 entradas) de la compuerta pedida
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

// ---------- Función de 3 variables (de la teoría) ----------
// F = A·B + C'
bool funcionF(bool a, bool b, bool c) {
  return (a && b) || !c;
}

void imprimirTablaF() {
  Serial.printf("\nTabla de F = A.B + !C\n");
  Serial.printf(" A | B | C | F\n");
  Serial.printf("---+---+---+---\n");
  for (int fila = 0; fila < 8; fila++) {
    int a = (fila / 4) % 2;       // A es el bit de más peso
    int b = (fila / 2) % 2;
    int c = fila % 2;             // C es el bit de menos peso
    Serial.printf(" %d | %d | %d | %d\n", a, b, c, funcionF(a, b, c));
  }
}

// ---------- Presentación ----------
void imprimirCartel() {
  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico de Ana\n");
  Serial.printf("  Sistemas Logicos - UNSE 2026\n");
  Serial.printf("  Clase 4: tablas de verdad\n");
  Serial.printf("==============================\n");
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
  pinMode(BOTON_A, INPUT_PULLUP);
  pinMode(BOTON_B, INPUT_PULLUP);

  imprimirCartel();

  // Tablas de AND (0), OR (1), XOR (2) y NAND (3)
  for (int compuerta = 0; compuerta <= 3; compuerta++) {
    imprimirTabla(compuerta);
  }
  imprimirTablaF();

  Serial.printf("\nModo compuertas: apretar A y B\n");
}

void loop() {
  bool a = !digitalRead(BOTON_A);   // apretado = LOW, por eso el !
  bool b = !digitalRead(BOTON_B);

  // Cada LED muestra la compuerta con su mismo número
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], evaluar(i, a, b));
  }

  // Estado completo en una línea: A=1 B=0 | AND=0 OR=1 XOR=1 ...
  Serial.printf("A=%d B=%d |", a, b);
  for (int i = 0; i <= 5; i++) {          // solo las 6 compuertas
    Serial.printf(" ");
    imprimirNombre(i);
    Serial.printf("=%d", evaluar(i, a, b));
  }
  Serial.printf("\n");

  delay(500);
}
