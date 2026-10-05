# Clase 1 — Hola ESP32

**Duración:** 3 hs
**Objetivo:** escribir, correr y modificar los primeros programas en C. Al salir, un LED parpadea porque **el propio alumno** se lo ordenó.

---

## Por qué arrancamos así

No se necesita experiencia previa en programación. Hoy no vamos a ver teoría de lenguajes: vamos a escribir un programa de 5 líneas, correrlo y ver qué pasa. Después lo rompemos a propósito para aprender a leer los errores.

---

## Contenido

### 1. ¿Qué es programar? (20 min)

Un **microcontrolador** (como el ESP32) es una computadora pequeña en un solo chip. No tiene pantalla ni teclado: tiene **pines**, patitas que pueden poner 3,3 V o 0 V (salidas) o leer si les llega tensión (entradas).

Un **programa** es una lista de **instrucciones** que el micro ejecuta **de a una, en orden, de arriba hacia abajo**.

El micro no entiende C: entiende unos y ceros (código máquina). Por eso hay un **compilador**, un programa que traduce lo que se escribe en C a código máquina.

```
  Código en C      ──►  Compilador  ──►  Código máquina  ──►  ESP32 lo ejecuta
   (main.cpp)          (botón ✓)         (unos y ceros)
```

Si se escribe algo que el compilador no entiende, **no traduce nada** y muestra un **error**. Es normal: los programadores con años de experiencia ven errores todos los días.

### 2. Nuestro laboratorio: PlatformIO y Wokwi (20 min)

Cada programa del curso es un **proyecto PlatformIO**: una carpeta con el código en `src/main.cpp` y un `platformio.ini` que dice para qué placa compilar. Es el mismo entorno que se usa en Microprogramables.

Se puede probar de dos formas, con el mismo código:

| | Cómo |
|---|---|
| **VS Code + PlatformIO** (en la PC) | Instalar una sola vez siguiendo [`platformio/README.md`](../../platformio/README.md). *File → Open Folder…* y elegir la carpeta del proyecto. Compilar con ✓ y cargar en la placa con →. |
| **[Wokwi](https://wokwi.com) en el navegador** (sin instalar nada) | Armar la placa de cátedra siguiendo [`placa/README.md`](../../placa/README.md) (es copiar y pegar un archivo) y pegar el código en `sketch.ino`. Los programas del curso están listos para Wokwi en [`wokwi/`](../../wokwi/). Correr con ▶. |

Para el TP de hoy: copiar la carpeta [`platformio/plantilla/`](../../platformio/plantilla/) y renombrarla `SL2026-Apellido-Clase1`.

La placa puede ser un **ESP32 DevKit** o un **ESP32-C3 Super Mini**: se elige en la barra azul de abajo de VS Code (`env:esp32dev` o `env:esp32c3`). El código es el mismo.

### 3. El primer programa (30 min)

Escribir esto en `src/main.cpp` (o en `sketch.ino` de Wokwi) y compilar:

```c
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.printf("Hola, mundo!\n");
}

void loop() {
}
```

Cargarlo en la placa y abrir el **monitor serie** (🔌 en VS Code; en Wokwi aparece solo abajo del circuito): muestra el texto `Hola, mundo!`. ¡Ese es el primer programa!

La primera línea, `#include <Arduino.h>`, le avisa al compilador que vamos a usar las funciones del ESP32 (`Serial`, `pinMode`, `delay`...). Va siempre, al principio de todo programa.

#### La anatomía de un programa de ESP32

Todo programa tiene **dos bloques obligatorios**:

```c
void setup() {
  // Lo que está aquí se ejecuta UNA vez, al arrancar.
}

void loop() {
  // Lo que está aquí se repite PARA SIEMPRE.
}
```

```
  encender ──► setup() ──► loop() ──► loop() ──► loop() ──► ... (hasta que se apague)
```

#### Las reglas de escritura (sintaxis)

| Regla | Ejemplo |
|---|---|
| Cada instrucción termina con **punto y coma** `;` | `delay(500);` |
| Los bloques se encierran entre **llaves** `{ }` | `void loop() { ... }` |
| C distingue **mayúsculas y minúsculas** | `Serial` ≠ `serial`, `HIGH` ≠ `high` |
| El texto va entre **comillas dobles** | `"Hola"` |
| `//` empieza un **comentario**: el compilador lo ignora | `// esto es para humanos` |
| Los espacios y los "enter" no importan, pero **la sangría ayuda a leer** | (ver los ejemplos) |

#### Imprimir: `Serial.printf`

- `Serial.begin(115200);` abre la comunicación con la PC a 115200 baudios (bits por segundo). Va **una vez**, en `setup()`.
- `Serial.printf("texto\n");` imprime el texto en el monitor serie.
- `\n` significa "salto de línea". Sin `\n`, todo sale en un mismo renglón.

> **¿`\n` o `\r\n`?** Vienen de la máquina de escribir, donde terminar un renglón eran dos movimientos: `\r` (*retorno de carro*, código 13) vuelve al principio del renglón y `\n` (*salto de línea*, código 10) baja un renglón. Linux, macOS y el C estándar usan solo `\n`; Windows y muchos protocolos usan `\r\n`, y `Serial.println()` de Arduino agrega `\r\n` solo. En el curso usamos `\n`, que el monitor de PlatformIO y el de Wokwi muestran bien. Si alguna terminal muestra el texto "en escalera" (cada renglón empieza donde terminó el anterior), cambiar a `\r\n`.

> `printf` es **la** función de C estándar para imprimir. Aparece en cualquier libro de C. Aquí le anteponemos `Serial.` para que el texto salga por el USB hacia la PC.

### 4. Encender un LED: salidas digitales (40 min)

Tres instrucciones nuevas:

| Instrucción | Qué hace |
|---|---|
| `pinMode(4, OUTPUT);` | Configura el pin 4 como **salida**. Va en `setup()`. |
| `digitalWrite(4, HIGH);` | Pone el pin 4 en **1** lógico (3,3 V). El LED **enciende**. |
| `digitalWrite(4, LOW);` | Pone el pin 4 en **0** lógico (0 V). El LED **apaga**. |
| `delay(500);` | **Espera** 500 milisegundos (medio segundo) sin hacer nada. |

> 💡 **Conexión con la teoría:** `HIGH` y `LOW` son el **1** y el **0** de Sistemas Lógicos. Un pin de salida es literalmente una variable binaria que se puede ver con un LED.

El clásico **Blink** ([`ejemplos/02_blink`](ejemplos/02_blink/src/main.cpp)):

```c
void setup() {
  pinMode(4, OUTPUT);          // L0 es salida
}

void loop() {
  digitalWrite(4, HIGH);       // encender
  delay(500);                  // esperar
  digitalWrite(4, LOW);        // apagar
  delay(500);                  // esperar
}                              // ... y loop() vuelve a empezar
```

**Pregunta para pensar:** ¿qué pasa si se borra el segundo `delay(500)`? Probarlo. ¿Por qué el LED parece estar siempre prendido?

### 5. Aprender a leer errores (20 min)

Romper el Blink a propósito, **de a un error por vez**, y leer qué dice el compilador:

| Romper esto | Mensaje típico |
|---|---|
| Borrar un `;` | `expected ';' before ...` |
| Escribir `digitalwrite` (con w minúscula) | `'digitalwrite' was not declared in this scope` |
| Borrar una `}` | `expected '}' at end of input` |
| Escribir `delay(500` (sin cerrar el paréntesis) | `expected ')' before ';' token` |

Consejos:
- Leer **el primer error**, no el último. Muchas veces un error causa otros diez.
- El mensaje indica el **número de línea**. El error está en esa línea **o en la anterior** (un `;` que falta se nota recién en la línea siguiente).

### 6. Ejercicios (50 min)

**Ejercicio 1 — Tocar los números.**
Partiendo del Blink: hacer que el LED esté prendido 100 ms y apagado 900 ms. Después probar con 50 ms y 50 ms. ¿Hasta qué valor el ojo distingue el parpadeo?

**Ejercicio 2 — SOS.**
Hacer que L0 transmita **SOS** en código Morse: tres destellos cortos (200 ms), tres largos (600 ms), tres cortos, y una pausa de 2 segundos. Que además imprima `SOS` en el monitor serie.

**Ejercicio 3 — Barrido.**
Encender L0, L1, L2 y L3 **de a uno**, en secuencia, como las luces del auto fantástico. Pines: L0 = 4, L1 = 16, L2 = 17, L3 = 18.
*(Queda un código largo y repetitivo. Está bien: en la Clase 4 aprendemos a acortarlo.)*

---

## ⭐ TP Clase 1 — Mi entrenador se presenta

Escribir un programa que:

1. Empiece con un **comentario de encabezado** con el nombre del alumno, la clase y qué hace el programa.
2. Al arrancar, imprima un **cartel de presentación** con el nombre del alumno, por ejemplo:
   ```
   ==============================
     Entrenador Logico de Ana
     Sistemas Logicos - UNSE 2026
   ==============================
   ```
3. En el `loop()`, encienda L0, L1, L2 y L3 **uno por uno** (quedando prendidos), cada medio segundo, e imprima cuál se encendió.
4. Apague los cuatro, imprima `Todos apagados` y espere un segundo antes de repetir.

Entrega: carpeta del proyecto PlatformIO `SL2026-Apellido-Clase1` comprimida en ZIP, sin la carpeta `.pio` (ver [EVALUACION.md](../../EVALUACION.md#cómo-se-entrega-un-tp)).

---

## Checklist de salida

Al terminar la clase, se debe poder:

- [ ] Explicar con palabras propias qué es un programa y qué hace el compilador.
- [ ] Decir la diferencia entre `setup()` y `loop()`.
- [ ] Imprimir un mensaje en el monitor serie.
- [ ] Encender y apagar un LED con `pinMode` y `digitalWrite`.
- [ ] Leer un error del compilador y encontrar la línea que lo causa.

---

## Material de la clase

- [Slides de la clase](https://claude.ai/artifact/8G1Ysvv7cJyxeu2gbCEBDg): la presentación para proyectar en el aula (también en [PDF](Slides_Clase_1.pdf)).
- [`ejemplos/`](ejemplos/): hola mundo, blink y secuencia de dos LEDs.
- [`referencia/`](referencia/): soluciones de los ejercicios y del TP (**no abrir antes de intentarlo**).
- [`Guion_Docente.md`](Guion_Docente.md): guion para el docente.
