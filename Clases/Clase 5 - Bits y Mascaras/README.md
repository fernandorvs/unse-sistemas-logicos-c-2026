# Clase 5 — Bits y Máscaras

**Duración:** 3 hs
**Objetivo:** tratar los 8 LEDs como **un solo byte**. Al salir, se sabe encender, apagar, invertir, leer y rotar bits individuales con los operadores bit a bit de C, y el entrenador funciona como un **registro de 8 bits**.

---

## Por qué esta clase

Hasta ahora, para mostrar un número en los LEDs se lo dividía por 2 una y otra vez. Funciona, pero es dar una vuelta larga: **el número ya está guardado en binario adentro del micro**. Una variable `uint8_t` son, literalmente, 8 celdas de memoria de un bit cada una. Y el entrenador tiene 8 LEDs.

C tiene operadores para tocar esos bits **directamente**, de a uno o todos a la vez. Son las mismas compuertas de la Clase 3 (AND, OR, XOR, NOT), pero aplicadas a los 8 bits en paralelo. Es la herramienta más usada al programar microcontroladores en serio.

---

## Contenido

### 1. Un byte = 8 LEDs (25 min)

Un `uint8_t` (Clase 2) guarda un número de 0 a 255 usando **8 bits**. Cada bit tiene un número de posición y un peso:

| Bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|---|
| Peso | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| LED | L7 | L6 | L5 | L4 | L3 | L2 | L1 | L0 |
| Color | verde | verde | verde | verde | rojo | rojo | rojo | rojo |

Los 4 verdes son el **nibble alto** y los 4 rojos el **nibble bajo**. Cada nibble es un dígito hexadecimal.

#### Escribir números en binario y en hexa

C permite escribir un número directamente en binario o en hexadecimal. Estas tres líneas guardan **exactamente lo mismo**:

```c
uint8_t a = 177;          // decimal
uint8_t b = 0b10110001;   // binario: empieza con 0b
uint8_t c = 0xB1;         // hexadecimal: empieza con 0x
```

```
   0b 1011 0001
      ──── ────
       B    1     →  0xB1  =  128 + 32 + 16 + 1  =  177
```

> 💡 **Conexión con la teoría:** el pasaje binario ↔ hexa que se hace en el pizarrón agrupando de a 4 bits es exactamente por qué los programadores usan hexa: `0xB1` es más corto que `0b10110001` y se traduce a ojo, nibble por nibble.

Conviene usar `0b...` cuando importan los **bits sueltos** (qué LED enciende) y `0x...` cuando se quiere escribir un byte entero de forma compacta.

#### Dos funciones que se usan todo el curso

`printf` sabe imprimir en decimal (`%u`) y en hexa (`%X`), pero **no tiene un formato para binario**. Y para los LEDs necesitamos sacar cada bit. Así que nos hacemos dos funciones ([`ejemplos/01_literales_binarios`](ejemplos/01_literales_binarios/sketch.ino)):

```c
// Muestra los 8 bits de 'valor' en los 8 LEDs (bit i -> LED Li)
void mostrarByte(uint8_t valor) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}

// Imprime los 8 bits, del 7 al 0 (como se escribe un binario)
void imprimirBinario(uint8_t valor) {
  for (int i = 7; i >= 0; i--) {
    Serial.printf("%d", (valor >> i) & 1);
  }
}
```

`(valor >> i) & 1` quiere decir "el bit `i` de `valor`". En las próximas secciones se verá por qué. Notar que `imprimirBinario` recorre **al revés** (de 7 a 0), porque en papel el bit de más peso se escribe primero, a la izquierda.

### 2. Operadores bit a bit (35 min)

Son las compuertas de siempre, pero aplicadas a **cada par de bits** por separado, los 8 a la vez:

| Operador | Nombre | Compuerta | Regla por bit |
|---|---|---|---|
| `x & y` | AND bit a bit | AND | 1 solo si **los dos** bits son 1 |
| `x \| y` | OR bit a bit | OR | 1 si **alguno** es 1 |
| `x ^ y` | XOR bit a bit | XOR | 1 si son **distintos** |
| `~x` | NOT bit a bit (complemento) | NOT | invierte **cada** bit |

Ejemplo ([`ejemplos/02_operaciones_bit_a_bit`](ejemplos/02_operaciones_bit_a_bit/sketch.ino)), con `x = 0b11001100` y `y = 0b10101010`:

```
  x      1 1 0 0 1 1 0 0
  y      1 0 1 0 1 0 1 0
         ───────────────
  x & y  1 0 0 0 1 0 0 0      ← AND columna por columna
  x | y  1 1 1 0 1 1 1 0      ← OR
  x ^ y  0 1 1 0 0 1 1 0      ← XOR
  ~x     0 0 1 1 0 0 1 1      ← NOT de x
```

> 💡 **Conexión con la teoría:** `x & y` entre dos bytes es **un integrado con 8 compuertas AND** (como el 74LS08, que trae 4): el bit 0 de x con el bit 0 de y, el bit 1 con el bit 1, y así. Ninguna columna se entera de lo que pasa en las otras. No hay acarreo como en la suma.

#### ⚠️ El error clásico: `&` no es `&&`

| Lógicos (Clase 3) | Bit a bit (hoy) |
|---|---|
| `&&` `\|\|` `!` | `&` `\|` `^` `~` |
| Miran **el valor entero** como verdadero (≠ 0) o falso (0) | Operan **cada bit** por separado |
| El resultado es siempre 0 o 1 | El resultado es otro byte |
| Para **condiciones** de un `if` | Para **manipular bits** |

```c
5 && 2   // = 1   (5 es verdadero, 2 es verdadero → verdadero)
5 & 2    // = 0   (101 & 010 = 000)
!5       // = 0   (NO verdadero = falso)
~5       // = -6  (invierte los 32 bits de un int... no es lo que se buscaba)
```

Si se escribe `if (a & b)` donde se quería `if (a && b)`, **compila igual** y a veces funciona, a veces no. Con `bool` (0 o 1) da lo mismo; con números, no. Regla: **condiciones con `&&`, bits con `&`**.

> ⚠️ `~` invierte **todos** los bits del tipo con el que C hace la cuenta (un `int`, de 32 bits). Hay que guardar el resultado en un `uint8_t` para quedarse con los 8 que importan: `uint8_t inv = ~x;`. Si se hace `Serial.printf("%X", ~x)` directo, aparece `FFFFFF33` en lugar de `33`.

### 3. Desplazamientos: `<<` y `>>` (25 min)

`x << n` **corre** todos los bits de `x` `n` lugares a la **izquierda**; por la derecha entran ceros. `x >> n` los corre a la **derecha**; por la izquierda entran ceros (en números sin signo).

```
  x        = 0 0 0 1 0 1 1 0   (22)
  x << 1   = 0 0 1 0 1 1 0 0   (44)   ← todo un lugar a la izquierda
  x >> 1   = 0 0 0 0 1 0 1 1   (11)   ← todo un lugar a la derecha
```

| Operación | Equivale a | Ejemplo |
|---|---|---|
| `x << 1` | `x * 2` | `22 << 1` = 44 |
| `x << n` | `x * 2ⁿ` | `3 << 4` = 48 |
| `x >> 1` | `x / 2` (división entera) | `22 >> 1` = 11 |
| `x >> n` | `x / 2ⁿ` | `200 >> 4` = 12 |
| `1 << n` | 2ⁿ: un único bit en 1, en la posición `n` | `1 << 5` = `0b00100000` |

> 💡 **Conexión con la teoría:** igual que en decimal multiplicar por 10 es "agregar un cero a la derecha", en binario multiplicar por 2 es correr un lugar a la izquierda. Y `x >> 1` es el **registro de desplazamiento** (*shift register*) que se verá con flip-flops: cada bit pasa al vecino en cada pulso de reloj.

`1 << n` es la `potencia2(n)` de la Clase 4 en un solo operador.

Ahora sí se entiende `(valor >> i) & 1`: correr el número `i` lugares a la derecha deja el bit `i` en la posición 0, y el `& 1` borra todos los demás.

```
  valor          = 1 0 1 1 0 0 0 1
  valor >> 4     = 0 0 0 0 1 0 1 1    ← el bit 4 quedó en la posición 0
  (valor>>4) & 1 = 0 0 0 0 0 0 0 1    ← solo sobrevive ese bit: vale 1
```

#### Barrido con shifts

([`ejemplos/03_barrido_shift`](ejemplos/03_barrido_shift/sketch.ino)): un `1` que camina por los LEDs.

```c
uint8_t patron = 0b00000001;
for (int i = 0; i < 8; i++) {
  mostrarByte(patron);
  delay(200);
  patron = patron << 1;
}
```

#### Rotación circular

Al correr `0b10000000 << 1`, el 1 "se cae" por la izquierda y queda `0`. En una **rotación**, el bit que sale por un extremo entra por el otro:

```c
x = (x << 1) | (x >> 7);    // rotar a la izquierda (8 bits)
```

```
  x            = 1 0 1 1 0 0 0 1
  x << 1       = 0 1 1 0 0 0 1 0   (el bit 7 se perdió al guardarlo en 8 bits)
  x >> 7       = 0 0 0 0 0 0 0 1   (... pero lo rescatamos aquí)
  OR de ambos  = 0 1 1 0 0 0 1 1   ✔ rotado
```

> ⚠️ **Siempre poner paréntesis con los shifts.** En C, `+` y `-` se calculan **antes** que `<<` y `>>`, y `==` se calcula **antes** que `&`. Sin paréntesis pasan cosas así:
>
> | Se escribió | C entiende | |
> |---|---|---|
> | `1 << n + 1` | `1 << (n + 1)` | ¿era eso? |
> | `x << 1 \| x >> 7` | `(x << 1) \| (x >> 7)` | aquí funciona por casualidad, pero es mejor no arriesgarse |
> | `if (x & 1 == 0)` | `if (x & (1 == 0))` → `x & 0` → **siempre falso** | ❌ |
>
> Regla del curso: **toda operación de bits va entre paréntesis.** `if ((x & 1) == 0)`.

### 4. Máscaras: tocar un solo bit (35 min)

Una **máscara** es un byte que se elige para "tapar" o "destapar" bits. Con `1 << n` se arma una máscara con un único 1 en la posición `n`, y con eso se hacen las cuatro operaciones básicas:

| Quiero… | Código | Por qué funciona |
|---|---|---|
| **Leer** el bit n | `(x >> n) & 1` | Lo traigo a la posición 0 y borro el resto |
| **Poner en 1** el bit n | `x \|= (1 << n);` | `algo OR 1 = 1`, `algo OR 0 = algo` |
| **Poner en 0** el bit n | `x &= ~(1 << n);` | `~(1 << n)` tiene todo en 1 menos el bit n; `algo AND 0 = 0` |
| **Invertir** el bit n | `x ^= (1 << n);` | `algo XOR 1 = NOT algo`, `algo XOR 0 = algo` |

> 💡 **Conexión con la teoría:** son las propiedades del álgebra de Boole que se ven en clase: **A + 1 = 1**, **A + 0 = A**, **A · 0 = 0**, **A · 1 = A**, **A ⊕ 1 = A'**, **A ⊕ 0 = A**. Las máscaras son esos teoremas aplicados a 8 bits a la vez.

Ejemplo: apagar L4 sin tocar los demás.

```
  x            = 1 0 1 1 0 0 0 1
  1 << 4       = 0 0 0 1 0 0 0 0
  ~(1 << 4)    = 1 1 1 0 1 1 1 1     ← la máscara
  x & máscara  = 1 0 1 0 0 0 0 1     ✔ solo cambió el bit 4
```

#### Operadores compuestos

`x |= m` es una forma corta de escribir `x = x | m`. Igual con los demás:

| Corto | Largo |
|---|---|
| `x \|= m;` | `x = x \| m;` |
| `x &= m;` | `x = x & m;` |
| `x ^= m;` | `x = x ^ m;` |
| `x <<= 1;` | `x = x << 1;` |
| `x >>= 1;` | `x = x >> 1;` |

#### Máscaras de varios bits

La máscara puede tener varios unos. Las más comunes son las de los nibbles:

```c
uint8_t bajo = x & 0x0F;          // 0000 1111 → me quedo con L3..L0
uint8_t alto = (x >> 4) & 0x0F;   // traigo L7..L4 abajo y me quedo con ellos
x |= 0xF0;                        // enciendo los 4 verdes sin tocar los rojos
x ^= 0xFF;                        // invierto los 8 (igual que x = ~x)
```

#### Contar de 0 a 255

Con `mostrarByte`, un contador de 8 bits es casi nada ([`ejemplos/04_contador_8_bits`](ejemplos/04_contador_8_bits/sketch.ino)):

```c
uint8_t contador = 0;   // global

void loop() {
  mostrarByte(contador);
  contador++;           // de 255 vuelve solo a 0
  delay(100);
}
```

> ⚠️ `for (uint8_t n = 0; n <= 255; n++)` **nunca termina**: un `uint8_t` siempre es menor o igual a 255 (después de 255 viene 0). Para un `for` de 0 a 255, usar `int`.

### 5. Pulsadores que hacen una acción (10 min)

Para el TP se necesita que **cada vez que se apriete** un botón pase algo una vez (rotar, invertir, sumar). Detectar bien "el momento en que se aprieta" lo vemos en la Clase 7. Por ahora usamos un **truco provisorio**:

```c
bool a = !digitalRead(BOTON_A);   // apretado = LOW, por eso el !

if (a) {
  registro++;      // la acción
  delay(250);      // esperamos para no repetirla mil veces
}
```

Sin el `delay(250)`, el `loop()` pasa miles de veces mientras el dedo está apretado y la acción se repite miles de veces. Con el `delay`, se repite **cada 250 ms** mientras se lo mantenga (como una tecla del teclado). No es perfecto, pero alcanza por ahora.

> ⚠️ Este patrón es **provisorio**. Tiene dos problemas: si se mantiene apretado, la acción se repite; y mientras dura el `delay`, el micro no hace nada más. En la Clase 7 lo reemplazamos por **detección de flanco**, que es lo correcto.

### 6. Ejercicios (50 min)

**Ejercicio 1 — Contar unos y bit de paridad.**
Escribir `int contarUnos(uint8_t x)` que devuelva cuántos bits valen 1 (recorrer los 8 bits con `(x >> i) & 1`). Después, para datos de 7 bits (0 a 127), calcular el **bit de paridad par** (`unos % 2`) y ponerlo en el bit 7 con `dato | (paridad << 7)`. Mostrar el byte resultante en los LEDs y verificar que siempre tiene una cantidad **par** de unos.

> 💡 **Conexión con la teoría:** el bit de paridad es el detector de errores más simple que hay. En hardware se calcula con un árbol de compuertas XOR: el XOR de todos los bits vale 1 si hay una cantidad impar de unos.

**Ejercicio 2 — Auto fantástico.**
Un único LED encendido que va de L0 a L7 con `<<=` y vuelve de L7 a L0 con `>>=`, sin parar. Cuidar que en los extremos no se quede "dos turnos" en el mismo LED.

**Ejercicio 3 — Complemento a 2 (breve).**
Para x de 1 a 6, imprimir en binario `x`, `~x` y `~x + 1`. Guardar `~x + 1` en un `int8_t` (entero **con signo** de 8 bits, va de −128 a 127) e imprimirlo con `%d`. ¿Qué número aparece? Verificar que `x + (~x + 1)` da 0 en 8 bits.

**Ejercicio 4 — Nibbles.**
Dado `x = 0xB1`, separarlo en nibble bajo (`x & 0x0F`) y nibble alto (`(x >> 4) & 0x0F`). Imprimir los dos en binario y en hexa, y mostrarlos uno por vez en los LEDs. ¿Qué diferencia hay entre `(x >> 4) & 0x0F` y `x & 0xF0`?

---

## ⭐ TP Clase 5 — Registro de 8 bits

El entrenador ahora tiene un **registro**: una variable global `uint8_t registro` que se ve todo el tiempo en los 8 LEDs.

1. **Encabezado** actualizado, pinout con `LEDS[8]` y los **tres** botones (`BOTON_A`, `BOTON_B`, `BOTON_C`).
2. Las funciones `mostrarByte(uint8_t valor)` e `imprimirBinario(uint8_t valor)`.
3. Una función `uint8_t rotarIzquierda(uint8_t x)` que haga la rotación circular.
4. El registro arranca en un valor a elección (por ejemplo `0b10110001`), y los botones hacen:

   | Botón | Acción | En C |
   |---|---|---|
   | **A** | Rotar a la izquierda (circular) | `registro = rotarIzquierda(registro);` |
   | **B** | Invertir todos los bits | `registro = ~registro;` |
   | **C** | Sumar 1 | `registro++;` |

5. Cada vez que cambia, se muestra en los LEDs y se imprime en los tres sistemas:
   ```
   Bin: 10110001  Hex: B1  Dec: 177
   ```
6. Usar el patrón provisorio "si está apretado → acción + `delay(250)`".

Salida esperada:

```
==============================
  Entrenador Logico de Ana
  Sistemas Logicos - UNSE 2026
  Clase 5: registro de 8 bits
==============================
A = rotar   B = invertir   C = sumar 1

Bin: 10110001  Hex: B1  Dec: 177
[A] rotar    -> Bin: 01100011  Hex: 63  Dec: 99
[B] invertir -> Bin: 10011100  Hex: 9C  Dec: 156
[C] sumar 1  -> Bin: 10011101  Hex: 9D  Dec: 157
```

**Para pensar:** apretar A ocho veces seguidas. ¿Qué valor queda? ¿Y si se aprieta B dos veces?

Entrega: link del proyecto Wokwi `SL2026 - Apellido - Clase 5`.

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Escribir un mismo byte en decimal, binario (`0b`) y hexa (`0x`), y pasar de uno a otro.
- [ ] Explicar la diferencia entre `&` y `&&` con un ejemplo donde den distinto.
- [ ] Calcular a mano el resultado de `&`, `|`, `^`, `~`, `<<` y `>>` entre dos bytes.
- [ ] Leer, poner en 1, poner en 0 e invertir un bit con máscaras.
- [ ] Explicar por qué `x << 1` multiplica por 2.
- [ ] Usar `mostrarByte` e `imprimirBinario` y explicar cada línea.

---

## Material de la clase

- [`ejemplos/`](ejemplos/): literales binarios, operaciones bit a bit, barrido con shifts y contador de 8 bits.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
