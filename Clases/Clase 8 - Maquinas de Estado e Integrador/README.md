# Clase 8 — Máquinas de Estado e Integrador

**Duración:** 3 hs (+ las defensas, que pueden ser otro día)
**Objetivo:** programar máquinas de estado con `enum` y `switch`, y armar el **Entrenador Lógico** completo: un menú que junta todo lo hecho en el curso. Al salir, el proyecto final queda encaminado y se sabe cómo es la defensa.

---

## Por qué esta clase

En la teoría ya se dibujaron **diagramas de estados**: circulitos (estados) unidos por flechas (transiciones). Hoy se verá que pasar ese dibujo a C es casi mecánico: **un nombre por circulito, un `case` por circulito, un `if` por flecha**.

Y resulta que el entrenador también es una máquina de estados: cada modo es un estado, y el botón C es la entrada que hace pasar de uno al siguiente. Con eso cerramos el curso.

| Bloque | Tiempo |
|---|---|
| 1. `enum` y `switch` | 15 min |
| 2. Máquinas de estado: del diagrama al código | 45 min |
| 3. Integrador: Mi Entrenador Lógico (trabajo asistido) | 90 min |
| 4. Ensayo de defensa en parejas | 30 min |

---

## Contenido

### 1. `enum`: ponerle nombre a los números (5 min)

Observar este fragmento de código:

```c
if (estado == 2) {
  digitalWrite(LEDS[0], HIGH);
}
```

¿Qué es el estado 2? ¿Rojo? ¿Abierta? ¿Alarma? Nadie lo sabe, ni siquiera quien lo escribió, dentro de dos semanas. A esos números sueltos que "significan algo" se los llama **números mágicos**, y son una fuente clásica de errores.

Un **`enum`** (enumeración) le pone **nombre** a una lista de valores enteros:

```c
enum Estado { VERDE, AMARILLO, ROJO };   // VERDE vale 0, AMARILLO 1, ROJO 2

Estado estado = VERDE;                   // una variable de tipo Estado

if (estado == ROJO) {                    // ahora se lee solo
  digitalWrite(LEDS[0], HIGH);
}
```

| Concepto | Detalle |
|---|---|
| Valores | Empiezan en **0** y suben de a 1, en el orden en que se escriben |
| Tipo nuevo | `enum Estado {...};` crea el tipo `Estado`. Las variables de ese tipo solo deberían guardar esos valores |
| Nombres | Por costumbre, los valores van en MAYÚSCULAS (como las constantes) |
| Truco del último | Si se agrega un valor extra al final, ese valor es **la cantidad**: `enum Modo { MODO_A, MODO_B, MODO_C, CANTIDAD_MODOS };` → `CANTIDAD_MODOS` vale 3 |

Para pasar al siguiente valor (por ejemplo, el siguiente modo del menú) se hace la cuenta con enteros y se la convierte de vuelta a `Modo` poniendo el tipo entre paréntesis:

```c
modo = (Modo)((modo + 1) % CANTIDAD_MODOS);   // 0 → 1 → 2 → 0 → ...
```

El `% CANTIDAD_MODOS` es el resto de la división (Clase 2): hace que después del último vuelva al primero. El `(Modo)` le dice al compilador "este entero es un `Modo` válido, se puede confiar".

**La promesa de la Clase 7.** ¿Recuerdan `seApreto(0)`, `seApreto(1)` y `seApreto(2)`? Había que recordar que 0 era A, 1 era B y 2 era C. Con un `enum` les ponemos nombre a esos índices, sin tocar la función:

```c
enum Boton { BOT_A, BOT_B, BOT_C };   // 0, 1, 2: los índices del arreglo BOTONES

if (seApreto(BOT_A)) {                // es lo mismo que seApreto(0), pero se lee solo
  contador++;
}
```

(Se llaman `BOT_A` y no `BOTON_A` porque `BOTON_A` ya es la constante con el **pin** 32.)

> 💡 **Conexión con la teoría:** en la teoría, al implementar una máquina de estados con flip-flops, se hace la **asignación de estados**: se le da un código binario a cada estado (VERDE = 00, AMARILLO = 01, ROJO = 10). El `enum` es exactamente eso: el compilador hace la asignación de estados y el programador usa los nombres.

### 2. `switch`: elegir entre muchos caminos (10 min)

Cuando hay que hacer cosas distintas según el valor de **una** variable, una cadena de `if / else if` se vuelve larga. El `switch` lo ordena:

```c
switch (estado) {
  case VERDE:
    Serial.printf("Pasen\n");
    break;                    // ¡termina el switch aquí!
  case AMARILLO:
    Serial.printf("Cuidado\n");
    break;
  case ROJO:
    Serial.printf("Alto\n");
    break;
  default:                    // si no coincidió ningún case
    Serial.printf("Estado raro\n");
    break;
}
```

| Parte | Qué hace |
|---|---|
| `switch (variable)` | Mira el valor de la variable (tiene que ser entera o un `enum`) |
| `case VALOR:` | "Si vale esto, empezar a ejecutar desde aquí" |
| `break;` | Salta afuera del `switch` |
| `default:` | Lo que se ejecuta si ningún `case` coincidió (opcional, pero recomendado) |

#### ⚠️ El error más famoso: olvidar el `break`

Un `case` es solo una **etiqueta de "empezar aquí"**. Si no hay `break`, la ejecución **sigue de largo** en el `case` de abajo:

```c
switch (modo) {
  case 0:
    Serial.printf("BINARIO\n");
    // ¡falta el break!
  case 1:
    Serial.printf("COMPUERTAS\n");
    break;
}
```

Con `modo == 0` imprime **BINARIO y COMPUERTAS**. Compila sin errores y funciona mal. Si un modo "hace cosas de otro modo", lo primero que hay que revisar es si falta un `break`.

> Excepción: si dentro del `case` hay un `return`, la función termina ahí y el `break` no hace falta. Aparece en `evaluar()` del entrenador.

#### `switch` o `if / else`

| Usar `switch` cuando… | Usar `if / else` cuando… |
|---|---|
| Se compara **una sola** variable contra valores fijos | Las condiciones son de distintas variables (`a && b`) |
| Los valores son enteros o un `enum` | Se compara con `<`, `>`, rangos o tiempos |
| Hay 3 o más casos | Hay 1 o 2 casos |

En una máquina de estados se usan **los dos**: el `switch` elige el estado, y adentro de cada `case` los `if` deciden las transiciones.

### 3. Máquinas de estado: del diagrama al código (25 min)

Una **máquina de estados finita** (FSM, *Finite State Machine*) es un sistema que:

- Está **siempre en uno** de un número finito de **estados**.
- En cada estado tiene ciertas **salidas**.
- Pasa de un estado a otro (**transición**) cuando ocurre algo: una entrada, un evento o que pasó cierto tiempo.

Es lo mismo que se ve en la teoría con flip-flops. La diferencia es que aquí la "memoria de estado" es **una variable** y la "lógica de estado siguiente" son **`if`**.

#### El semáforo: el diagrama

```
                    pasaron 5 s                 pasaron 2 s
     inicio   ┌─────────────┐ ───────────► ┌─────────────┐ ───────────► ┌─────────────┐
     ───────► │    VERDE    │              │  AMARILLO   │              │    ROJO     │
              │   L4 = 1    │              │   L1 = 1    │              │   L0 = 1    │
              └─────────────┘              └─────────────┘              └─────────────┘
                     ▲                                                         │
                     └──────────────────── pasaron 5 s ────────────────────────┘
```

En la placa no hay LED amarillo, así que usamos: **L0 (rojo) = luz roja**, **L1 (rojo) = luz "amarilla"**, **L4 (verde) = luz verde**.

#### El semáforo: la tabla de transiciones

| Estado actual | Salida (LEDs) | Condición | Estado siguiente |
|---|---|---|---|
| VERDE | L4 | pasaron 5 s | AMARILLO |
| AMARILLO | L1 | pasaron 2 s | ROJO |
| ROJO | L0 | pasaron 5 s | VERDE |

#### La receta: de diagrama a C en 5 pasos

| Paso | En el diagrama | En C |
|---|---|---|
| 1 | Cada **circulito** | Un valor del `enum`: `enum Estado { VERDE, AMARILLO, ROJO };` |
| 2 | La flecha de **inicio** | La variable de estado, **inicializada**: `Estado estado = VERDE;` |
| 3 | Cada circulito | Un `case` dentro de `switch (estado)`, terminado en `break` |
| 4 | Lo que está **dentro** del circulito (salidas) | Las primeras líneas del `case`: `digitalWrite`, `mostrarByte`… |
| 5 | Cada **flecha que sale** | Un `if (condición) { estado = OTRO; }` dentro del `case` |

Si hay tiempos, se suma una variable `inicioEstado` que guarda **cuándo se entró** al estado, y la condición "pasaron 5 s" es `millis() - inicioEstado >= 5000` (el patrón no bloqueante de la Clase 7).

El resultado ([`ejemplos/02_semaforo`](ejemplos/02_semaforo/src/main.cpp)):

```c
enum Estado { VERDE, AMARILLO, ROJO };       // paso 1
Estado estado = VERDE;                       // paso 2
unsigned long inicioEstado = 0;

void cambiarEstado(Estado nuevo) {           // toda transición pasa por aquí
  estado = nuevo;
  inicioEstado = millis();                   // reinicia el "cronómetro" del estado
}

void loop() {
  unsigned long enEstado = millis() - inicioEstado;

  switch (estado) {                          // paso 3
    case VERDE:
      luces(false, false, true);             // paso 4: salida
      if (enEstado >= TIEMPO_VERDE) {        // paso 5: flecha que sale
        cambiarEstado(AMARILLO);
      }
      break;

    case AMARILLO:
      luces(false, true, false);
      if (enEstado >= TIEMPO_AMARILLO) {
        cambiarEstado(ROJO);
      }
      break;

    case ROJO:
      luces(true, false, false);
      if (enEstado >= TIEMPO_ROJO) {
        cambiarEstado(VERDE);
      }
      break;
  }
}
```

Notar que **no hay ni un `delay`**. El `loop()` da miles de vueltas por segundo; en casi todas no pasa nada (no se cumple ninguna condición) y cada tanto una condición se cumple y el estado cambia. Por eso se pueden agregar botones, otro semáforo o un menú, y todo sigue respondiendo.

> 💡 **Conexión con la teoría:** el `switch` completo es la **lógica combinacional** de la máquina: a partir del estado actual y las entradas calcula las salidas y el estado siguiente. La variable `estado` es el **registro de estado** (los flip-flops). Y cada vuelta de `loop()` es como un **pulso de reloj**.

### 4. Detector de secuencia: Moore y Mealy (15 min)

Un clásico de la teoría: detectar una secuencia de entradas. El nuestro detecta **A, A, B** (tres pulsaciones seguidas) y enciende L0.

Aquí las transiciones no ocurren por tiempo sino por **eventos**: cada flanco de A o de B (Clase 7) es un "símbolo" que entra a la máquina.

#### Versión Moore (salida dentro del estado)

```
                       A                  A                   B
  inicio ─► ( ESPERANDO/0 ) ───► ( VIO_A/0 ) ───► ( VIO_AA/0 ) ───► ( DETECTADO/1 )
```

El dibujo muestra solo el "camino feliz". Las demás flechas están en la tabla:

| Estado actual | Llega A | Llega B | Salida L0 |
|---|---|---|---|
| ESPERANDO | VIO_A | ESPERANDO | 0 |
| VIO_A | VIO_AA | ESPERANDO | 0 |
| VIO_AA | VIO_AA | DETECTADO | 0 |
| DETECTADO | VIO_A | ESPERANDO | **1** |

Dos detalles que también aparecen en la teoría:

- **VIO_AA + A → VIO_AA**: si entra "A A A", las dos últimas siguen siendo "A A". No hay que volver a empezar.
- **DETECTADO + A → VIO_A**: esa A puede ser el comienzo de una secuencia nueva (secuencias **superpuestas**).

En código ([`ejemplos/03_detector_secuencia`](ejemplos/03_detector_secuencia/src/main.cpp)), las transiciones van dentro de un `if (flancoA || flancoB)` y la salida va **afuera**, porque depende solo del estado:

```c
if (flancoA || flancoB) {
  switch (estado) {
    case VIO_AA:
      if (flancoA) estado = VIO_AA;
      else         estado = DETECTADO;
      break;
    // ... un case por estado
  }
}

digitalWrite(LEDS[0], estado == DETECTADO);   // salida Moore
```

El ejemplo además enciende **un LED verde por estado** (L4 = ESPERANDO … L7 = DETECTADO), así se ve la máquina "por dentro" mientras se aprietan los botones.

#### Versión Mealy (salida en la flecha)

En una máquina de **Mealy** la salida depende del estado **y de la entrada**: se escribe sobre la flecha. Alcanzan 3 estados:

```
                       A/0               A/0
  inicio ─► ( ESPERANDO ) ───► ( VIO_A ) ───► ( VIO_AA ) ──── B/1 ────► ( ESPERANDO )
```

En C, la salida Mealy es algo que se hace **en el momento de la transición**:

```c
case VIO_AA:
  if (flancoB) {
    Serial.printf("DETECTADO!\n");   // la salida ocurre AL pasar por la flecha
    estado = ESPERANDO;
  }
  break;
```

| | Moore | Mealy |
|---|---|---|
| La salida depende de | Solo el estado | El estado **y** la entrada |
| Dónde se dibuja | Dentro del circulito (`ESTADO/salida`) | Sobre la flecha (`entrada/salida`) |
| En C, la salida va | Afuera de las transiciones, según `estado` | Dentro del `if` de la transición |
| Cantidad de estados | Suele necesitar más | Suele necesitar menos |
| Duración de la salida | Mientras dure el estado | Un instante (o lo que se programe) |

### 5. Una máquina más completa: la cerradura (5 min de demo)

[`ejemplos/04_cerradura`](ejemplos/04_cerradura/src/main.cpp) junta todo: eventos (botones), tiempos y **variables extra** además del estado.

```
                 A o B                    4 teclas y clave OK
  ( BLOQUEADA ) ───────► ( INGRESANDO ) ─────────────────────► ( ABIERTA )
       ▲   ▲                  │   │                                │
       │   └── clave mal  ────┘   │ clave mal                      │ 5 s o C
       │       (< 3 intentos)     │ (3er intento)                  │
       │       o 5 s sin teclas   ▼                                │
       │                     ( ALARMA ) ── 10 s ──┐                │
       └──────────────────────────────────────────┴────────────────┘
```

| Estado | Salida | Sale cuando… |
|---|---|---|
| BLOQUEADA | L0 fijo | Se aprieta A o B → INGRESANDO |
| INGRESANDO | L0 + barra verde con las teclas | 4 teclas → ABIERTA, BLOQUEADA o ALARMA; 5 s sin teclas → BLOQUEADA |
| ABIERTA | L4..L7 | 5 s, o botón C → BLOQUEADA |
| ALARMA | L0..L3 titilando | 10 s → BLOQUEADA |

El estado dice **qué** está haciendo la cerradura; las variables `teclas`, `claveBien` e `intentosFallidos` dicen **cuánto** lleva. Si hubiera que hacer un estado por cada combinación (1 tecla con 0 fallos, 1 tecla con 1 fallo…) habría decenas de estados. Separar "estado" de "datos" es la forma práctica de mantener el diagrama chico.

> 💡 **Conexión con la teoría:** un contador de intentos con 3 valores son 2 flip-flops más. En hardware, la máquina "completa" tendría 4 × 3 × 5 = 60 estados. En software, la mantenemos en 4 estados y unas variables.

---

### 6. Integrador: Mi Entrenador Lógico (90 min)

Llegó el momento. El entrenador va a tener un **menú**: el botón **C** pasa al modo siguiente y **A** y **B** son las entradas del modo activo.

#### Paso 0 — Planificar en papel (10 min)

Antes de tocar el teclado, completar esta tabla con los modos **propios** (mínimo 4, ver la entrega):

| # | Modo | Viene del TP | Qué hace A | Qué hace B | Qué muestran los LEDs |
|---|---|---|---|---|---|
| 1 | BINARIO | Clase 2 | pausa / sigue | vuelve a 0 | el contador en binario |
| 2 | COMPUERTAS | Clases 3 y 4 | entrada A | entrada B | una compuerta por LED (L6 = A, L7 = B) |
| 3 | TABLA DE VERDAD | Clase 4 | compuerta siguiente | anterior | número de compuerta |
| 4 | REGISTRO | Clase 5 | rotar | invertir | el registro |
| 5 | 7 SEGMENTOS | Clase 6 | dígito +1 | dígito −1 | los segmentos |
| 6 | CONTADOR | Clase 7 | +1 | −1 | el contador |
| 7 | SEMÁFORO / CERRADURA | Clase 8 | (según la FSM) | | las luces |

Y el diagrama del menú, que **también es una máquina de estados** (cada modo es un estado; la entrada es el flanco de C):

```
              C               C                       C
  inicio ─► [1 BINARIO] ─► [2 COMPUERTAS] ─► ... ─► [7 SEMAFORO]
                 ▲                                        │
                 └─────────────────── C ──────────────────┘
```

#### Paso 1 — El esqueleto que compila (15 min)

Empezar por el menú **con los modos vacíos**. Que compile, que C cambie de modo y que se imprima el nombre. Recién después se le agrega contenido. El esqueleto está en [`ejemplos/01_menu_enum_switch`](ejemplos/01_menu_enum_switch/src/main.cpp); adaptado al entrenador queda así:

```c
enum Modo { MODO_BINARIO, MODO_COMPUERTAS, MODO_CONTADOR, MODO_SEMAFORO, CANTIDAD_MODOS };
Modo modo = MODO_BINARIO;

void modoBinario(bool flancoA, bool flancoB) {
  // por ahora vacío
}
// ... una función por modo, todas vacías

void loop() {
  bool flancoA = seApreto(BOT_A);    // los botones se leen UNA vez, aquí arriba
  bool flancoB = seApreto(BOT_B);
  bool flancoC = seApreto(BOT_C);

  if (flancoC) {
    entrarModo((Modo)((modo + 1) % CANTIDAD_MODOS));
  }

  switch (modo) {
    case MODO_BINARIO:    modoBinario(flancoA, flancoB);   break;
    case MODO_COMPUERTAS: modoCompuertas();                break;
    case MODO_CONTADOR:   modoContador(flancoA, flancoB);  break;
    case MODO_SEMAFORO:   modoSemaforo();                  break;
    default:              entrarModo(MODO_BINARIO);        break;
  }
}
```

Observar la idea: **el `loop()` no hace casi nada**. Lee entradas, atiende el menú y le pasa el control a la función del modo activo. Cada modo es una función que se llama **miles de veces por segundo** y nunca se queda trabada.

#### Paso 2 — Las funciones utilitarias compartidas (10 min)

Hay funciones que usan varios modos. Se escriben **una sola vez**, arriba de los modos:

| Función | De qué clase | Para qué |
|---|---|---|
| `void mostrarByte(uint8_t valor)` | 5 | Mostrar cualquier byte en los 8 LEDs |
| `void imprimirBinario(uint8_t valor)` | 5 | Imprimir los 8 bits por el monitor |
| `uint8_t rotarIzquierda(uint8_t x)` | 5 | Rotación circular del registro |
| `bool seApreto(int boton)` | 7 | `true` una vez por pulsación, con antirrebote |
| `bool evaluar(int compuerta, bool a, bool b)` | 4 | Calcular AND (0), OR (1), XOR (2), NAND (3), NOR (4), XNOR (5); 6 y 7 devuelven A y B |

**`seApreto` es la misma función de la Clase 7**, con su bloque completo (`CANT_BOTONES`, `BOTONES`, `ANTIRREBOTE_MS`, `apretadoAntes[]`, `ultimoCambio[]`). Copiarla **tal cual**, sin cambiarle nada: ya está probada y funciona. Lo único que le sumamos en la Clase 8 es el `enum Boton` para llamarla con nombres. Lo mismo con `mostrarByte`, `imprimirBinario` (Clase 5), `SEGMENTOS` (Clase 6) y `evaluar` (Clase 4): se traen de los TPs. `evaluar` e `imprimirNombre` pueden quedar con su cadena de `if / else if` o, ahora que se conoce el `switch`, reescribirse con él.

Si aparece el mismo `for` de 3 líneas copiado en dos modos, eso es una función que falta. "Sin bloques copiados y pegados" es uno de los requisitos mínimos.

#### Paso 3 — Traer los modos de a uno (40 min)

Tomar el TP de una clase, convertirlo en función de modo, **probarlo**, y recién ahí pasar al siguiente. Nunca pegar tres modos juntos sin probar: si algo falla no se sabrá cuál fue.

Cómo convertir el `loop()` de un TP en una función de modo:

| En el TP había… | En el entrenador va… |
|---|---|
| `void loop() { ... }` | `void modoContador(bool flancoA, bool flancoB) { ... }` |
| `if (seApreto(0)) { ... }` adentro del `loop()` del TP | `if (flancoA) { ... }`: se usa el parámetro, porque `seApreto` ya se llamó **una vez** en el `loop()` del entrenador |
| `delay(500);` para ir más lento | `if (millis() - ultimoPaso >= 500) { ... }` |
| Variables locales que "recuerdan" (`static`) | Globales con nombre claro (`contador`, `registro`), para poder reiniciarlas al entrar |
| `pinMode` en el `setup()` del TP | Una sola vez, en el `setup()` del entrenador |
| Prints dentro del `loop()` sin condición | Prints **solo cuando algo cambia** (si no, se inunda el monitor) |

Ejemplo, el contador del TP 7:

```c
// ANTES (TP 7): el loop entero era el contador
void loop() {
  if (seApreto(0)) {             // A: sumar
    contador++;
    mostrarContador();
  }
  if (seApreto(1)) {             // B: restar
    contador--;
    mostrarContador();
  }
  // ... y el modo AUTO
}

// DESPUÉS (entrenador): una función que recibe los flancos ya listos
void modoContador(bool flancoA, bool flancoB) {
  if (flancoA) {
    contador++;
  }
  if (flancoB) {
    contador--;
  }
  mostrarByte(contador);
}
```

#### Paso 4 — Entrar a un modo: anunciar y reiniciar (10 min)

Al cambiar de modo pasan tres cosas, y conviene juntarlas en **una** función `entrarModo()`:

1. Se imprime el nombre del modo y qué hacen A y B.
2. Se **reinician las variables** de ese modo (si no, el contador empieza donde había quedado, o el semáforo aparece en la mitad del amarillo).
3. Se muestra el número de modo en los LEDs un ratito (un "cartel").

```c
void entrarModo(Modo nuevo) {
  modo = nuevo;
  inicioModo = millis();                     // empieza el cartel
  Serial.printf("\n===== Modo %d: ", modo + 1);
  switch (modo) {
    case MODO_CONTADOR:
      Serial.printf("CONTADOR (A: +1, B: -1) =====\n");
      contador = 0;                          // reiniciar SUS variables
      break;
    // ... un case por modo
  }
}
```

Y el cartel, **sin `delay`**: durante los primeros 800 ms del modo se muestra su número; después se ejecuta el modo.

```c
if (millis() - inicioModo < CARTEL_MS) {
  mostrarByte(modo + 1);          // "1", "2", ... en binario en los LEDs
} else {
  switch (modo) { /* ... los modos ... */ }
}
```

> ¿Y si se pone `delay(800)` y listo? Funciona, pero durante 800 ms el entrenador está sordo: si se aprieta C dos veces rápido, la segunda se pierde. Un `delay` cortito en `entrarModo()` es aceptable si se aclara en un comentario; uno **dentro de un modo** (que se llama todo el tiempo) congela el menú y no es aceptable.

#### Paso 5 — Ordenar el archivo (5 min)

Un programa de 300 líneas se lee bien si siempre está ordenado igual. Usar este orden, con un comentario-separador en cada sección:

| # | Sección | Qué va |
|---|---|---|
| 0 | Encabezado | Nombre, clase, qué hace, cómo se usa (qué botón hace qué) |
| 1 | Pines y constantes | `LEDS`, botones, tiempos, `SEGMENTOS`, los `enum` |
| 2 | Variables globales | El `modo`, el antirrebote, y las variables de cada modo (agrupadas y comentadas) |
| 3 | Funciones utilitarias | El bloque de `seApreto` de la Clase 7, `mostrarByte`, `imprimirBinario`, `evaluar`… |
| 4 | Los modos | Una función por modo, cada una con un comentario de qué hacen A y B |
| 5 | El menú | `entrarModo()` |
| 6 | `setup()` y `loop()` | Al final, cortitos |

(Las funciones van **antes** de usarse: por eso `setup()` y `loop()` quedan al final.)

Nombres y comentarios:

- Funciones con verbo: `mostrarByte`, `imprimirTabla`, `entrarModo`. Modos con prefijo: `modoBinario`, `modoContador`.
- Constantes en MAYÚSCULAS: `CARTEL_MS`, `TIEMPO_VERDE`. Nada de números mágicos sueltos.
- Un comentario útil dice **para qué**, no repite el código. Mal: `contador++; // suma 1 al contador`. Bien: `contador++; // 255 + 1 desborda a 0, como un contador de 8 bits`.

La versión completa de la cátedra, con los 7 modos, está en [`referencia/entrenador_completo`](referencia/entrenador_completo/src/main.cpp). **Usarla para comparar organización, no para copiar**: en la defensa habrá que explicar cada línea y modificarla en vivo.

#### Las reglas de oro del entrenador

| Regla | Por qué |
|---|---|
| Los botones se leen **una sola vez**, al principio del `loop()` | Si dos funciones llaman a `seApreto(BOT_A)`, la primera "se come" el flanco (actualiza `apretadoAntes`) y la segunda nunca lo ve |
| **Ningún `delay`** dentro de un modo | Congela el menú: el botón C deja de responder |
| Cada `case` termina en `break` | Si no, se ejecuta también el modo de abajo |
| La variable `modo` empieza **inicializada** | Si no, el entrenador empieza en un modo cualquiera |
| Al entrar a un modo se **reinician** sus variables | Si no, el modo empieza "a la mitad" |

---

### 7. Ejercicios

Son para practicar máquinas de estado. Cualquiera de ellos puede ser **el modo 7** del entrenador.

**Ejercicio 1 — Semáforo con botón peatonal (Mealy).**
Partiendo del semáforo: agregar un semáforo peatonal (L3 = rojo peatón, L7 = verde peatón) y el botón A como pulsador del peatón. Sin peatones el verde de los autos dura 10 s; si alguien aprieta A, el verde se **acorta**: termina apenas se cumplan 3 s de verde. Cuando el peatón aprieta, L2 se enciende al instante ("ESPERE"). Que el verde peatonal titile los últimos 2 s.
*Pista: guardar el pedido en una variable `bool pedido`. La condición para salir del verde queda `enEstado >= 10000 || (pedido && enEstado >= 3000)`.*

**Ejercicio 2 — Detector de 1011.**
Entrada bit a bit: A = 0, B = 1. Dibujar primero el diagrama de Moore (5 estados) y la tabla de transiciones; después programarlo. L0 se enciende al detectar `1011`. Tiene que detectar secuencias superpuestas: con `1011011` detecta **dos** veces. Como extra, mostrar los últimos 4 bits ingresados en L4..L7 (un registro de desplazamiento, Clase 7).
*Pista: escribir una función `Estado siguienteEstado(Estado actual, int bit)` que sea la tabla de transiciones en C.*

**Ejercicio 3 — Máquina expendedora.**
Un producto cuesta \$300. A = moneda de \$100, B = moneda de \$200, C = cancelar. Estados: ESPERANDO, ACUMULANDO (el crédito se ve como barra en L0..L3), ENTREGANDO (3 s, L4..L7 encendidos, imprime el vuelto) y DEVOLVIENDO (2 s, rojos titilando, imprime cuánto devuelve).

**Ejercicio 4 (bonus) — Dado electrónico.**
Con `diagram_7seg.json`. A tira el dado: durante 1,5 s los números cambian rápido (estado RODANDO, un número nuevo cada 80 ms con `random(1, 7)`) y después queda fijo el resultado (estado MOSTRANDO). Usar la tabla `SEGMENTOS` de la Clase 6.

---

## ⭐ Entrega final — Mi Entrenador Lógico

Es **el** trabajo del módulo. Requisitos mínimos (de [EVALUACION.md](../../EVALUACION.md)):

1. Un **menú** que se recorre con los pulsadores (C cambia de modo).
2. Al menos **4 modos** de los vistos en el curso: binario, compuertas, tabla de verdad, bits, 7 segmentos, contador, FSM.
3. Código organizado en **funciones**, sin bloques copiados y pegados.
4. Una **máquina de estados con `enum` y `switch`** (el menú ya lo es; si además hay un modo FSM como el semáforo o la cerradura, mejor).

Además:

- Que **no se cuelgue** y que **responda a los botones** en todo momento (nada de `delay` largos).
- Comentario de encabezado con nombre y apellido, qué hace y **cómo se usa** (qué botón hace qué en cada modo).
- Prolijo: indentado (`Shift+Alt+F`), nombres claros, el orden de secciones del paso 5.
- Al cambiar de modo, imprime el nombre del modo y qué hacen A y B.

Entrega: carpeta del proyecto PlatformIO `SL2026-Apellido-Entrenador` comprimida en ZIP, sin la carpeta `.pio`, antes de la fecha de defensa (ver [EVALUACION.md](../../EVALUACION.md#cómo-se-entrega-un-tp)).

### Antes de entregar, revisar

- [ ] Recorrer todos los modos con C, dos vueltas completas. ¿Cada uno empieza "limpio"?
- [ ] En cada modo, apretar A y B muchas veces rápido. ¿Responde a todas?
- [ ] Apretar C en medio de cada modo (por ejemplo, en el amarillo del semáforo). ¿Cambia al instante?
- [ ] Buscar `delay` en el código (`Ctrl+F`). ¿Hay alguno dentro de un modo?
- [ ] Buscar `seApreto`. ¿Se llama solo al principio del `loop()`, una vez por botón?
- [ ] Cada `case` tiene su `break`.
- [ ] Se puede explicar **cada línea** en voz alta.

---

## La defensa

Son 10 minutos: **demo** (3), **explicación** de una parte que elige el docente (4) y una **modificación en vivo** (3). Todo lo que hay que saber, las preguntas típicas y un banco de modificaciones para practicar están en [`DEFENSA.md`](DEFENSA.md).

En la última media hora de esta clase ensayamos en parejas: uno hace de docente con el banco de modificaciones y el otro defiende. Después se cambian.

> Sobre la IA en esta clase: se puede usar como **revisor** ("¿qué problemas hay en este código?"), no como autor. La modificación en vivo de la defensa se hace sin IA, y ahí se nota.

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Declarar un `enum` y explicar qué valor numérico tiene cada nombre.
- [ ] Escribir un `switch` con `case`, `break` y `default`, y explicar qué pasa si falta un `break`.
- [ ] Pasar un diagrama de estados dibujado a código: `enum` + variable de estado + `switch` + un `if` por flecha.
- [ ] Explicar la diferencia entre una máquina de Moore y una de Mealy, y dónde va la salida en cada caso en C.
- [ ] Temporizar estados con `millis()` sin usar `delay`.
- [ ] Tener el entrenador con menú y al menos 2 modos funcionando (el resto se termina para la entrega).

---

## Material de la clase

- [Slides de la clase](https://claude.ai/artifact/Ya3viSSAqro5TZTFAsfJnY): la presentación para proyectar en el aula (también en [PDF](Slides_Clase_8.pdf)).
- [`ejemplos/`](ejemplos/): menú con `enum` y `switch`, semáforo (Moore), detector de secuencia A-A-B y cerradura electrónica.
- [`referencia/`](referencia/): soluciones de los ejercicios (semáforo peatonal, detector 1011, expendedora, dado) y el **entrenador completo** de la cátedra (**no abrir antes de intentarlo**).
- [`DEFENSA.md`](DEFENSA.md): cómo preparar la defensa, preguntas típicas, banco de modificaciones en vivo y rúbrica.
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
