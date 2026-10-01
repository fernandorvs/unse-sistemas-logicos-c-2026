# Clase 4 — Funciones y Tablas de Verdad

**Duración:** 3 hs
**Objetivo:** dejar de copiar y pegar código. Al salir, se debe saber repetir instrucciones con un bucle, empaquetar una idea en una función y hacer que el ESP32 imprima **la tabla de verdad de cualquier función lógica**.

---

## Por qué esta clase

¿Recuerda el Ejercicio 3 de la Clase 1, el barrido de 4 LEDs? Quedaron 4 renglones casi iguales, y si hubieran sido 8 LEDs, 8 renglones. La promesa fue hacerlo en 5 líneas. Aquí está, y además recorre **los 8** LEDs:

```c
for (int i = 0; i < 8; i++) {
  digitalWrite(LEDS[i], HIGH);
  delay(150);
  digitalWrite(LEDS[i], LOW);
}
```

Al final de la clase se entenderá cada carácter de ese código. Y con las mismas dos herramientas (bucles y funciones) se generarán tablas de verdad completas, como las que se hacen a mano en la teoría, pero sin equivocarse nunca en una fila.

---

## Contenido

### 1. Repetir sin copiar: el bucle `for` (30 min)

Un **bucle** es un bloque de código que se repite. El más usado en C es el `for`. Empecemos con uno que solo cuenta:

```c
for (int i = 0; i < 4; i++) {
  Serial.printf("Vuelta %d\n", i);
}
Serial.printf("Listo\n");
```

Imprime:

```
Vuelta 0
Vuelta 1
Vuelta 2
Vuelta 3
Listo
```

#### La anatomía del `for`

Entre los paréntesis hay **tres partes separadas por punto y coma**:

```
for ( int i = 0 ;   i < 4    ;  i++  ) {
      ─────────     ───────     ───
       inicio      condición    paso
  ...cuerpo: lo que se repite...
}
```

| Parte | Qué hace | Cuándo se ejecuta |
|---|---|---|
| **inicio** `int i = 0` | Crea la variable contadora y le da su primer valor | **Una sola vez**, al entrar |
| **condición** `i < 4` | Si es verdadera, se ejecuta el cuerpo; si es falsa, se sale del `for` | **Antes** de cada vuelta |
| **paso** `i++` | Actualiza el contador (`i++` es `i = i + 1`, de la Clase 2) | **Después** de cada vuelta |

#### Seguir el `for` a mano

Hacer esta tabla en el cuaderno. Es la mejor manera de entender un bucle:

| Momento | `i` | ¿`i < 4`? | ¿Qué pasa? |
|---|---|---|---|
| inicio | 0 | sí | imprime `Vuelta 0` |
| paso | 1 | sí | imprime `Vuelta 1` |
| paso | 2 | sí | imprime `Vuelta 2` |
| paso | 3 | sí | imprime `Vuelta 3` |
| paso | 4 | **no** | sale del `for` → imprime `Listo` |

Notar que el cuerpo se ejecutó **4 veces**, con `i` valiendo 0, 1, 2 y 3. El 4 **no** se ejecuta. Por eso la forma típica de "repetir N veces" es:

```c
for (int i = 0; i < N; i++) { ... }    // i vale 0, 1, ..., N-1
```

Para que el último valor también entre, se usa `<=`:

```c
for (int n = 0; n <= 15; n++) { ... }  // n vale 0, 1, ..., 15
```

> ⚠️ **Nunca poner `;` después del paréntesis del `for`.** Ese `;` es una instrucción vacía, y es **eso** lo que se repite:
> ```c
> for (int i = 0; i < 4; i++);   // repite "nada" 4 veces
> {
>   Serial.printf("Hola\n");      // se ejecuta UNA sola vez
> }
> ```
> Compila sin error, así que cuesta encontrarlo. Y si el cuerpo usa `i`, el error que aparece es raro: *"'i' was not declared in this scope"* (la `i` ya murió con el `for`).

#### El otro bucle: `while`

`while` ("mientras") repite mientras una condición sea verdadera. No tiene inicio ni paso: eso lo maneja el programador.

```c
int potencia = 1;
while (potencia < 100) {
  Serial.printf("%d ", potencia);
  potencia = potencia * 2;
}
// imprime: 1 2 4 8 16 32 64
```

Un uso muy práctico: **esperar a que aprieten un pulsador**.

```c
Serial.printf("Apretar A para empezar\n");
while (digitalRead(BOTON_A) == HIGH) {
  // no hace nada: espera mientras A esté suelto (HIGH)
}
Serial.printf("Arrancamos!\n");
```

| Usar... | cuando... |
|---|---|
| `for` | se sabe **cuántas veces** repetir (8 LEDs, 16 números, 4 filas) |
| `while` | se repite **hasta que pase algo** (un botón, llegar a un valor) |

> ⚠️ Si la condición del `while` nunca se vuelve falsa, el programa queda trabado ahí **para siempre** (un *bucle infinito*). Si en el `while` de arriba se olvida `potencia = potencia * 2;`, `potencia` vale 1 eternamente.

### 2. Primer arreglo: la lista de pines (20 min)

Para el barrido necesitamos que el `for` recorra los pines de los LEDs. Pero los pines son 4, 16, 17, 18, 19, 21, 22, 23: no hay una cuenta que los genere. La solución es guardarlos en una **lista**, que en C se llama **arreglo** (*array*):

```c
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
```

- `LEDS` es el nombre de la lista. `[8]` dice que tiene 8 elementos.
- Cada elemento se lee con su **índice** (su posición) entre corchetes: `LEDS[0]`, `LEDS[1]`, …
- **Los índices empiezan en 0.** El primero es `LEDS[0]` y el último es `LEDS[7]`. `LEDS[8]` **no existe**.

| Índice | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| `LEDS[i]` (GPIO) | 4 | 16 | 17 | 18 | 19 | 21 | 22 | 23 |
| LED | L0 | L1 | L2 | L3 | L4 | L5 | L6 | L7 |

> 💡 **Conexión con la teoría:** que el índice arranque en 0 no es un capricho. Los bits de un número también se numeran desde 0: el bit 0 es el de menos peso (2⁰ = 1). Por eso `LEDS[i]` es el LED del **bit i**, y L0 es el bit menos significativo.

Ahora sí, el `for` y el arreglo trabajan juntos. Configurar los 8 pines pasa de 8 renglones a 3:

```c
for (int i = 0; i < 8; i++) {
  pinMode(LEDS[i], OUTPUT);
}
```

Y el barrido de la introducción ([`ejemplos/01_barrido_for`](ejemplos/01_barrido_for/sketch.ino)) se lee así: *"para i desde 0 mientras i < 8: encender el LED i, esperar, apagarlo"*.

> ⚠️ Si se escribe `for (int i = 0; i <= 8; i++)`, en la última vuelta se pide `LEDS[8]`, que está **fuera** del arreglo. El compilador no avisa y el programa lee basura de la memoria. Regla: arreglo de 8 → `i < 8`.

Por ahora el arreglo lo usamos solo para esto: una lista de pines. En la Clase 6 se usarán a fondo.

### 3. Funciones: ponerle nombre a una idea (50 min)

Ya se vienen usando funciones sin saberlo: `pinMode`, `digitalWrite`, `delay` y `Serial.printf` son funciones que alguien escribió. Y `setup()` y `loop()` son funciones que escribe uno mismo. Hoy aprendemos a crear funciones propias.

Una **función** es un bloque de código **con nombre**, que se puede ejecutar (*llamar*) cuantas veces se quiera. Sirve para dos cosas:

1. **No repetir código.** Se escribe una vez y se llama desde todos lados.
2. **Ponerle nombre a una idea.** `mostrarNumero(7)` se entiende mucho mejor que cuatro `digitalWrite` con divisiones.

#### 3.1 Función sin parámetros

```c
void imprimirSeparador() {
  Serial.printf("------------------------------\n");
}
```

| Parte | Qué significa |
|---|---|
| `void` | La función **no devuelve** ningún resultado ("vacío") |
| `imprimirSeparador` | El nombre. Lo elige el programador: que diga **qué hace** |
| `()` | Los parámetros (aquí ninguno) |
| `{ ... }` | El **cuerpo**: lo que se ejecuta cada vez que se la llama |

Para usarla, se la **llama** por su nombre con paréntesis y punto y coma:

```c
imprimirSeparador();
```

#### 3.2 Función con parámetros

Los **parámetros** son datos que se le pasan a la función para que haga lo mismo pero con distintos valores. Se declaran como variables, con su tipo, separados por coma:

```c
void parpadear(int pin, int veces) {
  for (int i = 0; i < veces; i++) {
    digitalWrite(pin, HIGH);
    delay(150);
    digitalWrite(pin, LOW);
    delay(150);
  }
}
```

```c
parpadear(LEDS[0], 3);   // pin = 4, veces = 3
parpadear(LEDS[7], 1);   // pin = 23, veces = 1
```

Al llamar, los valores se copian **en orden** a los parámetros: el primero a `pin`, el segundo a `veces`.

#### 3.3 Función que devuelve un resultado: `return`

Si la función **calcula** algo, en lugar de `void` se pone el **tipo del resultado**, y con `return` se lo devuelve:

```c
int cuadrado(int x) {
  int resultado = x * x;
  return resultado;
}
```

La llamada "se reemplaza" por el valor devuelto, así que se puede usar en cualquier lugar donde iría un número:

```c
int c = cuadrado(5);                               // c vale 25
Serial.printf("7 al cuadrado = %d\n", cuadrado(7)); // imprime 49
```

> ⚠️ `return` **termina** la función en ese momento. Lo que esté escrito después del `return` no se ejecuta.
> ⚠️ Si la función dice que devuelve `int` o `bool` y se olvida el `return`, el compilador a lo sumo da un *warning*, y la función devuelve cualquier cosa.

Todo esto junto, funcionando: [`ejemplos/02_primeras_funciones`](ejemplos/02_primeras_funciones/sketch.ino).

#### 3.4 Variables locales

Las variables que se crean **adentro** de una función (incluidos sus parámetros y la `i` del `for`) son **locales**: nacen cuando la función arranca y desaparecen cuando termina. Ninguna otra función las ve.

```c
int cuadrado(int x) {
  int resultado = x * x;   // 'resultado' solo existe aquí adentro
  return resultado;
}

void setup() {
  Serial.printf("%d\n", resultado);   // ❌ ERROR: 'resultado' was not declared in this scope
}
```

Eso es bueno: se puede usar `i` en diez funciones distintas sin que se pisen. Y los parámetros son **copias**: si la función modifica su parámetro, la variable original de quien la llamó no cambia.

Las constantes como `LEDS` o `BOTON_A`, declaradas **afuera** de toda función (arriba de todo), son **globales**: las ve todo el programa.

#### 3.5 El orden importa: definir antes de usar

El compilador lee el archivo **de arriba hacia abajo**. Si en `setup()` se llama a `mostrarNumero(5)` y la función está escrita más abajo, en C común da error: *"'mostrarNumero' was not declared in this scope"*. Hay dos soluciones:

**a)** Escribir las funciones **arriba** de `setup()` y `loop()` (lo que hacemos en el curso).

**b)** Escribir arriba solo el **prototipo** (la primera línea de la función, con `;`) y la función completa abajo:

```c
void mostrarNumero(int n);    // prototipo: "existe una función así"

void setup() {
  mostrarNumero(5);           // ahora el compilador ya la conoce
}

void mostrarNumero(int n) {   // la definición completa, más abajo
  ...
}
```

> Arduino a veces lo perdona porque genera los prototipos solo. Conviene no acostumbrarse: en C "de verdad" no pasa.

#### 3.6 Funciones compuerta

Una compuerta lógica **es** una función: recibe entradas binarias y devuelve una salida binaria. En C se escribe literalmente así:

```c
bool compuertaAND(bool a, bool b) {
  return a && b;
}

bool compuertaXOR(bool a, bool b) {
  return a != b;
}
```

```c
bool s = compuertaAND(1, 0);   // s vale 0
```

> 💡 **Conexión con la teoría:** en el pizarrón se escribe **S = f(A, B)**. En C se escribe `bool f(bool a, bool b)`. Es la misma idea: una función lógica tiene variables de entrada y una salida, y para cada combinación de entradas da un único valor.

#### 3.7 `mostrarNumero`: binario en los LEDs, en una función

En la Clase 2 se mostró un número de 0 a 15 en los LEDs con cuatro renglones de divisiones. Ahora, con un `for` y divisiones sucesivas, queda así ([`ejemplos/04_contar_0_a_15`](ejemplos/04_contar_0_a_15/sketch.ino)):

```c
void mostrarNumero(int n) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], n % 2);   // el resto de dividir por 2 es el bit i
    n = n / 2;                      // "corremos" el número para el bit siguiente
  }
}
```

Seguirlo a mano con `n = 6`:

| `i` | `n` al entrar | `n % 2` → LED | `n / 2` |
|---|---|---|---|
| 0 | 6 | 0 → L0 apagado | 3 |
| 1 | 3 | 1 → L1 encendido | 1 |
| 2 | 1 | 1 → L2 encendido | 0 |
| 3 | 0 | 0 → L3 apagado | 0 |

Resultado en los LEDs: `L3 L2 L1 L0` = `0 1 1 0` = 6. ✔

Y contar de 0 a 15 es ahora un `for` que llama a la función:

```c
for (int n = 0; n <= 15; n++) {
  mostrarNumero(n);
  delay(400);
}
```

> 💡 **Conexión con la teoría:** es exactamente el método de **divisiones sucesivas** para pasar de decimal a binario. Los restos, leídos de abajo hacia arriba, son los bits; aquí cada resto va directo a su LED.

### 4. Tablas de verdad con bucles (35 min)

#### 4.1 Dos entradas: bucles anidados

Una tabla de verdad recorre **todas** las combinaciones de las entradas. Con dos entradas, por cada valor de A hay que probar todos los de B. Eso es un `for` **adentro** de otro (*anidado*) ([`ejemplos/03_tabla_and`](ejemplos/03_tabla_and/sketch.ino)):

```c
Serial.printf(" A | B | S\n");
Serial.printf("---+---+---\n");
for (int a = 0; a <= 1; a++) {
  for (int b = 0; b <= 1; b++) {
    Serial.printf(" %d | %d | %d\n", a, b, compuertaAND(a, b));
  }
}
```

```
 A | B | S
---+---+---
 0 | 0 | 0
 0 | 1 | 0
 1 | 0 | 0
 1 | 1 | 1
```

El `for` de afuera da 2 vueltas; en **cada una**, el de adentro da 2 vueltas completas. Total: 2 × 2 = 4 filas. Es igual que un cuentakilómetros: el dígito de la derecha (B) gira completo antes de que el de la izquierda (A) avance uno.

> 💡 **Conexión con la teoría:** la tabla sale **en el mismo orden** en que se escribe en el pizarrón (00, 01, 10, 11), porque A es el bucle de afuera (el de más peso) y B el de adentro (el de menos peso). Fila = número binario AB.

#### 4.2 Tres entradas: 8 filas

Con 3 variables se pueden anidar tres `for` (2 × 2 × 2 = 8 filas). Pero hay otra forma, más corta, que usa lo que ya se sabe: **las filas de la tabla son los números de 0 a 7 escritos en binario**. Basta un solo `for` y sacar cada bit por división:

```c
bool funcionF(bool a, bool b, bool c) {
  return (a && b) || !c;          // F = A·B + C'
}
```

```c
for (int fila = 0; fila < 8; fila++) {
  int a = (fila / 4) % 2;   // bit de peso 4
  int b = (fila / 2) % 2;   // bit de peso 2
  int c = fila % 2;         // bit de peso 1
  Serial.printf(" %d | %d | %d | %d\n", a, b, c, funcionF(a, b, c));
}
```

| `fila` | `fila / 4 % 2` (A) | `fila / 2 % 2` (B) | `fila % 2` (C) |
|---|---|---|---|
| 5 | 5 / 4 = 1 → 1 | 5 / 2 = 2 → 0 | 1 |
| 6 | 6 / 4 = 1 → 1 | 6 / 2 = 3 → 1 | 0 |

> 💡 **Conexión con la teoría:** el número de fila es el **número de mintérmino**. La fila 5 (A=1, B=0, C=1) es m5 = A·B'·C. Cuando la salida vale 1 en una fila, ese mintérmino forma parte de la **suma de productos** canónica.

### 5. Ejercicios (45 min)

**Ejercicio 1 — Mayoría de 3.**
Escribir la función `bool mayoria(bool a, bool b, bool c)` que vale 1 cuando **al menos dos** entradas valen 1, e imprimir su tabla de verdad completa (8 filas) con tres `for` anidados. Comparar con la tabla hecha a mano.

**Ejercicio 2 — Verificar De Morgan.**
Para **todas** las combinaciones de A y B, calcular `!(a && b)` y `!a || !b`, e imprimir una línea por combinación terminada en `OK` si dan igual o `ERROR` si no. Hacer lo mismo con la segunda ley (`!(a || b)` contra `!a && !b`). Al final, imprimir si la ley se cumple siempre.
*(Así se demuestra un teorema por "inducción perfecta": probando todos los casos.)*

**Ejercicio 3 — `potencia2`.**
Escribir `int potencia2(int n)` que devuelva 2ⁿ usando un `for` que multiplica por 2. Usarla para imprimir el peso de cada LED (`L0 pesa 1`, …, `L7 pesa 128`). Bonus: reescribir `mostrarNumero` usando `(n / potencia2(i)) % 2`.

**Ejercicio 4 — Mintérminos.**
Para la función del Ejercicio 1 (o la que indique el docente), recorrer las 8 filas con un solo `for`, **contar** cuántas veces la salida vale 1 e imprimir la suma de mintérminos así:
```
F = Σm(3,5,6,7)
```
*(Pista: ojo con las comas. Van entre números, no después del último.)*

---

## ⭐ TP Clase 4 — El entrenador imprime tablas de verdad

Partir del TP de la Clase 3 (el modo compuertas con A y B) y agregarle:

1. **Encabezado** actualizado y el pinout nuevo, con el arreglo:
   ```c
   const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
   const int BOTON_A = 32;
   const int BOTON_B = 33;
   ```
   Y configurar los 8 LEDs con un `for`.
2. Una **función por compuerta**: `compuertaAND`, `compuertaOR`, `compuertaXOR`, `compuertaNAND`, `compuertaNOR`, `compuertaXNOR`.
3. Una función `bool evaluar(int compuerta, bool a, bool b)` que, según un número, llame a la compuerta que corresponde (con `if` / `else if`):

   | `compuerta` | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
   |---|---|---|---|---|---|---|---|---|
   | Compuerta | AND | OR | XOR | NAND | NOR | XNOR | A (entrada) | B (entrada) |

4. Una función `void imprimirTabla(int compuerta)` que imprima la tabla de verdad de esa compuerta. En `setup()`, con un `for`, imprimir las tablas de **AND, OR, XOR y NAND**.
5. La tabla de verdad de una función de **3 variables** de la teoría, por ejemplo **F = A·B + C'**, usando un solo `for` de 0 a 7.
6. En el `loop()`, el **modo compuertas** del TP3 sigue funcionando, pero refactorizado: leer A y B, un `for` que hace `digitalWrite(LEDS[i], evaluar(i, a, b))`, y otro `for` que imprime cada medio segundo la misma línea del TP3 (`A=1 B=0 | AND=0 OR=1 ...`) usando una función `imprimirNombre(int compuerta)`.

Salida esperada al arrancar (fragmento):

```
==============================
  Entrenador Logico de Ana
  Sistemas Logicos - UNSE 2026
  Clase 4: tablas de verdad
==============================

Tabla de AND
 A | B | S
---+---+---
 0 | 0 | 0
 0 | 1 | 0
 1 | 0 | 0
 1 | 1 | 1
...
```

> Es el mismo orden de LEDs del TP3. Si en el propio se usó otro, respetarlo: lo importante es que el número de compuerta coincida con el número de LED.

Entrega: link del proyecto Wokwi `SL2026 - Apellido - Clase 4`.

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Explicar las tres partes de un `for` y seguir uno a mano en una tabla.
- [ ] Elegir entre `for` y `while` según el problema.
- [ ] Recorrer los 8 LEDs con `LEDS[i]` y explicar por qué el índice va de 0 a 7.
- [ ] Escribir una función con parámetros y otra con `return`, y llamarlas.
- [ ] Explicar qué es una variable local.
- [ ] Generar la tabla de verdad de una función de 2 o 3 variables con bucles.

---

## Material de la clase

- [`ejemplos/`](ejemplos/): barrido con `for`, primeras funciones, tabla de la AND y contador de 0 a 15 con `mostrarNumero`.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
