# Guion Docente — Clase 4

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Escriben un `for` de N vueltas sin mirar y lo siguen a mano en una tabla.
- Recorren los 8 LEDs con `LEDS[i]`.
- Escriben y llaman una función con parámetros y otra con `return`.
- Imprimen la tabla de verdad de una compuerta con dos `for` anidados.

Es la clase más densa hasta ahora: entran **dos** ideas grandes (bucles y funciones). Priorizar que el `for` quede firme: si a las 2 hs el `for` no está claro, recortar la sección de tres variables (4.2) y dejarla para el TP.

---

## Preparación (1 día antes)

- [ ] Tener abierto en Wokwi el `ej3_barrido` de la Clase 1 (el largo y repetitivo) para mostrarlo al lado del `for`.
- [ ] Tener impresa o en el pizarrón la tabla de verdad de **F = A·B + C'** hecha a mano, para compararla con la del ESP32.
- [ ] Revisar los TP3 entregados: anotar en qué orden puso cada uno las compuertas en los LEDs (el TP4 lo reutiliza).
- [ ] Preparar en el pizarrón una tabla vacía de seguimiento (`Momento | i | ¿condición? | ¿Qué pasa?`).

---

## Apertura — Promesa cumplida (5 min) | Proyector

### Mensaje central
*"En la Clase 1 les dije que el barrido era horrible y que lo íbamos a hacer en 5 líneas. Hoy les pago la deuda."*

Mostrar lado a lado el barrido de la Clase 1 (16 líneas, 4 LEDs) y el del ejemplo 01 (5 líneas, 8 LEDs). Correr los dos. Preguntar: *"¿cuánto cambiaría el primero si fueran 100 LEDs? ¿Y el segundo?"*. No explicar todavía el código: generar la curiosidad.

---

## Bloque 1 — El bucle `for` (30 min) | Pizarrón + hands-on

### Mensaje central
*"Un `for` es un contador con tres preguntas: ¿desde dónde arranco?, ¿hasta cuándo sigo?, ¿cómo avanzo?"*

### Dinámica de pizarrón (10 min): "la tabla de seguimiento"
Escribir `for (int i = 0; i < 4; i++)` y completar **entre todos** la tabla de seguimiento, fila por fila, con un alumno diciendo qué pasa en cada momento. Insistir en la última fila: `i` llega a valer 4, la condición da falsa y **no** se ejecuta el cuerpo. Después repetir con `i <= 4` y contar las vueltas (5).

### Hands-on
- Que modifiquen el `for` del contador para que imprima: de 1 a 10; de 10 a 1 (`i--`); los pares de 0 a 20 (`i = i + 2`). Que **predigan** la salida antes de correrlo.
- Experimento del punto y coma: con un cuerpo que imprima `Hola` (sin usar `i`), agregar `;` después del `)` del `for`. Que lo corran y expliquen por qué imprime una sola vez. Después probar con el cuerpo que usa `i`: aparece *"'i' was not declared in this scope"*, un error que confunde mucho si no lo vieron antes. Es un error que van a cometer: mejor que lo vean provocado.

### `while`
- Mostrar el `while` de las potencias y el de "esperar el botón A". El de las potencias se sigue en tabla igual que el `for`.
- Provocar un bucle infinito borrando la línea que actualiza la variable. En Wokwi se ve que el programa "se cuelga". Botón ⏹ y listo.

---

## Bloque 2 — El arreglo de pines (20 min) | Pizarrón

### Mensaje central
*"El `for` sabe contar 0, 1, 2, … El arreglo traduce 'LED número i' a 'pin del LED i'."*

### Dinámica de pizarrón
Dibujar 8 cajitas en fila, numeradas abajo 0 a 7 y con los GPIO adentro (4, 16, 17, …). Preguntar *"¿qué hay en LEDS[3]?"*, *"¿y en LEDS[8]?"*. La respuesta a la segunda es la clave: no existe.

### Puente con Sistemas Lógicos
Arriba de las cajitas escribir los pesos 2⁰, 2¹, …, 2⁷. *"El índice del arreglo es el número de bit. Por eso empieza en 0."*

- No profundizar en arreglos (no mostrar arreglos de otros tipos, ni modificar elementos): es tema de la Clase 6.
- Hands-on: que reescriban el barrido para que vaya de L7 a L0 (`for (int i = 7; i >= 0; i--)`).

---

## Bloque 3 — Funciones (50 min) | Pizarrón + hands-on

### Mensaje central
*"Una función es ponerle nombre a una idea. `parpadear(L0, 3)` se lee solo; ocho líneas de `digitalWrite` y `delay`, no."*

### Secuencia sugerida
1. **Sin parámetros** (`imprimirSeparador`): 5 min. Que la llamen 3 veces.
2. **Con parámetros** (`parpadear`): 10 min. Dibujar en el pizarrón la llamada `parpadear(LEDS[0], 3)` y flechas de cada valor a su parámetro. Insistir en que el **orden** importa.
3. **Con `return`** (`cuadrado`): 10 min. Metáfora: *"la llamada es una pregunta; el `return` es la respuesta, que ocupa el lugar de la pregunta"*. Mostrar `Serial.printf("%d", cuadrado(7))`.
4. **Variables locales**: 5 min. Provocar el error de usar `resultado` en `setup()`.
5. **Orden / prototipo**: 5 min. Mover una función abajo de `setup()`. En PlatformIO da `was not declared in this scope`; mostrar el error y arreglarlo con el prototipo. Comentar que en Wokwi (`sketch.ino`) puede compilar igual porque Arduino genera prototipos, y que en C estándar no.
6. **Funciones compuerta y `mostrarNumero`**: 15 min. Seguir `mostrarNumero(6)` a mano en el pizarrón con la tabla de la guía.

### Puente con Sistemas Lógicos
Escribir en el pizarrón **S = f(A, B)** y abajo `bool f(bool a, bool b)`. Una compuerta es una función; un circuito combinacional es una función que llama a otras (como un circuito que conecta compuertas).

### Errores a anticipar
- Llamar a la función sin paréntesis: `imprimirSeparador;` (compila con warning y no hace nada).
- Poner el tipo en la llamada: `parpadear(int 4, int 3);`.
- Olvidar el `return` en una función `bool` o `int`.
- Definir una función **adentro** de `loop()`.

---

## Bloque 4 — Tablas de verdad (35 min) | Pizarrón + hands-on

### Mensaje central
*"Una tabla de verdad es probar todas las combinaciones. Probar todas las combinaciones es exactamente lo que hace un bucle."*

### Dinámica de pizarrón: "el cuentakilómetros"
Para los `for` anidados, dibujar dos ruedas (A y B). B gira completa (0, 1) y recién ahí A avanza un lugar. Escribir las filas en el orden en que se generan y mostrar que coincide con el orden de la tabla que hacen en teoría.

### Hands-on
- Correr el ejemplo 03 (tabla AND) y cambiar la compuerta a OR, XOR, NAND.
- Pasar a 3 variables con un solo `for`. En el pizarrón, completar entre todos la columna `fila / 4 % 2` para las 8 filas.
- Comparar la tabla de **F = A·B + C'** impresa por el ESP32 con la hecha a mano. Si no coinciden, ¿quién se equivocó? (Suele ser la hecha a mano.)

### Puente con Sistemas Lógicos
Número de fila = número de mintérmino. Esto prepara el Ejercicio 4 (Σm) y la Clase 6 (tablas guardadas en arreglos).

---

## Bloque 5 — Ejercicios (45 min)

- El Ejercicio 2 (De Morgan) es el mejor para comentar en voz alta: *"acaban de demostrar un teorema del álgebra de Boole probando todos los casos. Eso se llama inducción perfecta, y es lo que hacen a mano en la teoría."*
- En el Ejercicio 4 la dificultad son las comas. Dejar que se equivoquen (`Σm(3,5,6,7,)`) y que lo arreglen solos. Pista si se traban: *"¿la coma va antes o después del número? ¿Cuándo NO va?"*
- Los que terminan rápido arrancan con el TP.

---

## Cierre (5 min)

- Mostrar el TP funcionando: tablas al arrancar y el modo compuertas del TP3 resuelto con dos `for`. Comparar en el proyector el `loop()` del TP3 (8 `digitalWrite` y 6 variables) con el del TP4.
- Recordar la entrega (`SL2026 - Apellido - Clase 4`).
- Adelanto de la Clase 5: *"hoy sacamos los bits de un número dividiendo por 2. La semana que viene les muestro que C tiene operadores para tocar los bits directamente, y los 8 LEDs se vuelven un byte."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| El `for` ejecuta el cuerpo **una sola vez**, o `'i' was not declared in this scope` | `;` después del paréntesis: `for (...);` |
| Un LED de más hace cosas raras, o lee basura | `i <= 8` en lugar de `i < 8`: se sale del arreglo (`LEDS[8]` no existe) |
| La tabla tiene una fila de menos (falta la del 1) | `a < 1` en lugar de `a <= 1` |
| El programa se cuelga | `while` cuya condición nunca se vuelve falsa |
| `'resultado' was not declared in this scope` | Usar una variable local fuera de su función |
| `'mostrarNumero' was not declared in this scope` | La función está definida **abajo** de donde se usa y no hay prototipo |
| La función devuelve valores raros | Falta el `return`, o hay código útil después del `return` |
| `expected primary-expression before 'int'` | Escribir el tipo en la llamada: `f(int 3)` |
| Llamada que "no hace nada" | Falta el `()`: `imprimirSeparador;` |
| `a function-definition is not allowed here` | Función escrita adentro de `loop()` o `setup()` |
