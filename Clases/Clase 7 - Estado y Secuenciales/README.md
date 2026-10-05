# Clase 7 — Estado y Secuenciales

**Duración:** 3 hs
**Objetivo:** que el programa **recuerde** lo que pasó antes. Con eso se programan flip-flops, contadores y registros, y los pulsadores se leen como corresponde: un apretón = una acción, sin `delay(250)`.

---

## Por qué empezamos así

En la Clase 6 todo era **combinacional**: la salida dependía solamente de lo que se estaba apretando **en ese momento**. Por eso alcanzaba con una tabla.

Pero un contador no se puede hacer con una tabla: para responder "¿qué número sigue?", hace falta saber **en qué número se estaba**. Eso es un circuito **secuencial**, y en la teoría se resuelve con **flip-flops**, que son memorias de 1 bit.

Hoy se verá que en C la memoria es algo ya conocido: **una variable**. Pero no cualquier variable: una que **sobreviva** entre una vuelta de `loop()` y la siguiente.

Y de paso cumplimos la promesa de la Clase 5: vamos a arreglar los pulsadores.

---

## Contenido

### 1. Combinacional vs. secuencial: la memoria (15 min)

| | Combinacional | Secuencial |
|---|---|---|
| La salida depende de... | Solo las entradas **actuales** | Las entradas actuales **y lo que pasó antes** |
| Necesita memoria | No | **Sí** |
| Ejemplos de la teoría | Compuertas, decodificador, MUX | Flip-flops, contadores, registros |
| En C | Una función o una tabla: `salida = TABLA[entrada]` | Una **variable que recuerda** entre vueltas de `loop()` |

Pensar en dos botones de una casa:
- El **timbre** suena **mientras** se lo aprieta: es combinacional.
- La **luz de un velador con pulsador** se enciende con un apretón y se apaga con otro: tiene que **recordar** si estaba encendida. Es secuencial.

> 💡 **Conexión con la teoría:** el **estado** de un circuito secuencial es lo que tienen guardado sus flip-flops. En C, el estado del programa es lo que tienen guardado sus variables. Un flip-flop = una variable `bool`. Un registro de 8 flip-flops = una variable `uint8_t`.

### 2. Dónde vive una variable: local, global y `static` (25 min)

Hasta ahora las variables se declaraban sin preocuparse demasiado de **dónde**. Hoy importa, porque de eso depende que la variable **recuerde** o **se olvide**.

#### Variable local: nace y muere en cada llamada

```c
void loop() {
  int contador = 0;     // se crea CADA vez que empieza loop()
  contador++;
  Serial.printf("%d\n", contador);   // imprime 1, 1, 1, 1... ¡siempre 1!
}
```

Una variable declarada **adentro** de una función (o de un bloque `{ }`) es **local**:
- Solo se puede usar adentro de esas llaves (ese es su **alcance**).
- Se crea cuando el programa entra a las llaves y **se destruye al salir** (esa es su **vida**). La próxima vez empieza de nuevo con el valor inicial.

`loop()` es una función que se llama una y otra vez: **todo lo local a `loop()` se olvida en cada vuelta**.

#### Variable global: vive todo el programa

```c
int contador = 0;       // AFUERA de toda función: es global

void loop() {
  contador++;
  Serial.printf("%d\n", contador);   // 1, 2, 3, 4... ¡recuerda!
}
```

Una variable declarada **afuera** de todas las funciones (arriba del todo, como `LEDS`) es **global**:
- Se puede usar desde **cualquier** función.
- Se crea al encender y vive **hasta que se apaga** el ESP32. Se inicializa **una sola vez**.

#### Variable local `static`: lo mejor de los dos mundos

```c
void contar() {
  static int contador = 0;   // se inicializa UNA vez, la primera
  contador++;                // y conserva su valor entre llamadas
  Serial.printf("%d\n", contador);   // 1, 2, 3, 4...
}
```

La palabra `static` delante de una variable local hace que **no se destruya al salir**: tiene el alcance de una local (nadie de afuera la ve) y la vida de una global (recuerda).

| Tipo | Dónde se declara | Quién la puede usar (alcance) | Cuánto vive | ¿Recuerda entre vueltas? |
|---|---|---|---|---|
| Local | Adentro de una función | Solo esa función | Mientras se ejecuta la función | **No** |
| Global | Afuera de todo | Todas las funciones | Todo el programa | **Sí** |
| Local `static` | Adentro, con `static` | Solo esa función | Todo el programa | **Sí** |

Ejecutar [`ejemplos/01_global_local_static`](ejemplos/01_global_local_static/src/main.cpp) y observar los tres contadores lado a lado.

**¿Cuál usar?** En este curso, para el **estado** del entrenador (el contador, el modo, el valor de un flip-flop) usamos **globales**, declaradas arriba, juntas y con nombres claros. Así se ve de un vistazo "qué recuerda" el programa. `static` es útil cuando una función necesita recordar algo que a nadie más le importa.

> 💡 **Conexión con la teoría:** en el diagrama de un circuito secuencial hay una parte combinacional y un bloque de **memoria** que se realimenta. En el programa, la parte combinacional son las cuentas y los `if` de `loop()`, y la memoria son las **variables globales**: lo que `loop()` deja escrito en una vuelta lo lee en la siguiente.

### 3. Detección de flanco: un apretón = una acción (25 min)

#### El problema

En la Clase 5 leíamos un pulsador así:

```c
if (digitalRead(BOTON_A) == LOW) {   // ¿está apretado?
  contador++;
  delay(250);
}
```

Esto pregunta por el **nivel**: "¿está apretado **ahora**?". Si se lo mantiene apretado, cuenta 4 veces por segundo. Y si se le quita el `delay`, cuenta **miles** de veces por segundo, porque `loop()` da miles de vueltas mientras el dedo está abajo.

Lo que queremos es detectar el **momento** en que el botón **pasa** de suelto a apretado. Eso es un **flanco**.

```
 Pin del botón (con INPUT_PULLUP):

 HIGH ────────┐              ┌──────────
  (suelto)    │  apretado    │
 LOW          └──────────────┘
              ▲              ▲
       flanco descendente   flanco ascendente
       (se APRETÓ)          (se SOLTÓ)
```

Con `INPUT_PULLUP`, apretar hace que el pin baje de `HIGH` a `LOW`: por eso "se apretó" es un **flanco descendente** del pin.

#### La solución: recordar cómo estaba antes

Para saber si algo **cambió**, hay que comparar **cómo está ahora** con **cómo estaba antes**. Y para saber cómo estaba antes... hace falta memoria: una variable global.

```c
bool apretadoAntes = false;   // GLOBAL: cómo estaba el botón en la vuelta anterior

void loop() {
  bool apretadoAhora = (digitalRead(BOTON_A) == LOW);

  if (apretadoAhora && !apretadoAntes) {
    // antes suelto, ahora apretado: ¡FLANCO! Pasa una sola vez por apretón
    contador++;
  }

  apretadoAntes = apretadoAhora;   // anotar para la próxima vuelta
}
```

| `apretadoAntes` | `apretadoAhora` | Qué pasó |
|---|---|---|
| `false` | `false` | Sigue suelto |
| `false` | `true` | **Se apretó** (flanco de bajada del pin) |
| `true` | `true` | Sigue apretado |
| `true` | `false` | **Se soltó** (flanco de subida del pin) |

En general: **hay un flanco cuando `apretadoAntes != apretadoAhora`**. Si además `apretadoAhora` es `true`, fue un apretón; si es `false`, fue una soltada.

Probar [`ejemplos/02_nivel_vs_flanco`](ejemplos/02_nivel_vs_flanco/src/main.cpp): cuenta a la vez "por nivel" y "por flanco". Mantener A apretado y comparar.

> 💡 **Conexión con la teoría:** un flip-flop **disparado por flanco** hace exactamente esto. Por dentro tiene dos latches en cascada (maestro-esclavo): uno "recuerda" el valor anterior del reloj para que el flip-flop cambie **solo** en el instante de la transición, no mientras el reloj está en 1. La variable `apretadoAntes` es ese maestro.

### 4. El rebote y cómo filtrarlo con `millis()` (25 min)

#### El rebote mecánico

Un pulsador por dentro son dos chapitas que se tocan. Al apretarlo, las chapitas **rebotan** unas cuantas veces durante algunos milisegundos antes de quedar firmes:

```
 Lo que se imagina:     Lo que pasa de verdad:

 HIGH ──┐               HIGH ──┐ ┌┐ ┌┐
        │                      │ ││ ││
 LOW    └─────────      LOW    └─┘└─┘└──────────
                               |<-- ~1 a 10 ms -->|
```

Para nosotros es un solo apretón, pero el ESP32 es tan rápido que ve **varios flancos**: el contador salta de a 2 o de a 3. El viejo `delay(250)` "tapaba" el problema porque, después del primer flanco, el programa se quedaba dormido mientras rebotaba.

> En la placa real el rebote aparece seguro. **Wokwi también puede simular rebote** en sus pulsadores, así que si en el ejemplo 02 a veces un apretón cuenta 2 o 3 flancos, es eso. Si no se observa en el simulador, no importa: el antirrebote tiene que estar igual, porque el programa tiene que funcionar en la placa.

#### `millis()`: el reloj del ESP32

`millis()` devuelve **cuántos milisegundos pasaron desde que se encendió** el ESP32. No espera nada: solo consulta la hora y sigue.

```c
unsigned long ahora = millis();
Serial.printf("Encendido hace %lu ms\n", ahora);   // %lu = unsigned long
```

| Detalle | Por qué |
|---|---|
| Se guarda en `unsigned long` | Es un entero **sin signo de 32 bits**: llega hasta 4 294 967 295 ms ≈ **49,7 días**. En un `int` (con signo) se desborda a los ~24 días y da números negativos. |
| Se imprime con `%lu` | `l` de *long*, `u` de *unsigned*. |
| Para medir tiempo, se **resta** | `millis() - ultimoCambio` = cuánto tiempo pasó desde `ultimoCambio`. |

#### Antirrebote: ignorar cambios demasiado seguidos

La idea: aceptamos un cambio del botón **solo si pasaron al menos 50 ms desde el último cambio aceptado**. Los rebotes llegan todos juntos en pocos milisegundos, así que quedan ignorados.

```c
const unsigned long ANTIRREBOTE_MS = 50;

bool apretadoAntes = false;
unsigned long ultimoCambio = 0;      // cuándo aceptamos el último cambio

void loop() {
  bool apretadoAhora = (digitalRead(BOTON_A) == LOW);

  if (apretadoAhora != apretadoAntes) {                 // ¿cambió?
    if (millis() - ultimoCambio >= ANTIRREBOTE_MS) {    // ¿y no es un rebote?
      ultimoCambio = millis();
      apretadoAntes = apretadoAhora;
      if (apretadoAhora) {
        contador++;                                     // un apretón de verdad
      }
    }
  }
}
```

> ⚠️ **Siempre restar: `millis() - ultimoCambio >= intervalo`.** No escribir `millis() >= ultimoCambio + intervalo`. Las dos parecen iguales, pero cuando `millis()` llega a su máximo (a los 49 días) vuelve a 0 (¡el desborde de la Clase 2!). La versión con resta **sigue funcionando** al pasar por 0, porque la resta de `unsigned long` también "da la vuelta" y el resultado es el tiempo correcto. La versión con suma falla justo en ese momento. Un sistema embebido puede estar encendido meses: conviene acostumbrarse a la forma correcta desde hoy.

### 5. Una función para los tres botones (10 min)

Con un botón está bien tener `apretadoAntes` y `ultimoCambio` sueltas. Pero tenemos **tres** botones, y cada uno necesita **su propia memoria**. Copiar el bloque tres veces con `apretadoAntesA`, `apretadoAntesB`, `apretadoAntesC`... funciona, pero es justo el problema que resolvimos en la Clase 6: **muchas variables iguales → un arreglo**.

```c
const int CANT_BOTONES = 3;
const int BOTONES[CANT_BOTONES] = {BOTON_A, BOTON_B, BOTON_C};  // 0 = A, 1 = B, 2 = C
const unsigned long ANTIRREBOTE_MS = 50;

bool apretadoAntes[CANT_BOTONES] = {false, false, false};  // memoria de cada botón
unsigned long ultimoCambio[CANT_BOTONES] = {0, 0, 0};      // cuándo cambió por última vez

// Devuelve true UNA sola vez por cada pulsación del botón (0 = A, 1 = B, 2 = C).
bool seApreto(int boton) {
  bool apretadoAhora = (digitalRead(BOTONES[boton]) == LOW);
  bool hayFlanco = false;

  if (apretadoAhora != apretadoAntes[boton]) {                 // ¿cambió?
    if (millis() - ultimoCambio[boton] >= ANTIRREBOTE_MS) {    // ¿y no es un rebote?
      ultimoCambio[boton] = millis();
      apretadoAntes[boton] = apretadoAhora;
      if (apretadoAhora) {
        hayFlanco = true;                                      // cambió de suelto a apretado
      }
    }
  }
  return hayFlanco;
}
```

Es **el mismo código** de la sección anterior, pero cada variable de memoria ahora es un arreglo con una cajita por botón, y el parámetro `boton` dice cuál cajita usar. Se usa así:

```c
void loop() {
  if (seApreto(0)) {    // A
    contador++;
  }
  if (seApreto(1)) {    // B
    contador--;
  }
}
```

**Copiar este bloque tal cual en los programas** de aquí en adelante (y en el entrenador final). Reglas de uso:
- Llamar a `seApreto(n)` **una sola vez por vuelta** para cada botón (cada llamada lee el pin y actualiza la memoria).
- En `setup()`, hacer `pinMode(BOTONES[i], INPUT_PULLUP)` para los tres.
- **Nunca más `delay()` para leer un botón.**

*(¿Por qué no usamos `static` adentro de la función? Porque una variable `static` sería **una sola** compartida por los tres botones. El arreglo global le da una memoria separada a cada uno. En la Clase 8, con `enum`, se les podrá poner nombre a esos 0, 1 y 2.)*

### 6. `millis()` contra `delay()`: hacer varias cosas a la vez (20 min)

Observar este programa: L0 parpadea y además queremos leer el pulsador A.

```c
void loop() {
  digitalWrite(LEDS[0], HIGH);
  delay(1000);
  digitalWrite(LEDS[0], LOW);
  delay(1000);
  if (seApreto(0)) { ... }   // ¡solo se pregunta una vez cada 2 segundos!
}
```

`delay()` **bloquea**: durante ese segundo el ESP32 no hace **nada más**. Si A se aprieta y se suelta mientras está en el `delay`, el programa ni se entera.

La solución es **no esperar, sino mirar el reloj**: en cada vuelta de `loop()` preguntamos "¿ya pasó el tiempo?". Si pasó, hacemos la acción y **anotamos la hora**. Si no pasó, seguimos de largo y atendemos otras cosas.

```c
const unsigned long INTERVALO_L0 = 500;   // ms
const unsigned long INTERVALO_L1 = 300;
unsigned long ultimoL0 = 0;
unsigned long ultimoL1 = 0;
bool estadoL0 = false;
bool estadoL1 = false;

void loop() {
  // Tarea 1: parpadear L0 cada 500 ms
  if (millis() - ultimoL0 >= INTERVALO_L0) {
    ultimoL0 = millis();          // anotar la hora
    estadoL0 = !estadoL0;
    digitalWrite(LEDS[0], estadoL0);
  }

  // Tarea 2: parpadear L1 cada 300 ms (¡a otro ritmo!)
  if (millis() - ultimoL1 >= INTERVALO_L1) {
    ultimoL1 = millis();
    estadoL1 = !estadoL1;
    digitalWrite(LEDS[1], estadoL1);
  }

  // Tarea 3: atender el botón en cada vuelta
  if (seApreto(0)) {
    Serial.printf("Se apreto A\n");
  }
}
```

**El patrón, para memorizar:**

```c
if (millis() - ultimaVez >= INTERVALO) {
  ultimaVez = millis();
  // hacer la acción
}
```

| | `delay(ms)` | Patrón con `millis()` |
|---|---|---|
| Qué hace | Se queda **esperando** | **Mira el reloj** y sigue |
| Mientras tanto | No hace nada más | `loop()` sigue dando vueltas |
| Varias tareas a distinto ritmo | Imposible (o muy difícil) | Una variable `ultimaVez` por tarea |
| Botones | Se pierden pulsaciones | Responden al instante |
| ¿Cuándo usarlo? | Programas de una sola cosa, pruebas rápidas | **Siempre que haya botones** |

> 💡 **Conexión con la teoría:** un sistema secuencial **sincrónico** tiene un **reloj** que marca cuándo cambia el estado. El patrón con `millis()` es un reloj hecho en software: cada `INTERVALO` ms "llega un flanco" y el estado avanza. Y como cada tarea tiene su propio reloj, es como tener varios circuitos funcionando en paralelo.

### 7. Galería de circuitos secuenciales en C (25 min)

Con **memoria** (variables globales), **flancos** (`seApreto`) y **tiempo** (`millis()`) ya se pueden programar los circuitos secuenciales de la teoría. Todos los fragmentos suponen el bloque de `seApreto` de la sección 5.

#### Latch SR (A = Set, B = Reset)

El latch responde al **nivel**, no al flanco: mientras S esté en 1, Q queda en 1.

```c
bool q = false;   // la memoria del latch

void loop() {
  bool s = (digitalRead(BOTON_A) == LOW);
  bool r = (digitalRead(BOTON_B) == LOW);

  if (s && !r) {
    q = true;            // Set
  } else if (r && !s) {
    q = false;           // Reset
  }
  // s = r = 0: mantiene.  s = r = 1: estado prohibido; aquí elegimos mantener.

  digitalWrite(LEDS[0], q);
}
```

Probarlo: apretar y soltar A. L0 queda **encendido** aunque ya no se apriete nada. Eso es memoria.

#### Flip-flop D (A = dato, B = reloj)

En el **flanco** del reloj, Q copia el dato. El resto del tiempo, el dato puede cambiar todo lo que quiera, que Q no se entera.

```c
bool qD = false;

void loop() {
  if (seApreto(1)) {                       // flanco del reloj B
    qD = (digitalRead(BOTON_A) == LOW);    // el dato se lee como nivel
  }
  digitalWrite(LEDS[0], qD);
}
```

#### Flip-flop T (cada flanco de A invierte L0)

```c
bool qT = false;

void loop() {
  if (seApreto(0)) {
    qT = !qT;            // T = 1: invertir
  }
  digitalWrite(LEDS[0], qT);
}
```

¡Es el velador de la sección 1! Los flip-flops D y T están juntos en [`ejemplos/03_flipflops_D_y_T`](ejemplos/03_flipflops_D_y_T/src/main.cpp).

#### Contador binario de 8 bits, ascendente/descendente, con reloj manual

```c
uint8_t contador = 0;   // 8 flip-flops

void loop() {
  if (seApreto(0)) {
    contador++;         // 255 + 1 = 0: el desborde hace de "vuelta"
    mostrarByte(contador);
  }
  if (seApreto(1)) {
    contador--;         // 0 - 1 = 255
    mostrarByte(contador);
  }
}
```

Notar que no hace falta ningún `if` para volver a 0: `uint8_t` tiene 8 bits y desborda solo, igual que un contador de 8 flip-flops.

#### Registro de desplazamiento (A = dato, B = reloj)

```c
uint8_t registro = 0;

void loop() {
  if (seApreto(1)) {                              // flanco del reloj
    bool dato = (digitalRead(BOTON_A) == LOW);
    registro = (registro << 1) | dato;            // todos se corren y entra el dato en L0
    mostrarByte(registro);
  }
}
```

Cada `<<` mueve los 8 bits un lugar hacia `L7`, como 8 flip-flops D en cadena compartiendo el reloj.

#### Divisor de frecuencia / contador asincrónico (ripple)

En un contador **asincrónico** el reloj solo le llega al primer flip-flop T; cada uno de los siguientes usa como reloj la salida del anterior y cambia cuando ese **baja de 1 a 0**. Cada etapa divide la frecuencia por 2.

```c
const unsigned long MEDIO_PERIODO = 250;   // L0 cambia cada 250 ms: 2 Hz
unsigned long ultimoTic = 0;
bool etapa[7];                              // 7 flip-flops T

void loop() {
  if (millis() - ultimoTic >= MEDIO_PERIODO) {
    ultimoTic = millis();
    int i = 0;
    bool propagar = true;
    while (propagar && i < 7) {
      etapa[i] = !etapa[i];
      digitalWrite(LEDS[i], etapa[i]);
      propagar = (etapa[i] == false);      // si bajó de 1 a 0, le da "reloj" al siguiente
      i++;
    }
  }
}
```

L0 parpadea a 2 Hz, L1 a 1 Hz, L2 a 0,5 Hz... y si se observan todos juntos, **cuentan en binario**. Programa completo en [`ejemplos/04_divisor_frecuencia`](ejemplos/04_divisor_frecuencia/src/main.cpp): además, L7 sigue al pulsador A **sin demora**, porque nadie está bloqueando con `delay()`.

> 💡 **Conexión con la teoría:** comparar el contador ripple (cada etapa espera a la anterior, el cambio se "propaga" como una onda) con el contador `contador++` (todos los bits cambian a la vez, como un contador **sincrónico**). El resultado en los LEDs es el mismo; la estructura es distinta. En hardware, la propagación del ripple genera *glitches* y limita la velocidad; por eso en la práctica se prefieren los sincrónicos.

### 8. Ejercicios (35 min)

**Ejercicio 1 — Flip-flops JK y T.**
Con C como reloj (flanco), A = J y B = K: programar un **flip-flop JK** con Q en `L0` y Q negada en `L1` (J K = 00 mantiene, 01 reset, 10 set, 11 invierte). Agregar un **flip-flop T** en `L7` con T = A y el mismo reloj. Imprimir J, K y Q en cada flanco. ¿Por qué se dice que un T es un JK con J = K?

**Ejercicio 2 — Registro de desplazamiento.**
A = dato, B = reloj, C = borrar. En cada flanco de B, todo el byte se corre un lugar hacia `L7` y en `L0` entra el valor de A. Imprimir el registro en binario después de cada reloj. Probar "escribir" el patrón `10110001` bit por bit.

**Ejercicio 3 — Contador BCD (módulo 10).**
A = +1, B = −1, C = reset. Contar de 0 a 9 en `L0`..`L3`: después del 9 viene el 0, y antes del 0 viene el 9. Cuando pasa de 9 a 0, imprimir `Acarreo!` e invertir `L7` (sería el reloj de un segundo contador, el de las decenas).

**Ejercicio 4 — ¿Cuánto tiempo estuvo apretado?**
Cuando se aprieta A, anotar `millis()`. Cuando se suelta, restar e imprimir `A estuvo apretado durante 1234 ms`. **Desafío:** mientras está apretado, que una barra de LEDs vaya creciendo (un LED más cada 250 ms) sin usar `delay()`.

---

## ⭐ TP Clase 7 — El entrenador cuenta

Partir del TP anterior y escribir un programa que:

1. Empiece con el **comentario de encabezado** (nombre, clase, qué hace).
2. Tenga un **contador binario de 8 bits** (`uint8_t`) que se vea en `L0`..`L7`.
3. El pulsador **A** le **suma 1** y el **B** le **resta 1**, detectando el **flanco con antirrebote** (la función `seApreto`). **Prohibido** usar `delay()` en todo el programa.
4. El pulsador **C** alterna entre modo **MANUAL** y modo **AUTO**. En AUTO, el contador suma 1 **solo, cada 500 ms**, usando el patrón con `millis()`. En AUTO, A y B **siguen respondiendo al instante**.
5. Cada vez que el contador cambia, imprima el valor en decimal, hexa y binario, y el modo:
   ```
   Contador:  42 (0x2A) 00101010 [MANUAL]
   ```
6. Cada vez que cambia el modo, lo avise: `Modo AUTO` / `Modo MANUAL`.

Entrega: carpeta del proyecto PlatformIO `SL2026-Apellido-Clase7` comprimida en ZIP, sin la carpeta `.pio` (ver [EVALUACION.md](../../EVALUACION.md#cómo-se-entrega-un-tp)).

> Desde esta clase se puede usar una IA como **revisor**: escribir el TP de forma propia y pedirle que critique el código. En la defensa habrá que explicar cada línea de `seApreto`.

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Explicar la diferencia entre un circuito combinacional y uno secuencial, y qué tiene que ver con la memoria.
- [ ] Decir qué pasa con una variable local, una global y una `static` entre dos vueltas de `loop()`.
- [ ] Detectar el flanco de un pulsador guardando su estado anterior.
- [ ] Explicar qué es el rebote y cómo lo filtra el antirrebote de 50 ms.
- [ ] Usar `millis()` con `unsigned long` y el patrón `millis() - ultimaVez >= INTERVALO`, y explicar por qué se resta.
- [ ] Explicar por qué `delay()` impide atender los botones.
- [ ] Programar un flip-flop T, un flip-flop D, un contador y un registro de desplazamiento.

---

## Material de la clase

- [Slides de la clase](https://claude.ai/artifact/TJrg7863ooMmGDDwc6K6sf): la presentación para proyectar en el aula (también en [PDF](Slides_Clase_7.pdf)).
- [`ejemplos/`](ejemplos/): global/local/static, nivel contra flanco, flip-flops D y T con antirrebote, y divisor de frecuencia ripple con `millis()`.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
