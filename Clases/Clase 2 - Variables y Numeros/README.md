# Clase 2 — Variables y Números

**Duración:** 3 hs
**Objetivo:** que el programa **recuerde** y **calcule** números. Al salir, el entrenador cuenta de 0 a 15 y muestra cada número en decimal, en hexa y en binario con 4 LEDs.

---

## Por qué seguimos así

En la Clase 1 el ESP32 repetía siempre lo mismo: prender, esperar, apagar. No podía **acordarse** de nada ni **hacer cuentas**.
Hoy aprendemos las dos cosas: guardar números en **variables** y operar con ellos. Y como en la teoría están viendo sistemas de numeración, vamos a usar esas cuentas para pasar de decimal a binario… y verlo en los LEDs.

---

## Contenido

### 1. Repaso y una pregunta (10 min)

Abrir el TP de la Clase 1. Pregunta: **¿cómo se podría hacer que el ESP32 cuente 0, 1, 2, 3… e imprima cada número?**
Con lo que sabemos hasta ahora habría que escribir un `Serial.printf("0\n");`, otro `Serial.printf("1\n");`… y así hasta el infinito. Necesitamos un lugar donde **guardar** el número y poder **cambiarlo**. Eso es una variable.

### 2. Variables: cajitas con nombre (30 min)

Una **variable** es una **cajita en la memoria** del micro. Tiene:

| Parte | Qué es | Ejemplo |
|---|---|---|
| **Tipo** | Qué clase de cosa entra y cuánto lugar ocupa | `int` |
| **Nombre** | Cómo se la llama en el programa | `edad` |
| **Valor** | Lo que tiene guardado ahora | `19` |

```
          edad
        ┌──────┐
  int   │  19  │      ← una cajita de tipo int, llamada edad, que guarda 19
        └──────┘
```

#### Declarar y asignar

```c
int edad;          // DECLARAR: crear la cajita (tipo + nombre)
edad = 19;         // ASIGNAR: guardar un valor en la cajita
int anio = 2026;   // las dos cosas en una sola línea
```

> ⚠️ El `=` de C **no es** el igual de matemática. Significa **"guardar lo de la derecha en la cajita de la izquierda"**. Por eso esto es válido:
> ```c
> edad = edad + 1;   // leer edad (19), sumarle 1, guardar el resultado (20) en edad
> ```
> En matemática `x = x + 1` no tiene solución. En C es lo más común del mundo.

Reglas para los nombres:
- Letras, números y `_`. **No** pueden empezar con número ni tener espacios. Evitar tildes y `ñ`: muchos compiladores no las aceptan. (`anio` sí, `año` no.)
- Mayúsculas y minúsculas son distintas: `Edad` y `edad` son dos variables diferentes.
- Usar nombres que digan qué guardan: `contador` es mejor que `x`.

#### Los tipos que vamos a usar

| Tipo | Qué guarda | Rango | Ejemplo |
|---|---|---|---|
| `int` | Entero con signo (32 bits en el ESP32) | −2.147.483.648 a 2.147.483.647 | `int edad = 19;` |
| `uint8_t` | Entero **sin signo** de **8 bits** | **0 a 255** | `uint8_t nota = 8;` |
| `float` | Número con coma (decimal) | Muy grande, con ~7 cifras de precisión | `float temp = 27.5;` |

> 💡 **Conexión con la teoría:** `uint8_t` se lee *"unsigned int de 8 bits"*. Con **n bits** se pueden representar **2ⁿ** valores distintos: con 8 bits, 2⁸ = 256 valores, del 0 al 255. Es exactamente lo que ven en la teoría con los números binarios sin signo. Un `uint8_t` es **un byte**, y en la placa tenemos **8 LEDs**: un LED por bit.

En C los decimales se escriben con **punto**: `27.5`, no `27,5`.

#### Imprimir variables: `printf` con formatos

`printf` puede imprimir el valor de una variable. Dentro del texto se pone un **especificador de formato** (empieza con `%`) y después de la coma, la variable:

```c
int edad = 19;
Serial.printf("Tengo %d anios\n", edad);        // Tengo 19 anios
```

```
   Serial.printf("Tengo %d anios\n", edad);
                        ▲              │
                        └──────────────┘   el %d se reemplaza por el valor de edad
```

Con varias variables, se reemplazan **en orden**:

```c
Serial.printf("%d + %d = %d\n", 2, 3, 2 + 3);   // 2 + 3 = 5
```

| Formato | Imprime | Ejemplo | Sale |
|---|---|---|---|
| `%d` | Entero en **decimal** (con signo) | `printf("%d", 42)` | `42` |
| `%u` | Entero en decimal **sin signo** (*unsigned*) | `printf("%u", nota)` | `8` |
| `%X` | Entero en **hexadecimal** (letras mayúsculas) | `printf("%X", 255)` | `FF` |
| `%f` | Número con coma | `printf("%f", 27.5)` | `27.500000` |
| `%.2f` | Con coma, **2 decimales** | `printf("%.2f", 27.5)` | `27.50` |
| `%%` | El carácter `%` | `printf("100%%")` | `100%` |

Dos trucos útiles para que los números queden alineados:

| Formato | Qué hace | Ejemplo | Sale |
|---|---|---|---|
| `%3u` | Ocupa al menos 3 lugares (rellena con espacios) | `printf("%3u", 7)` | `  7` |
| `%02X` | Al menos 2 dígitos, rellenando con **ceros** | `printf("%02X", 7)` | `07` |

Leer y correr [`ejemplos/01_variables`](ejemplos/01_variables/sketch.ino).

### 3. Operaciones con números (30 min)

| Operador | Qué hace | Ejemplo con `a = 17`, `b = 5` | Resultado |
|---|---|---|---|
| `+` | Suma | `a + b` | `22` |
| `-` | Resta | `a - b` | `12` |
| `*` | Multiplicación | `a * b` | `85` |
| `/` | División | `a / b` | **`3`** (¡ojo!) |
| `%` | **Resto** de la división (módulo) | `a % b` | `2` |
| `++` | Suma 1 a la variable | `a++;` | `a` pasa a valer `18` |

#### La división entera

Al dividir **dos enteros**, C da un resultado **entero**: tira los decimales (no redondea, **corta**).

```c
17 / 5    // da 3, no 3,4
7 / 2     // da 3, no 3,5
1 / 2     // da 0
```

¿Y lo que "sobra"? Lo da el operador **`%`** (se lee "módulo" o "resto"):

```
    17 │ 5
     2   3        17 / 5 = 3   (cociente)
                  17 % 5 = 2   (resto)        Comprobación: 3 × 5 + 2 = 17
```

Es la división que se aprende en la primaria, con cociente y resto. Para obtener decimales, usar `float`:

```c
float x = 17.0;
float y = 5.0;
Serial.printf("%.2f\n", x / y);   // 3.40
```

Dos usos de `%` que vamos a usar todo el tiempo:
- `n % 2` vale **0 si n es par** y **1 si n es impar**.
- `n % 16` siempre da un número **entre 0 y 15**: sirve para que un contador "dé la vuelta".

Correr [`ejemplos/02_operaciones`](ejemplos/02_operaciones/sketch.ino). **Antes de correrlo, anotar en un papel qué se espera que imprima cada línea.** Después comparar.

### 4. Ponerle nombre a los pines: `const` (10 min)

En la Clase 1 escribíamos `digitalWrite(17, HIGH)`. ¿Cuál LED era el 17? Hay que ir a mirar la tabla.
Mejor le ponemos nombre:

```c
const int L0 = 4;
const int L1 = 16;
const int L2 = 17;
const int L3 = 18;

void setup() {
  pinMode(L2, OUTPUT);
  digitalWrite(L2, HIGH);    // se lee solo: "encender L2"
}
```

`const` significa **constante**: es una variable que **no se puede cambiar**. Si por error se escribe `L0 = 5;`, el compilador lo frena con un error (`assignment of read-only variable 'L0'`). Es mejor que avise él a que el LED no prenda sin saber por qué.

Las constantes de pines van **arriba de todo**, afuera de `setup()` y `loop()`, para que se puedan usar en los dos.

### 5. Decimal a binario en los LEDs (35 min)

En la teoría aprendieron a pasar de decimal a binario con **divisiones sucesivas por 2**. Pasemos el **13**:

```
   13 / 2 = 6   resto 1   ← bit 0 (peso 1)    el primer resto es el de MENOS peso
    6 / 2 = 3   resto 0   ← bit 1 (peso 2)
    3 / 2 = 1   resto 1   ← bit 2 (peso 4)
    1 / 2 = 0   resto 1   ← bit 3 (peso 8)

   13 = 1101 en binario    (se lee de abajo hacia arriba)
   Comprobación: 1·8 + 1·4 + 0·2 + 1·1 = 13
```

Ahora observar: en C, "el resto de dividir por 2" es `% 2`, y "el cociente" es `/ 2`. Así que cada bit se puede calcular con una cuenta:

| Bit | Peso | En C | Para n = 13 |
|---|---|---|---|
| bit 0 | 1 | `n % 2` | 13 % 2 = **1** |
| bit 1 | 2 | `(n / 2) % 2` | 6 % 2 = **0** |
| bit 2 | 4 | `(n / 4) % 2` | 3 % 2 = **1** |
| bit 3 | 8 | `(n / 8) % 2` | 1 % 2 = **1** |

¿Por qué `n / 4` y no "dividir dos veces por 2"? Porque dividir por 2 dos veces (con división entera) es lo mismo que dividir por 4 una vez. `(13 / 2) / 2 = 6 / 2 = 3` y `13 / 4 = 3`. Dividir por el **peso** del bit "corre" el número hasta que ese bit queda en el lugar de las unidades, y `% 2` lo extrae.

Cada una de estas cuentas da **0 o 1**. Y `LOW` vale 0 y `HIGH` vale 1. Entonces podemos escribirlo **directo** en el LED:

```c
digitalWrite(L0, n % 2);
digitalWrite(L1, (n / 2) % 2);
digitalWrite(L2, (n / 4) % 2);
digitalWrite(L3, (n / 8) % 2);
```

Correr [`ejemplos/03_binario_4leds`](ejemplos/03_binario_4leds/sketch.ino) y cambiar el valor de `n`. Los LEDs están ordenados como se escribe un número: **L3 a la izquierda** (más peso), **L0 a la derecha** (menos peso).

> 💡 **Conexión con la teoría:** esto **es** el método de divisiones sucesivas, escrito en C. Y los pesos 1, 2, 4, 8 son las potencias de 2 de la notación posicional: un número en binario vale `b3·2³ + b2·2² + b1·2¹ + b0·2⁰`. Con 4 bits llegamos hasta 1111₂ = 15 = F₁₆: **4 bits son exactamente un dígito hexadecimal**. Por eso los 4 LEDs rojos (L0–L3) forman un *nibble*.

| Decimal | Binario (L3 L2 L1 L0) | Hexa |
|---|---|---|
| 0 | 0000 | 0 |
| 5 | 0101 | 5 |
| 9 | 1001 | 9 |
| 10 | 1010 | A |
| 12 | 1100 | C |
| 15 | 1111 | F |

### 6. Contar: variables que sobreviven, `% 16` y desborde (25 min)

#### ¿Dónde declaro el contador?

Queremos que `loop()` muestre un número, le sume 1 y vuelva a empezar. Primer intento:

```c
void loop() {
  int contador = 0;         // ❌ se crea de nuevo en CADA vuelta
  Serial.printf("%d\n", contador);
  contador++;
  delay(1000);
}
```

Imprime `0`, `0`, `0`… **para siempre**. ¿Por qué? Porque cada vez que `loop()` arranca, la línea `int contador = 0;` crea la cajita de nuevo y le pone 0.

La solución: declararla **afuera**, arriba de todo, igual que los pines. Así se crea **una sola vez** al encender y conserva su valor entre vuelta y vuelta:

```c
int contador = 0;           // ✅ afuera: se crea una sola vez

void loop() {
  Serial.printf("%d\n", contador);
  contador++;
  delay(1000);
}
```

*(A las variables declaradas afuera se las llama **globales**. En la Clase 7 vemos bien la diferencia.)*

#### Dar la vuelta con `% 16`

Con 4 LEDs solo podemos mostrar de 0 a 15. Después de 15 queremos volver a 0:

```c
contador = (contador + 1) % 16;
```

| `contador` | `contador + 1` | `% 16` |
|---|---|---|
| 14 | 15 | 15 |
| 15 | 16 | **0** |
| 0 | 1 | 1 |

#### El desborde: cuando el número no entra

¿Y si usamos un `uint8_t` y le seguimos sumando 1? Correr [`ejemplos/04_desborde`](ejemplos/04_desborde/sketch.ino):

```
Decimal: 254   Hexa: FE
Decimal: 255   Hexa: FF
Decimal:   0   Hexa: 00     ← ¡volvió a 0!
Decimal:   1   Hexa: 01
```

255 en binario es `11111111`. Si se le suma 1 da `1 00000000`: **9 bits**. Pero la cajita tiene solo 8, así que el 1 de la izquierda (el *acarreo*) **se pierde**. Queda `00000000` = 0.

Es como el cuentakilómetros de un auto viejo: después de 99999 viene 00000.

> 💡 **Conexión con la teoría:** la aritmética de **n bits** es aritmética **módulo 2ⁿ**. Un `uint8_t` hace todas sus cuentas módulo 2⁸ = 256: el `% 256` lo pone el hardware gratis. El bit que se pierde es el **acarreo de salida** (*carry out*) del sumador de 8 bits que ven en la teoría.

El desborde no da error ni aviso: el programa sigue como si nada. Es una fuente clásica de errores en programas reales, así que conviene saber que existe.

### 7. Ejercicios (40 min)

**Ejercicio 1 — Calculadora.**
Declarar dos variables `int a` y `int b` con cualquier valor. Imprimir la suma, la resta, el producto, el cociente **y el resto**, con este formato:
```
23 + 4 = 27
23 - 4 = 19
23 * 4 = 92
23 / 4 = 5  (resto 3)
```
Agregar una línea que muestre la división **con decimales** usando `float`. Probar con `b = 0`: ¿qué pasa? *(Spoiler: dividir por cero no se puede; anotar lo que se observe.)*

**Ejercicio 2 — Horas, minutos y segundos.**
Declarar `int totalSegundos = 3725;` e imprimir cuántas horas, minutos y segundos son, en el formato `01:02:05`. Usar solo `/` y `%`. *Pista:* una hora tiene 3600 segundos; `%02d` imprime al menos dos dígitos.
**Desafío:** hacer que `totalSegundos` aumente 1 por segundo, como un reloj.

**Ejercicio 3 — Verificación a mano.**
1. **Sin la computadora**, pasar a binario el número 11 por divisiones sucesivas. Anotar qué LEDs deberían quedar prendidos.
2. Escribir el programa que lo muestra en L0–L3 e imprime los bits.
3. Correr y comparar. Repetir con 6 y con 15.
4. Imprimir también la **verificación** `8*b3 + 4*b2 + 2*b1 + 1*b0` y comprobar que da el número original.

---

## ⭐ TP Clase 2 — Mi entrenador cuenta en binario

Partir del TP de la Clase 1 (dejar el cartel de presentación) y escribir un programa que:

1. Tenga el **comentario de encabezado** actualizado (Clase 2, qué hace ahora).
2. Use **constantes** con nombre para los pines: `const int L0 = 4;` etc.
3. Tenga una variable `contador` que arranque en 0 y **avance 1 cada segundo**, de 0 a 15, y después vuelva a 0.
4. Muestre el contador en **binario en L0–L3** (L0 = bit de menos peso).
5. Imprima cada valor en decimal y en hexa, por ejemplo:
   ```
   Decimal:  9  Hexa: 9  -> LEDs 1001
   Decimal: 10  Hexa: A  -> LEDs 1010
   Decimal: 11  Hexa: B  -> LEDs 1011
   ```

**Extensión (opcional, suma puntos):** hacer un contador **`uint8_t`** de 0 a 255 que se muestre en los **8 LEDs** (pesos hasta 128) y se imprima en decimal y en hexa con dos dígitos (`%02X`). **No** usar `% 256`: dejar que desborde solo y mostrar en el monitor serie el momento en que pasa de `FF` a `00`. *(Para no esperar 4 minutos, inicializarlo en 250.)*

Entrega: link del proyecto Wokwi `SL2026 - Apellido - Clase 2`.

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Explicar qué es una variable (tipo, nombre, valor) y la diferencia entre declarar y asignar.
- [ ] Elegir entre `int`, `uint8_t` y `float`, y decir el rango de un `uint8_t`.
- [ ] Imprimir variables con `%d`, `%u`, `%X` y `%f`.
- [ ] Predecir el resultado de `17 / 5` y de `17 % 5`.
- [ ] Usar `const int` para darle nombre a un pin.
- [ ] Pasar un número de 0 a 15 a binario con divisiones sucesivas, en papel y en C.
- [ ] Explicar por qué un `uint8_t` que vale 255 pasa a 0 al sumarle 1.

---

## Material de la clase

- [`ejemplos/`](ejemplos/): variables, operaciones, binario en 4 LEDs y desborde de un `uint8_t`.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP, con su extensión (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
