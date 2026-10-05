// Clase 4 - Ejemplo 2: Primeras funciones
// Tres tipos de función:
//   1) sin parámetros y sin resultado  (void ...())
//   2) con parámetros                   (void ...(int pin, int veces))
//   3) que devuelve un resultado        (int ...(int x) { return ...; })

#include <Arduino.h>

const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7

// 1) Sin parámetros: siempre hace lo mismo
void imprimirSeparador() {
  Serial.printf("------------------------------\n");
}

// 2) Con parámetros: hace lo mismo, pero con los datos que le pasen
void parpadear(int pin, int veces) {
  for (int i = 0; i < veces; i++) {   // i es LOCAL: solo existe aquí adentro
    digitalWrite(pin, HIGH);
    delay(150);
    digitalWrite(pin, LOW);
    delay(150);
  }
}

// 3) Con return: calcula algo y lo devuelve a quien la llamó
int cuadrado(int x) {
  int resultado = x * x;
  return resultado;
}

// Una función que devuelve bool: ¡una compuerta!
bool compuertaAND(bool a, bool b) {
  return a && b;
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

  imprimirSeparador();
  Serial.printf("  Probando funciones\n");
  imprimirSeparador();

  // Llamamos a cuadrado() con distintos valores
  for (int n = 1; n <= 5; n++) {
    Serial.printf("%d al cuadrado = %d\n", n, cuadrado(n));
  }
  imprimirSeparador();

  // Llamamos a la compuerta
  Serial.printf("1 AND 1 = %d\n", compuertaAND(1, 1));
  Serial.printf("1 AND 0 = %d\n", compuertaAND(1, 0));
  imprimirSeparador();
}

void loop() {
  parpadear(LEDS[0], 3);   // L0 parpadea 3 veces
  parpadear(LEDS[7], 1);   // L7 parpadea 1 vez
  delay(500);
}
