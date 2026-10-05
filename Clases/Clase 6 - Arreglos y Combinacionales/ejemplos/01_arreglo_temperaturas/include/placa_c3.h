// Adaptador para la placa ESP32-C3 Super Mini. NO HACE FALTA TOCARLO.
//
// Los programas del curso están escritos con los pines de la placa de cátedra
// (ESP32 DevKit). El C3 no tiene esos GPIO, así que este archivo traduce cada
// pin del DevKit al pin equivalente del C3 en pinMode, digitalWrite y
// digitalRead. El mismo main.cpp sirve para las dos placas.
//
// PlatformIO lo incluye solo en el entorno esp32c3 (ver platformio.ini).

#pragma once
#include <Arduino.h>

//  Elemento   DevKit   C3
//  L0          4    →   0
//  L1         16    →   1
//  L2         17    →   3
//  L3         18    →   4
//  L4         19    →   5
//  L5         21    →   6
//  L6         22    →   7
//  L7         23    →  10
//  Botón A    32    →  20
//  Botón B    33    →  21
//  Botón C    25    →   9   (es el mismo botón BOOT de la placa)
static inline uint8_t pinC3(uint8_t pinDevKit) {
  switch (pinDevKit) {
    case 4:  return 0;
    case 16: return 1;
    case 17: return 3;
    case 18: return 4;
    case 19: return 5;
    case 21: return 6;
    case 22: return 7;
    case 23: return 10;
    case 32: return 20;
    case 33: return 21;
    case 25: return 9;
    default: return pinDevKit;   // pin que no es de la placa de cátedra
  }
}

#define pinMode(pin, modo)        pinMode(pinC3(pin), (modo))
#define digitalWrite(pin, valor)  digitalWrite(pinC3(pin), (valor))
#define digitalRead(pin)          digitalRead(pinC3(pin))

// En el C3 el monitor serie va por el USB nativo, que tarda en conectarse
// después del reset. Sin esta espera se pierde lo que imprime setup().
// Este es el setup() real: espera hasta 3 s al monitor y llama al setup()
// del programa, que gracias al #define de abajo pasa a llamarse setup_programa().
void setup_programa();

void setup() {
  Serial.begin(115200);
  unsigned long inicio = millis();
  while (!Serial && millis() - inicio < 3000) {
    delay(10);
  }
  setup_programa();
}

#define setup setup_programa
