# Clase 3 — Decisiones y Compuertas

**Duración:** 3 hs
**Objetivo:** que el programa **lea** los pulsadores y **decida** qué hacer. Al salir, el entrenador funciona como seis compuertas lógicas a la vez, con A y B como entradas.

---

## Por qué seguimos así

Hasta ahora el ESP32 hablaba pero no escuchaba: hacía siempre lo mismo, pasara lo que pasara afuera.
Hoy aprendemos a **leer entradas** (los pulsadores) y a **tomar decisiones** con `if`. Y se verá que el álgebra de Boole de la teoría ya está adentro de C: AND, OR y NOT son operadores del lenguaje.

---

## Contenido

### 1. Entradas digitales y el pull-up (25 min)

#### Leer un pin: `digitalRead`

| Instrucción | Qué hace |
|---|---|
| `pinMode(32, INPUT_PULLUP);` | Configura el pin 32 como **entrada** con resistencia de *pull-up* interna. Va en `setup()`. |
| `digitalRead(32)` | **Lee** el pin: da `HIGH` (1) si hay tensión, `LOW` (0) si está a 0 V. |

`digitalRead` **devuelve un valor**: se puede guardar en una variable, imprimirlo o usarlo en una cuenta.

```c
int lectura = digitalRead(32);
Serial.printf("El pin lee %d\n", lectura);
```

#### ¿Por qué apretado da 0? La lógica invertida

En la placa, cada pulsador conecta el pin a **GND** cuando se aprieta. Adentro del ESP32, `INPUT_PULLUP` conecta una resistencia del pin a **3,3 V**:

```
          3,3 V
            │
           [R]  ← resistencia de pull-up (adentro del ESP32)
            │
  GPIO 32 ──┤
            │
          ┤ pulsador ├
            │
           GND
```

| Pulsador | El pin queda conectado a… | `digitalRead` da |
|---|---|---|
| Suelto | 3,3 V (a través de la resistencia) | `HIGH` = **1** |
| Apretado | GND (directo) | `LOW` = **0** |

O sea: **apretado = 0, suelto = 1**. Al revés de lo que uno esperaría. Si no tuviéramos la resistencia, con el botón suelto el pin quedaría "flotando" en el aire y leería cualquier cosa.

Para trabajar cómodos, **invertimos la lectura una sola vez**, apenas leemos, y de ahí en adelante pensamos en "1 = apretado":

```c
bool a = !digitalRead(BOTON_A);      // ! invierte: apretado (0) -> 1
// o, lo que es lo mismo:
bool a = digitalRead(BOTON_A) == LOW;  // "a es verdadero si el pin está en LOW"
```

> 💡 **Conexión con la teoría:** esto es **lógica negativa** (o *activa en bajo*): la señal física vale 0 cuando la condición es verdadera. En los esquemas se dibuja con un circulito o una barra arriba del nombre (`A̅`). El `!` de C es la compuerta NOT que "corrige" la señal.

Abrir y correr [`ejemplos/01_boton_led`](ejemplos/01_boton_led/src/main.cpp): imprime la lectura cruda del pin y la invertida. Apretar A y observar cómo cambian.

### 2. Verdadero y falso: `bool` y comparaciones (20 min)

#### El tipo `bool`

Una variable `bool` guarda **una sola cosa: verdadero o falso**. Es un bit.

| Valor | Significa | Vale como número |
|---|---|---|
| `true` | Verdadero | **1** |
| `false` | Falso | **0** |

```c
bool encendido = true;
Serial.printf("%d\n", encendido);    // imprime 1
digitalWrite(L0, encendido);         // true es 1 = HIGH: el LED enciende
```

En C, además, **cualquier número distinto de 0 cuenta como verdadero**, y el 0 cuenta como falso.

#### Operadores relacionales: comparar

Comparan dos valores y dan un `bool`:

| Operador | Pregunta | Ejemplo con `n = 7` | Resultado |
|---|---|---|---|
| `==` | ¿Es igual? | `n == 7` | `true` (1) |
| `!=` | ¿Es distinto? | `n != 7` | `false` (0) |
| `<` | ¿Es menor? | `n < 10` | `true` (1) |
| `>` | ¿Es mayor? | `n > 10` | `false` (0) |
| `<=` | ¿Es menor o igual? | `n <= 7` | `true` (1) |
| `>=` | ¿Es mayor o igual? | `n >= 8` | `false` (0) |

> ⚠️ **El error más famoso de C:** `=` y `==` son **cosas distintas**.
>
> | Se escribe | Significa |
> |---|---|
> | `n = 7` | **Asignación**: guardar 7 en `n` |
> | `n == 7` | **Comparación**: ¿`n` vale 7? |
>
> Si se escribe `if (n = 7)`, el compilador **no da error** (a lo sumo un *warning*): guarda 7 en `n`, y como 7 es distinto de 0, la condición es **siempre verdadera**. El programa funciona… mal, y no se sabe por qué.

### 3. Tomar decisiones: `if`, `else`, `else if` (30 min)

#### `if`: hacer algo solo si se cumple una condición

```c
if (condicion) {
  // esto se ejecuta solo si la condición es verdadera
}
```

#### `if` / `else`: una cosa u otra

```c
if (a) {
  digitalWrite(L0, HIGH);    // si A está apretado
} else {
  digitalWrite(L0, LOW);     // si no
}
```

```
               ┌───────────┐
               │ ¿a es     │
               │ verdadero?│
               └─────┬─────┘
             sí      │      no
        ┌────────────┴────────────┐
        ▼                         ▼
  digitalWrite(L0, HIGH)    digitalWrite(L0, LOW)
        │                         │
        └────────────┬────────────┘
                     ▼
              sigue el programa
```

Siempre se ejecuta **uno solo** de los dos bloques, nunca los dos.

#### `else if`: más de dos caminos

```c
if (n < 5) {
  Serial.printf("chico\n");
} else if (n < 15) {
  Serial.printf("mediano\n");      // llega aquí solo si n >= 5
} else {
  Serial.printf("grande\n");       // llega aquí solo si n >= 15
}
```

Las condiciones se prueban **en orden, de arriba hacia abajo**. Apenas una es verdadera, se ejecuta su bloque y se **saltean todas las demás**.

#### Reglas de escritura del `if`

| Regla | Bien | Mal |
|---|---|---|
| La condición va **entre paréntesis** | `if (n > 3)` | `if n > 3` |
| **No** lleva `;` después del paréntesis | `if (n > 3) {` | `if (n > 3); {` |
| Usar **siempre llaves**, aunque sea una sola línea | `if (a) { x++; }` | `if (a) x++; y++;` (el `y++` se ejecuta siempre) |
| Comparar es `==` | `if (n == 0)` | `if (n = 0)` |

> El `;` después del `if` es traicionero: `if (n > 3);` significa *"si n > 3, no hacer nada"*. Las llaves que vienen después se ejecutan **siempre**. El compilador no se queja.

Correr [`ejemplos/02_par_impar`](ejemplos/02_par_impar/src/main.cpp). Usa lo de la clase pasada: `n % 2 == 0` es la forma de preguntar "¿es par?".

### 4. Operadores lógicos = compuertas (35 min)

C tiene tres **operadores lógicos** que combinan valores verdadero/falso:

| Operador | Nombre | Da verdadero si… |
|---|---|---|
| `a && b` | Y (AND) | **las dos** son verdaderas |
| `a \|\| b` | O (OR) | **alguna** es verdadera (o las dos) |
| `!a` | NO (NOT) | `a` es **falsa** |

El `||` son dos "barras verticales" (en el teclado latinoamericano es la tecla a la izquierda del `1`; en el español de España, `AltGr` + `1`).

Con estos tres se arman **todas** las compuertas que ven en la teoría:

| Compuerta | Álgebra de Boole | En C | 00 | 01 | 10 | 11 |
|---|---|---|---|---|---|---|
| AND | A·B | `a && b` | 0 | 0 | 0 | 1 |
| OR | A + B | `a \|\| b` | 0 | 1 | 1 | 1 |
| NOT | A̅ | `!a` | 1 | 1 | 0 | 0 |
| NAND | (A·B)̅ | `!(a && b)` | 1 | 1 | 1 | 0 |
| NOR | (A+B)̅ | `!(a \|\| b)` | 1 | 0 | 0 | 0 |
| XOR | A ⊕ B | `a != b` | 0 | 1 | 1 | 0 |
| XNOR | (A ⊕ B)̅ | `a == b` | 1 | 0 | 0 | 1 |

*(Las columnas 00, 01, 10, 11 son los valores de A y B.)*

Observar XOR y XNOR: no hay un operador lógico especial, pero **XOR es "son distintas"** y **XNOR es "son iguales"**. Con `bool` (que solo valen 0 o 1) eso es exactamente `!=` y `==`.

Como el resultado de una compuerta es un `bool` (0 o 1), se puede pasar **directo** a `digitalWrite`:

```c
digitalWrite(L0, a && b);     // L0 es la salida de una AND
```

No hace falta un `if`. Pero, si se prefiere, es lo mismo que:

```c
if (a && b) {
  digitalWrite(L0, HIGH);
} else {
  digitalWrite(L0, LOW);
}
```

Correr [`ejemplos/03_compuertas`](ejemplos/03_compuertas/src/main.cpp). **Antes de apretar nada**, predecir: con A y B sueltos, ¿qué LEDs van a estar encendidos? Después recorrer la tabla de verdad apretando A, B y los dos.

> 💡 **Conexión con la teoría:** el **·** del álgebra de Boole se escribe `&&`, el **+** se escribe `||` y la **barra** se escribe `!`. Una expresión como `F = A·B + C̅` se traduce **símbolo por símbolo**: `f = (a && b) || !c;`. Igual que en el álgebra, el AND tiene prioridad sobre el OR, pero conviene **usar paréntesis** igual: se lee mejor y no hay sorpresas.

> ⚠️ No confundir `&&` con `&`, ni `||` con `|`. Los simples son otros operadores (los vemos en la Clase 5). Con uno solo, el programa a veces compila y funciona "casi bien"… hasta que no.

### 5. De Morgan, comprobado por el ESP32 (15 min)

Las **leyes de De Morgan** dicen:

| Ley | Álgebra | En C |
|---|---|---|
| 1 | (A·B)̅ = A̅ + B̅ | `!(a && b) == (!a \|\| !b)` |
| 2 | (A+B)̅ = A̅ · B̅ | `!(a \|\| b) == (!a && !b)` |

En la teoría las demuestran con tablas de verdad. Aquí las podemos **verificar**: calculamos los dos lados con los pulsadores y comprobamos si dan lo mismo en las 4 combinaciones de A y B.

```c
bool izquierda = !(a && b);
bool derecha   = !a || !b;
Serial.printf("izq=%d der=%d\n", izquierda, derecha);
```

Es el ejercicio 3. Y una pregunta para pensar: ¿qué pasa si, por error, se escribe `!a && !b` del lado derecho de la ley 1? ¿En qué combinación se descubre?

### 6. Ejercicios (55 min)

**Ejercicio 1 — Funciones de 3 entradas.**
Usar los tres pulsadores A, B y C.
- En **L0**, la función **mayoría**: vale 1 si **dos o más** de las tres entradas están en 1. Primero escribir su tabla de verdad (8 filas) y obtener la expresión como suma de productos.
- En **L1**, la función de la teoría **F = A·B + C̅**.
- Imprimir `A=1 B=0 C=1 | Mayoria=1  F=0` cada medio segundo y comprobar las 8 filas de la tabla apretando los botones.

**Ejercicio 2 — Alarma.**
Una casa tiene una alarma con:
- **B** (mantenido apretado) = sistema **armado**.
- **A** = sensor de la **puerta** (apretado = puerta abierta).
- **C** = botón de **pánico**.

Hacer que:
- Si se aprieta el pánico, **o** si está armada **y** se abre la puerta: los 4 LEDs rojos (L0–L3) **parpadean** e imprime `*** ALARMA ***` (y, si es posible, que indique cuál fue la causa).
- Si no, pero está armada: L4 (verde) encendido e imprime `Armada. Todo tranquilo.`
- Si no: imprime `Desarmada.`

Usar `if` / `else if` / `else`.

**Ejercicio 3 — Comprobar De Morgan.**
Calcular los dos lados de las dos leyes de De Morgan con A y B. Mostrarlos en L0–L3 (cada ley en un par de LEDs) e imprimirlos. Con un `if`, imprimir `OK` si los dos lados coinciden y `NO COINCIDEN` si no. Recorrer las 4 combinaciones.
*(Apretar los botones para cada combinación es un poco tedioso. En la Clase 4 el programa va a recorrer la tabla de verdad solo.)*

---

## ⭐ TP Clase 3 — Mi entrenador es un laboratorio de compuertas

Partir del TP de la Clase 2 (conservar el cartel y las constantes de los pines) y escribir un programa que:

1. Tenga el **comentario de encabezado** actualizado (Clase 3, qué hace ahora).
2. Lea los pulsadores **A** y **B** con `INPUT_PULLUP` y los convierta a `bool` donde **apretado = 1**.
3. Muestre en los LEDs:

   | LED | L0 | L1 | L2 | L3 | L4 | L5 | L6 | L7 |
   |---|---|---|---|---|---|---|---|---|
   | Salida | AND | OR | XOR | NAND | NOR | XNOR | A | B |

4. Imprima el estado **cada medio segundo**, en una línea:
   ```
   A=1 B=0 | AND=0 OR=1 XOR=1 NAND=1 NOR=0 XNOR=0
   ```
5. Verificar con los botones que las 4 filas de cada tabla de verdad coinciden con las de la teoría.

*(El contador de la Clase 2 no entra en este TP: no hay problema: en la Clase 8 el entrenador va a tener un menú para elegir entre todos los modos.)*

Entrega: carpeta del proyecto PlatformIO `SL2026-Apellido-Clase3` comprimida en ZIP, sin la carpeta `.pio` (ver [EVALUACION.md](../../EVALUACION.md#cómo-se-entrega-un-tp)).

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Leer un pulsador con `digitalRead` y explicar por qué con `INPUT_PULLUP` apretado da 0.
- [ ] Decir qué valores puede tener un `bool` y cuánto valen como número.
- [ ] Explicar la diferencia entre `=` y `==`.
- [ ] Escribir un `if` / `else if` / `else` sin `;` de más y con sus llaves.
- [ ] Escribir en C cualquier compuerta de dos entradas (AND, OR, NOT, NAND, NOR, XOR, XNOR).
- [ ] Traducir una expresión booleana de la teoría (como `A·B + C̅`) a C.
- [ ] Comprobar una ley de De Morgan con el ESP32.

---

## Material de la clase

- [Slides de la clase](https://claude.ai/artifact/CfmQmEm2s8zXfG7ToV9Mrk): la presentación para proyectar en el aula (también en [PDF](Slides_Clase_3.pdf)).
- [`ejemplos/`](ejemplos/): botón que enciende un LED, decisiones con números (par/impar) y cuatro compuertas con A y B.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
