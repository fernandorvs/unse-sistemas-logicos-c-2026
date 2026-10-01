// Clase 6 - Ejemplo 1: Arreglos en general
// Guarda 7 temperaturas (una por día de la semana) en un arreglo
// y calcula el promedio, la máxima y en qué día ocurrió.

const int DIAS = 7;                                   // tamaño del arreglo
int temperaturas[DIAS] = {31, 34, 29, 38, 36, 27, 33}; // grados, de lunes a domingo

void setup() {
  Serial.begin(115200);

  // 1) Recorrer el arreglo con for: el índice va de 0 a DIAS - 1
  Serial.printf("Temperaturas de la semana:\n");
  for (int i = 0; i < DIAS; i++) {
    Serial.printf("  temperaturas[%d] = %d\n", i, temperaturas[i]);
  }

  // 2) Promedio: sumar todos y dividir por la cantidad
  int suma = 0;
  for (int i = 0; i < DIAS; i++) {
    suma = suma + temperaturas[i];
  }
  Serial.printf("Suma: %d   Promedio: %d grados\n", suma, suma / DIAS);

  // 3) Máximo: suponemos que el primero es el mayor y comparamos con el resto
  int maxima = temperaturas[0];
  int diaMaximo = 0;
  for (int i = 1; i < DIAS; i++) {
    if (temperaturas[i] > maxima) {
      maxima = temperaturas[i];
      diaMaximo = i;
    }
  }
  Serial.printf("Maxima: %d grados, en el dia %d (0 = lunes)\n", maxima, diaMaximo);

  // 4) Un elemento del arreglo se puede modificar como cualquier variable
  temperaturas[2] = 40;
  Serial.printf("Corregido: temperaturas[2] ahora vale %d\n", temperaturas[2]);

  // 5) PELIGRO: temperaturas[7] NO existe (los índices válidos son 0..6).
  //    C no avisa: leería basura de la memoria. ¡No hacerlo!
  // Serial.printf("%d\n", temperaturas[7]);   // <- índice fuera del arreglo
}

void loop() {
}
