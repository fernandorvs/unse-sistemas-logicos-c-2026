// Clase 3 - Ejemplo 1: Un botón enciende un LED
// El pulsador A está conectado a GND y usa INPUT_PULLUP:
//   suelto   -> el pin lee HIGH (1)
//   apretado -> el pin lee LOW  (0)    <- ¡lógica invertida!

const int L0 = 4;
const int BOTON_A = 32;

void setup() {
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
  pinMode(BOTON_A, INPUT_PULLUP);   // entrada con resistencia de pull-up interna
}

void loop() {
  int lectura = digitalRead(BOTON_A);   // lo que lee el pin: 1 suelto, 0 apretado
  bool a = !lectura;                    // lo invertimos: a es true si está APRETADO

  Serial.printf("Pin: %d   a: %d\n", lectura, a);

  if (a) {
    digitalWrite(L0, HIGH);
  } else {
    digitalWrite(L0, LOW);
  }

  delay(200);
}
