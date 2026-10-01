// ============================================================
// Entrenador Lógico - Clase 3
// Alumno/a: (nombre y apellido)
// Qué hace: lee los pulsadores A y B como entradas lógicas y
//           muestra en los LEDs el resultado de 6 compuertas:
//             L0 = AND    L1 = OR     L2 = XOR    L3 = NAND
//             L4 = NOR    L5 = XNOR   L6 = A      L7 = B
//           Cada medio segundo imprime el estado completo.
// ============================================================

const int L0 = 4;
const int L1 = 16;
const int L2 = 17;
const int L3 = 18;
const int L4 = 19;
const int L5 = 21;
const int L6 = 22;
const int L7 = 23;
const int BOTON_A = 32;
const int BOTON_B = 33;

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
  pinMode(BOTON_A, INPUT_PULLUP);   // apretado = LOW
  pinMode(BOTON_B, INPUT_PULLUP);

  Serial.printf("==============================\n");
  Serial.printf("  Entrenador Logico de Ana\n");
  Serial.printf("  Sistemas Logicos - UNSE 2026\n");
  Serial.printf("==============================\n");
  Serial.printf("Modo: compuertas logicas (entradas A y B)\n\n");
}

void loop() {
  // Entradas: el pull-up invierte la lógica, así que negamos la lectura
  bool a = !digitalRead(BOTON_A);
  bool b = !digitalRead(BOTON_B);

  // Compuertas
  // (no podemos llamarlas and, or, xor: son palabras reservadas)
  bool salAnd  = a && b;       // AND
  bool salOr   = a || b;       // OR
  bool salXor  = a != b;       // XOR: 1 si son distintas
  bool salNand = !(a && b);    // NAND
  bool salNor  = !(a || b);    // NOR
  bool salXnor = a == b;       // XNOR: 1 si son iguales

  // Salidas
  digitalWrite(L0, salAnd);
  digitalWrite(L1, salOr);
  digitalWrite(L2, salXor);
  digitalWrite(L3, salNand);
  digitalWrite(L4, salNor);
  digitalWrite(L5, salXnor);
  digitalWrite(L6, a);
  digitalWrite(L7, b);

  Serial.printf("A=%d B=%d | AND=%d OR=%d XOR=%d NAND=%d NOR=%d XNOR=%d\n",
                a, b, salAnd, salOr, salXor, salNand, salNor, salXnor);

  delay(500);
}
