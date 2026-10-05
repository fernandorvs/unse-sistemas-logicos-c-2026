# Programación en C con ESP32 — Sistemas Lógicos UNSE 2026

**Materia:** Sistemas Lógicos (módulo de programación)
**Universidad Nacional de Santiago del Estero — Facultad de Ciencias Exactas y Tecnologías**
**Carga horaria:** 24 hs (8 clases × 3 hs, una por semana)

> Curso hermano de [unse-iot-esp32-2026](https://github.com/fernandorvs/unse-iot-esp32-2026) (Sistemas Microprogramables), pero desde cero: **no hace falta saber programar**.

---

## La idea

En Sistemas Lógicos se estudian compuertas, tablas de verdad, binario, decodificadores, flip-flops, contadores y máquinas de estado.
En este módulo **todo eso se programa en C** sobre un ESP32, y en el camino se aprende el lenguaje.

| Lo que se ve en la teoría | Lo que se aprende de C para programarlo |
|---|---|
| Sistemas de numeración (binario, hexa) | Variables, tipos (`int`, `uint8_t`), `printf` |
| Compuertas y álgebra de Boole | `if`, `else`, operadores `&&`, `\|\|`, `!` |
| Tablas de verdad | Bucles `for`, funciones |
| Operaciones con bits | Operadores `&`, `\|`, `^`, `~`, `<<`, `>>`, máscaras |
| Circuitos combinacionales (decodificador, MUX) | Arreglos (*arrays*) y tablas de búsqueda |
| Flip-flops, registros, contadores | Variables que recuerdan estado, `static`, `millis()` |
| Máquinas de estado (Moore / Mealy) | `enum` y `switch` |

**La compuerta que hoy se dibuja en el pizarrón, mañana se programa y enciende un LED.**

---

## Hilo conductor: Mi Entrenador Lógico

Las 8 clases construyen **un único proyecto que crece semana a semana**: un *entrenador lógico* hecho con un ESP32, 8 LEDs y 3 pulsadores.
Cada clase le agrega una función (mostrar números en binario, simular compuertas, imprimir tablas de verdad, decodificar a 7 segmentos, contar, ejecutar una máquina de estados).
En la última clase se integra todo con un menú y se defiende.

---

## Plan de clases

| # | Clase | Qué se aprende de C | Qué queda funcionando |
|---|---|---|---|
| 1 | [Hola ESP32](Clases/Clase%201%20-%20Hola%20ESP32/) | Qué es un programa, `setup()`/`loop()`, `pinMode`, `digitalWrite`, `delay`, `printf`, comentarios | LED que parpadea y mensajes en el monitor serie |
| 2 | [Variables y Números](Clases/Clase%202%20-%20Variables%20y%20Numeros/) | Variables, tipos, asignación, operadores aritméticos, `%d` `%X`, desborde | Contador que se muestra en decimal, hexa y en 4 LEDs |
| 3 | [Decisiones y Compuertas](Clases/Clase%203%20-%20Decisiones%20y%20Compuertas/) | `if`/`else`, `bool`, operadores relacionales y lógicos, `digitalRead` | Pulsadores A y B como entradas de compuertas AND, OR, XOR, NAND |
| 4 | [Funciones y Tablas de Verdad](Clases/Clase%204%20-%20Funciones%20y%20Tablas%20de%20Verdad/) | Funciones, parámetros, `return`, bucles `for` y `while`, primer arreglo (los pines) | El ESP32 imprime la tabla de verdad de cualquier función lógica |
| 5 | [Bits y Máscaras](Clases/Clase%205%20-%20Bits%20y%20Mascaras/) | Operadores bit a bit, desplazamientos, máscaras, `uint8_t` | Los 8 LEDs como un byte: encender, apagar, invertir y rotar bits |
| 6 | [Arreglos y Combinacionales](Clases/Clase%206%20-%20Arreglos%20y%20Combinacionales/) | Arreglos como tablas de búsqueda, tablas de verdad guardadas en arreglos | Decodificador BCD → 7 segmentos y multiplexor |
| 7 | [Estado y Secuenciales](Clases/Clase%207%20-%20Estado%20y%20Secuenciales/) | Variables globales vs locales, `static`, `millis()`, detección de flanco | Flip-flop T, contador binario y antirrebote por software |
| 8 | [Máquinas de Estado e Integrador](Clases/Clase%208%20-%20Maquinas%20de%20Estado%20e%20Integrador/) | `enum`, `switch`, diseño de un programa completo | Entrenador lógico con menú + defensa |

---

## Hardware

Todas las clases usan **la misma placa** (la "placa de cátedra"): ESP32 + 8 LEDs + 3 pulsadores.
Sirven las dos placas de la facultad (las mismas de Microprogramables): **ESP32 DevKit** y **ESP32-C3 Super Mini**.
Pinout, armado en protoboard y archivos de Wokwi en [`placa/`](placa/).

| Elemento | GPIO en el DevKit | GPIO en el C3 |
|---|---|---|
| LEDs `L0`…`L7` (bit 0 … bit 7) | 4, 16, 17, 18, 19, 21, 22, 23 | 0, 1, 3, 4, 5, 6, 7, 10 |
| Pulsador A | 32 | 20 |
| Pulsador B | 33 | 21 |
| Pulsador C | 25 | 9 (botón BOOT) |

Los programas están escritos con los pines del DevKit. En el C3, PlatformIO los traduce solo: **el mismo código sirve para las dos placas**.

## Software: PlatformIO y Wokwi

Cada ejemplo y cada solución es un **proyecto PlatformIO** (como en Microprogramables). Además, la carpeta [`wokwi/`](wokwi/) tiene los mismos programas listos para Wokwi web (`sketch.ino` + `diagram.json`). Se puede probar de las dos formas:

| | Para qué | Cómo |
|---|---|---|
| **VS Code + PlatformIO** | Trabajar en la PC y cargar en la placa real (DevKit o C3) | Abrir la carpeta del proyecto. Instructivo en [`platformio/README.md`](platformio/README.md) |
| **[Wokwi](https://wokwi.com) en el navegador** | Probar sin instalar nada y sin placa, en la PC de la facultad, en casa o en el celular | Pegar el `sketch.ino` y el `diagram.json` de la carpeta [`wokwi/`](wokwi/). Instructivo en [`wokwi/README.md`](wokwi/README.md) |

El `sketch.ino` de `wokwi/` y el `src/main.cpp` del proyecto tienen **exactamente el mismo código**.

El display de 7 segmentos de la Clase 6 está **solo en el simulador**. En la placa real se ve en los 8 LEDs.

---

## Cómo se trabaja cada clase

Cada carpeta `Clases/Clase N/` tiene:

| Archivo | Para qué |
|---|---|
| `README.md` | La guía de la clase: teoría explicada desde cero, ejemplos y ejercicios |
| `ejemplos/` | Programas cortos para probar en clase. Cada uno es un proyecto PlatformIO |
| `referencia/` | Soluciones, también como proyectos PlatformIO (**no abrir antes de intentarlo**) |
| `Guion_Docente.md` | Guion para el docente: tiempos, errores típicos, preguntas |
| `Slides_Clase_N.pdf` | Las slides de la clase en PDF |

Cada proyecto (ejemplo o solución) tiene esta forma:

```
02_blink/
├── platformio.ini      ← las dos placas: env:esp32dev y env:esp32c3
├── src/main.cpp        ← el programa
└── include/placa_c3.h  ← traduce los pines para el C3 (no se toca)
```

---

## Evaluación

**El proyecto es la materia del módulo.** No hay parciales.
En cada clase se entrega la carpeta del proyecto PlatformIO comprimida en ZIP y, al final, se defiende el entrenador lógico.
Detalles en [EVALUACION.md](EVALUACION.md).

---

## Sobre la IA

En este curso **se está aprendiendo a programar**, así que la regla es más estricta que en Microprogramables:

- **Clases 1 a 6:** se puede pedir a una IA (ChatGPT, Gemini, Claude…) que **explique** un error o un concepto. **No** se le pide que escriba el ejercicio.
- **Clases 7 y 8:** se puede usar como **revisor**: el alumno escribe el código y le pide a la IA que lo critique.
- En la defensa hay que poder explicar **cada línea** del programa propio. Si no se puede, no se aprueba.

---

## Estructura del repo

```
unse-sistemas-logicos-c-2026/
├── README.md              ← Este archivo
├── EVALUACION.md          ← Régimen y criterios
├── CHULETA_C.md           ← Resumen de C de una página
├── placa/                 ← Pinout, armado y diagramas de Wokwi
├── platformio/            ← Instructivo de PlatformIO y plantilla para proyectos nuevos
├── wokwi/                 ← Los mismos programas para Wokwi web (sketch.ino + diagram.json)
└── Clases/
    ├── Clase 1 - Hola ESP32/
    ├── ...
    └── Clase 8 - Maquinas de Estado e Integrador/
```
