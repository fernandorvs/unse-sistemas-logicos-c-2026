# Clase 6 — Arreglos y Combinacionales

**Duración:** 3 hs
**Objetivo:** dominar los arreglos de C y usarlos para lo que mejor hacen en Sistemas Lógicos: **guardar tablas**. Al salir, el entrenador muestra números en un display de 7 segmentos con un decodificador que es... una sola línea de código.

---

## Por qué arrancamos así

En la teoría, un decodificador, un multiplexor o cualquier circuito combinacional se describe con una **tabla de verdad**: para cada combinación de entradas, cuál es la salida.
Hasta ahora, para programar una tabla se escribían muchos `if`. Hoy se aprende a **guardar la tabla entera en el programa** y consultarla. Es más corto, más fácil de cambiar y es exactamente lo que hace una memoria ROM en un circuito real.

En la Clase 4 ya se usó un arreglo, `LEDS[8]`, como una lista de pines. Hoy lo vemos a fondo.

---

## Contenido

### 1. El problema: muchas variables iguales (10 min)

Supongamos que se quiere guardar la temperatura máxima de cada día de la semana. Con lo visto hasta ahora:

```c
int tempLunes = 31;
int tempMartes = 34;
int tempMiercoles = 29;
// ... y así hasta el domingo
```

¿Y para calcular el promedio? `(tempLunes + tempMartes + tempMiercoles + ...) / 7`. ¿Y si fueran los 365 días del año? Imposible.

Lo que necesitamos es **una sola variable que guarde muchos valores**, y poder elegir cuál con un número. Eso es un **arreglo** (en inglés, *array*).

### 2. Arreglos desde cero (35 min)

#### Declarar e inicializar

```c
int temperaturas[7] = {31, 34, 29, 38, 36, 27, 33};
```

| Parte | Qué significa |
|---|---|
| `int` | El **tipo** de cada elemento. Todos los elementos de un arreglo son del mismo tipo. |
| `temperaturas` | El **nombre** del arreglo. |
| `[7]` | El **tamaño**: cuántos elementos tiene. Es **fijo**: no puede crecer ni achicarse mientras corre el programa. |
| `{31, 34, ...}` | Los **valores iniciales**, separados por comas, en orden. |

Se lo puede imaginar como una fila de 7 cajitas numeradas:

```
 índice:      0     1     2     3     4     5     6
           ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 valor:    │ 31  │ 34  │ 29  │ 38  │ 36  │ 27  │ 33  │
           └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
```

#### Leer y escribir un elemento: el índice

Para usar una cajita, se escribe el nombre y entre corchetes el **índice** (la posición):

```c
Serial.printf("%d\n", temperaturas[0]);   // imprime 31 (el PRIMERO)
Serial.printf("%d\n", temperaturas[6]);   // imprime 33 (el ÚLTIMO)
temperaturas[2] = 40;                      // cambia el 29 por 40
```

> ⚠️ **El primer índice es 0, no 1.** Un arreglo de 7 elementos tiene índices **0, 1, 2, 3, 4, 5, 6**. El último es siempre `tamaño - 1`.

¿Por qué desde 0? Porque el índice dice **cuántos lugares hay que correrse desde el principio**. El primero está a 0 lugares del principio. Es la misma razón por la que el LED de menos peso se llama `L0` y el bit de menos peso es el bit 0.

#### Recorrer un arreglo con `for`

El `for` de la Clase 4 y los arreglos están hechos el uno para el otro:

```c
for (int i = 0; i < 7; i++) {
  Serial.printf("temperaturas[%d] = %d\n", i, temperaturas[i]);
}
```

Notar la condición: `i < 7`, **no** `i <= 7`. Así `i` toma los valores 0 a 6, exactamente los índices válidos.

Para no repetir el número 7 por todo el programa, se acostumbra guardarlo en una constante:

```c
const int DIAS = 7;
int temperaturas[DIAS] = {31, 34, 29, 38, 36, 27, 33};

for (int i = 0; i < DIAS; i++) { ... }
```

#### Dos recetas clásicas: suma y máximo

```c
// Promedio: acumular en una variable y dividir
int suma = 0;
for (int i = 0; i < DIAS; i++) {
  suma = suma + temperaturas[i];
}
int promedio = suma / DIAS;

// Máximo: suponer que el primero es el mayor y comparar con los demás
int maxima = temperaturas[0];
for (int i = 1; i < DIAS; i++) {
  if (temperaturas[i] > maxima) {
    maxima = temperaturas[i];
  }
}
```

Todo junto en [`ejemplos/01_arreglo_temperaturas`](ejemplos/01_arreglo_temperaturas/sketch.ino). Correrlo y después agregarle el **mínimo**.

#### Salirse del arreglo: el error silencioso

¿Qué pasa si se escribe `temperaturas[7]` o `temperaturas[100]`?

En muchos lenguajes el programa se detiene con un error. **En C, no**: el compilador no avisa y el programa lee (o, peor, **escribe**) la memoria que está al lado del arreglo, que pertenece a otra cosa. El resultado puede ser un número basura, otra variable que cambia "sola", o que el ESP32 se reinicie.

```c
int temperaturas[7] = {31, 34, 29, 38, 36, 27, 33};
Serial.printf("%d\n", temperaturas[7]);   // ¡NO EXISTE! Imprime cualquier cosa
```

> ⚠️ **Regla de oro:** antes de usar `arreglo[i]`, asegurarse de que `i` esté entre `0` y `tamaño - 1`. El error más común es `for (int i = 0; i <= 7; i++)` en un arreglo de 7.

#### Arreglos `const`: tablas que no cambian

Si el contenido del arreglo **nunca** debe cambiar (como el pinout), se lo declara `const`:

```c
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
```

Si después, por error, se escribe `LEDS[0] = 5;`, el compilador se queja. Es una protección gratis. Las tablas de hoy van a ser todas `const`.

**Resumen de arreglos:**

| Concepto | Ejemplo |
|---|---|
| Declarar con valores | `int notas[5] = {7, 4, 9, 10, 6};` |
| Tamaño en una constante | `const int N = 5;` y `int notas[N];` |
| Primer elemento | `notas[0]` |
| Último elemento | `notas[N - 1]` |
| Recorrer | `for (int i = 0; i < N; i++) { ... notas[i] ... }` |
| Tabla que no cambia | `const uint8_t TABLA[4] = {...};` |
| Índice fuera de rango | **No hay aviso.** Hay que cuidarlo. |

### 3. La idea central: un circuito combinacional es una tabla (15 min)

Un circuito **combinacional** es aquel cuya salida depende **solamente de las entradas actuales**. Por eso se puede describir completo con una tabla de verdad: una fila por cada combinación de entradas.

Ahora conviene mirar un arreglo con otros ojos:

```
   Tabla de verdad                       Arreglo en C
 entrada │ salida                   índice │ contenido
 ────────┼────────                  ───────┼──────────
    0    │  ...                        0   │   ...
    1    │  ...          ═══►          1   │   ...
    2    │  ...                        2   │   ...
    3    │  ...                        3   │   ...
```

**¡Es lo mismo!** La **entrada** del circuito es el **índice**, y la **salida** es el **contenido** de esa posición.

```c
salida = TABLA[entrada];
```

Esa única línea reemplaza a cualquier circuito combinacional, por grande que sea. A este truco se lo llama **tabla de búsqueda** (*lookup table*, **LUT**).

> 💡 **Conexión con la teoría:** así funciona una **memoria ROM** usada como circuito combinacional: las entradas van a las líneas de **dirección** y las salidas salen por las líneas de **datos**. Grabar la ROM es elegir qué función hace. Las **FPGA** (los chips de lógica programable) están hechas de miles de **LUT** pequeñas, que son exactamente esto: tablas de verdad guardadas en memoria.

### 4. Decodificador 2 → 4, de dos formas (20 min)

Un decodificador 2 → 4 tiene 2 entradas (B, A) y 4 salidas (Y0..Y3). Se enciende **solo** la salida cuyo número coincide con la entrada (salida *one-hot*).

| B | A | Entrada | Y3 | Y2 | Y1 | Y0 |
|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 1 | 0 | 0 | 1 | 0 |
| 1 | 0 | 2 | 0 | 1 | 0 | 0 |
| 1 | 1 | 3 | 1 | 0 | 0 | 0 |

En la placa: pulsador A = bit 0, pulsador B = bit 1, salidas en `L0`..`L3`.

**Primero, juntar las entradas en un número.** Si `a` y `b` son `bool` (0 o 1):

```c
bool a = (digitalRead(BOTON_A) == LOW);   // apretado = 1
bool b = (digitalRead(BOTON_B) == LOW);
int entrada = b * 2 + a;                  // B pesa 2, A pesa 1
```

Es la conversión binario → decimal de toda la vida: cada bit por su peso. Con lo de la Clase 5 también vale `(b << 1) | a`.

**Forma 1: con `if`** (como escribir la tabla fila por fila):

```c
uint8_t decoConIf(bool a, bool b) {
  if (!b && !a) {
    return 0b0001;
  } else if (!b && a) {
    return 0b0010;
  } else if (b && !a) {
    return 0b0100;
  } else {
    return 0b1000;
  }
}
```

**Forma 2: con tabla:**

```c
const uint8_t DECO_2A4[4] = {0b0001, 0b0010, 0b0100, 0b1000};

uint8_t decoConTabla(bool a, bool b) {
  int entrada = b * 2 + a;
  return DECO_2A4[entrada];
}
```

Y para ver el resultado, la función de la Clase 5: `mostrarByte(decoConTabla(a, b));`.

Programa completo, que además verifica que las dos formas coinciden: [`ejemplos/03_decodificador_2a4`](ejemplos/03_decodificador_2a4/sketch.ino).

| | Con `if` | Con tabla |
|---|---|---|
| Líneas | Una rama por fila | Una línea, más la tabla |
| Para cambiar la función | Reescribir condiciones | Cambiar números en la tabla |
| Para 16 entradas | 16 ramas... | Igual de corto |
| Se parece a | Las compuertas (ecuaciones) | Una ROM |

**Pregunta para pensar:** ¿cómo se haría un decodificador 3 → 8? ¿Cuántos `if` se necesitarían? ¿Y cuántos elementos tendría la tabla? *(Pista: con la Clase 5 ni siquiera hace falta tabla: `1 << entrada`.)*

### 5. Decodificador hexa → 7 segmentos (30 min)

#### El display

Un display de 7 segmentos son 7 LEDs con forma de rayita, llamados `a` a `g` (más un punto, `dp`):

```
      aaa
     f   b
     f   b
      ggg
     e   c
     e   c
      ddd   dp
```

En el display de **cátodo común**, todos los cátodos están unidos a GND, y cada segmento se enciende poniendo un **1** en su pin.

#### Usarlo en Wokwi

En la placa de cátedra el display está conectado **a los mismos pines que los LEDs**: `a` = L0, `b` = L1, ..., `g` = L6, `dp` = L7. Así que el byte que se manda con `mostrarByte` se ve **a la vez** en los LEDs y en el display.

Para tenerlo en Wokwi:

1. Abrir el proyecto y usar **Save a copy** (para no pisar el de la clase anterior).
2. Abrir la pestaña `diagram.json`, **borrar todo** y pegar el contenido de [`placa/diagram_7seg.json`](../../placa/diagram_7seg.json).
3. Apretar **Save**. Ahora aparece el display al lado de la placa.

> En la **placa real** no hay display: se ve el mismo patrón en los 8 LEDs (L0 = segmento a, L1 = segmento b, etc.). Con un poco de práctica, ¡se aprende a "leer" los dígitos en los LEDs!

#### La tabla

Para mostrar un dígito hay que decidir qué segmentos se encienden. Cada dígito es un byte con un bit por segmento: **bit 0 = a, bit 1 = b, ..., bit 6 = g** (el bit 7 es el punto, que dejamos en 0).

Ejemplo: el **2** usa los segmentos a, b, g, e, d:

```
 bit:       7   6   5   4   3   2   1   0
 segmento: dp   g   f   e   d   c   b   a
 valor:     0   1   0   1   1   0   1   1    →  0b01011011
```

Haciendo lo mismo con los 16 dígitos hexadecimales:

```c
const uint8_t SEGMENTOS[16] = {
  0b00111111,  // 0: a b c d e f
  0b00000110,  // 1: b c
  0b01011011,  // 2: a b d e g
  0b01001111,  // 3: a b c d g
  0b01100110,  // 4: b c f g
  0b01101101,  // 5: a c d f g
  0b01111101,  // 6: a c d e f g
  0b00000111,  // 7: a b c
  0b01111111,  // 8: todos
  0b01101111,  // 9: a b c d f g
  0b01110111,  // A: a b c e f g
  0b01111100,  // b: c d e f g
  0b00111001,  // C: a d e f
  0b01011110,  // d: b c d e g
  0b01111001,  // E: a d e f g
  0b01110001   // F: a e f g
};
```

(La `b` y la `d` van en minúscula porque en mayúscula se confundirían con el 8 y el 0.)

Y el decodificador completo es:

```c
mostrarByte(SEGMENTOS[n]);
```

Correr [`ejemplos/02_display_hexa`](ejemplos/02_display_hexa/sketch.ino): muestra 0, 1, ..., 9, A, b, C, d, E, F.

> 💡 **Conexión con la teoría:** en la teoría el decodificador BCD → 7 segmentos (el famoso **7447/7448**) se diseña con 7 mapas de Karnaugh, uno por segmento. Aquí la tabla de verdad **es** el programa: no hace falta simplificar nada. La simplificación ahorra compuertas; en una memoria, todas las tablas ocupan lo mismo.

**Para probar:** cambiar el patrón del 7 para que también encienda el segmento `f` (hay displays que lo dibujan así). ¿Cuántos caracteres de código hubo que tocar?

### 6. Multiplexor 4 → 1 (15 min)

Un **multiplexor** (MUX) es un "selector": tiene varias entradas de datos (D0..D3), unas entradas de **selección** (S1, S0) y una sola salida, que **copia el dato elegido**.

| S1 (B) | S0 (A) | Y |
|---|---|---|
| 0 | 0 | D0 |
| 0 | 1 | D1 |
| 1 | 0 | D2 |
| 1 | 1 | D3 |

En C, los datos son un arreglo y el selector es el índice:

```c
bool datos[4] = {0, 1, 1, 0};   // D0, D1, D2, D3

int sel = b * 2 + a;
bool y = datos[sel];            // ¡el multiplexor entero!
digitalWrite(LEDS[7], y);
```

Programa completo en [`ejemplos/04_multiplexor`](ejemplos/04_multiplexor/sketch.ino): los datos se ven en `L0`..`L3`, el selector se elige con A y B, y la salida en `L7`.

> 💡 **Conexión con la teoría:** notar la diferencia con el decodificador. En el decodificador la tabla es **fija** (`const`) y el índice elige la salida. En el MUX los **datos** son la entrada que cambia y el índice elige **cuál pasa**. Indexar un arreglo es, literalmente, multiplexar.

### 7. Cualquier función de 3 variables con un arreglo (15 min)

Si una tabla de búsqueda sirve para un decodificador, sirve para **cualquier** función lógica. Una función de 3 variables (C, B, A) tiene 8 filas, así que cabe en un arreglo de 8 `bool`:

| Fila | C | B | A | F |
|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 |
| 1 | 0 | 0 | 1 | 1 |
| 2 | 0 | 1 | 0 | 1 |
| 3 | 0 | 1 | 1 | 0 |
| 4 | 1 | 0 | 0 | 1 |
| 5 | 1 | 0 | 1 | 0 |
| 6 | 1 | 1 | 0 | 0 |
| 7 | 1 | 1 | 1 | 1 |

```c
//                   fila: 0  1  2  3  4  5  6  7
const bool F[8] =         {0, 1, 1, 0, 1, 0, 0, 1};

void loop() {
  bool a = (digitalRead(BOTON_A) == LOW);
  bool b = (digitalRead(BOTON_B) == LOW);
  bool c = (digitalRead(BOTON_C) == LOW);

  int fila = c * 4 + b * 2 + a;   // C pesa 4, B pesa 2, A pesa 1
  digitalWrite(LEDS[0], F[fila]);
}
```

¿Qué función es? Si se mira bien, es `A ^ B ^ C` (la **paridad impar**, sale 1 cuando hay una cantidad impar de unos). Pero el programa **no lo sabe ni le importa**: solo copia la columna F.

Para que la placa haga **otra** función, por ejemplo la **mayoría** (sale 1 si al menos dos entradas están en 1), no se toca el código, se cambia la tabla:

```c
const bool F[8] = {0, 0, 0, 1, 0, 1, 1, 1};   // mayoría
```

> 💡 **Conexión con la teoría:** esto es **regrabar una ROM**. El circuito (el programa) es el mismo; lo que cambia es el contenido de la memoria. Por eso con una ROM de 8 × 1 se puede hacer **cualquiera** de las 256 funciones posibles de 3 variables. ¿Por qué 256? Porque la columna F tiene 8 bits, y con 8 bits hay 2⁸ = 256 combinaciones.

**Para probar:** armar este programa completo (con los `pinMode` de los 3 pulsadores y L0) y cargarle la tabla de la función que esté viendo la teoría esta semana. Comprobar las 8 filas con los pulsadores.

### 8. Ejercicios (40 min)

**Ejercicio 1 — BCD con error.**
Un decodificador **BCD** solo acepta 0..9. Hacer una tabla de 16 elementos donde las entradas 10..15 muestren una **E** (de error) en el display. Recorrer las 16 entradas cada 800 ms e imprimir cuáles son válidas. *Sin* usar `if` para elegir el patrón: la decisión tiene que estar en la tabla.

**Ejercicio 2 — Codificador de prioridad 8 → 3.**
Es el inverso del decodificador: recibe un byte y devuelve la **posición del bit en 1 de más peso** (0..7), o `-1` si no hay ninguno. Escribir `int codificarPrioridad(uint8_t entrada)` buscando con un `for` desde el bit 7 hacia abajo. Probarlo con un arreglo de entradas de prueba (por ejemplo `0b00101000` debe dar 5), mostrando la entrada en los LEDs e imprimiendo la salida en decimal y en 3 bits.

**Ejercicio 3 — Código Gray, tabla contra fórmula.**
El **código Gray** es una forma de contar en la que entre un número y el siguiente cambia **un solo bit**.
- Hacer una tabla `const uint8_t GRAY[16]` con el Gray de 0 a 15 (sacarla de la teoría).
- Hacer la función `uint8_t grayConFormula(uint8_t n)` que devuelva `n ^ (n >> 1)`.
- Imprimir una tabla comparando las dos para los 16 valores y contar las diferencias (tienen que ser 0).
- **Desafío:** hacer también la conversión inversa, Gray → binario, con una segunda tabla.
- Al final, contar de 0 a 15 mostrando el binario en `L4`..`L7` y el Gray en `L0`..`L3`. ¿Qué se nota en los LEDs rojos?

**Ejercicio 4 — Comparador de 2 bits.**
X (0..3) se cambia con A, Y (0..3) se cambia con B (con el patrón provisorio `delay(250)`). Mostrar X en `L0`-`L1`, Y en `L2`-`L3`, y encender `L5` si X < Y, `L6` si X == Y y `L7` si X > Y. Hacer el comparador con una **tabla de 16 elementos** indexada con `y * 4 + x`, y verificarla al arrancar contra los operadores `<`, `==` y `>` de C.

---

## ⭐ TP Clase 6 — El entrenador decodifica

Partir del TP de la Clase 5 y usar la placa con display ([`placa/diagram_7seg.json`](../../placa/diagram_7seg.json)). Escribir un programa que:

1. Empiece con el **comentario de encabezado** (nombre, clase, qué hace).
2. Tenga una variable `valor` que vaya de **0 a 15**.
3. El pulsador **A** le **suma 1** y el **B** le **resta 1**, con vuelta: después de 15 viene 0, y antes de 0 viene 15. Usar el patrón provisorio "si está apretado, hacer la acción y `delay(250)`".
4. Muestre `valor` en el display usando la tabla `SEGMENTOS[16]` y `mostrarByte`.
5. Cada vez que cambia, imprima exactamente así (usando `imprimirBinario`):
   ```
   Valor: 11 (0xB) -> segmentos 0b01111100
   ```
6. Al arrancar muestre un cartel y el valor inicial (0).

Entrega: link del proyecto Wokwi `SL2026 - Apellido - Clase 6`.

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Declarar e inicializar un arreglo, y decir cuáles son sus índices válidos.
- [ ] Recorrer un arreglo con `for` para sumar, promediar y buscar el máximo.
- [ ] Explicar qué pasa en C si se usa un índice fuera del arreglo.
- [ ] Explicar por qué un circuito combinacional se puede reemplazar por una tabla (entrada = índice, salida = contenido).
- [ ] Armar el índice a partir de varias entradas (`c * 4 + b * 2 + a`).
- [ ] Hacer un decodificador, un multiplexor y una función de 3 variables con arreglos.
- [ ] Armar el patrón de 7 segmentos de un dígito y usar el display en Wokwi.

---

## Material de la clase

- [`ejemplos/`](ejemplos/): arreglo de temperaturas, display hexadecimal, decodificador 2 → 4 (con `if` y con tabla) y multiplexor 4 → 1.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
