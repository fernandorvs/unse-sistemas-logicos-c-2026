# Guion Docente — Clase 3

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Leen un pulsador con `digitalRead` y entienden (no solo copian) por qué el pull-up invierte la lógica.
- Escriben `if` / `else` / `else if` correctamente: paréntesis, llaves, sin `;` de más, con `==`.
- Traducen cualquier compuerta de dos entradas y una expresión booleana sencilla a C.
- Comprobaron una ley de De Morgan con el ESP32.

La idea fuerte de la clase: **el álgebra de Boole de la teoría y los operadores lógicos de C son el mismo lenguaje con otros símbolos.** El alumno que entiende eso tiene media materia ganada de los dos lados.

Todavía **no hay funciones propias ni bucles**. Recorrer las tablas de verdad apretando botones es un poco tedioso **a propósito**: genera la necesidad del `for` de la Clase 4.

---

## Preparación (1 día antes)

- [ ] Verificar en Wokwi que los pulsadores de la placa de cátedra responden (clic sostenido = apretado).
- [ ] Coordinar con la teoría: ¿ya vieron De Morgan? ¿suma de productos? ¿la función `F = A·B + C̅`? Si usaron otra función de ejemplo en la teoría, **usar esa** en el ejercicio 1: la conexión vale más que cualquier ejemplo nuevo.
- [ ] Tener dibujadas (o en diapositiva) las tablas de verdad de AND, OR, NOT, NAND, NOR, XOR, XNOR.
- [ ] Si hay placa real: llevar un pulsador suelto, un LED y una resistencia para mostrar físicamente el pull-up con un multímetro (3,3 V suelto, 0 V apretado).

---

## Bloque 1 — Entradas y pull-up (25 min) | Pizarrón + Hands-on

### Mensaje central
*"Con el pull-up, apretado es 0. Lo invertimos una vez, al leer, y nunca más pensamos en eso."*

### Dinámica sugerida (5 min): predecir antes de ejecutar
Antes de explicar el pull-up, correr el [ejemplo 1](ejemplos/01_boton_led/src/main.cpp) **sin** el `!` (con `bool a = lectura;`) y preguntar: *"¿el LED se va a prender cuando aprieto o cuando suelto?"*. Casi todos dicen "cuando aprieto". Correr. Se prende al revés. Ahora sí, explicar por qué.

### Puente con Sistemas Lógicos
- Dibujar el circuito de la resistencia de pull-up con el pulsador a GND y analizarlo como divisor de tensión con dos casos (abierto / cerrado). Es electrónica que ya conocen.
- Nombrarlo con el vocabulario de la teoría: **lógica negativa** o **activo en bajo**, el circulito de negación en la entrada de un bloque. *"Las señales activas en bajo están en todos lados: los `RESET`, los `CS`, los `EN` de los integrados que van a ver."*
- Preguntar: *"¿por qué no conectamos el pulsador a 3,3 V y listo?"*. Llevar a la idea de **entrada flotante**: sin la resistencia, el pin suelto no está ni en 0 ni en 1.

### Puntos a remarcar
- Hay dos formas equivalentes de invertir: `!digitalRead(...)` y `digitalRead(...) == LOW`. Que elijan una y la usen siempre.
- Error típico: olvidarse `pinMode(BOTON_A, INPUT_PULLUP)`. En la placa real el pin flota y lee basura (en Wokwi a veces "funciona", lo que confunde más).

---

## Bloque 2 — `bool` y comparaciones (20 min) | Pizarrón

### Mensaje central
*"Un `bool` es un bit. Una comparación es una pregunta que el programa responde con 1 o 0."*

### Dinámica: preguntas rápidas
Con `int n = 7;` en el pizarrón, preguntar a mano alzada cuánto vale (0 o 1): `n == 7`, `n != 7`, `n > 7`, `n >= 7`, `n % 2 == 1`, `n / 2 == 3`. Lo último repasa la división entera de la clase pasada.

### El error `=` vs `==` (5 min, no saltear)
Escribir en el proyector:

```c
int n = 3;
if (n = 7) {
  Serial.printf("n vale 7\n");
}
Serial.printf("n = %d\n", n);
```

Pedir que **predigan** qué imprime. Correr. Imprime `n vale 7` y `n = 7`. El `if` **cambió** la variable. Si el compilador muestra el *warning* (`suggest parentheses around assignment used as truth value`; en Arduino IDE depende de *Preferences → Compiler warnings*), mostrarlo y remarcar: **los warnings también se leen**.

---

## Bloque 3 — `if` / `else` / `else if` (30 min) | Hands-on

### Mensaje central
*"Siempre se ejecuta un solo camino. Las condiciones se prueban de arriba hacia abajo y gana la primera que se cumple."*

### Dinámica: hacer de computadora
Con el [ejemplo 2](ejemplos/02_par_impar/src/main.cpp) en el proyector, el docente dice un valor de `n` (por ejemplo 15) y la clase dice, **antes de correr**, qué se imprime. Insistir en el `else if`: con `n = 3`, ¿por qué no imprime "mediano" si 3 también es menor que 15? Porque la primera condición ya se cumplió y el resto se saltea.

### Dinámica: romper el `if`
Como en la Clase 1 con los errores, que rompan el `if` a propósito y anoten qué pasa:
1. Agregar `;` después del paréntesis: compila y el bloque se ejecuta **siempre**.
2. Sacar las llaves y poner dos instrucciones: la segunda se ejecuta siempre.
3. Sacar los paréntesis: error de compilación `expected '(' before ...`.
4. Escribir `else` sin `if` delante (por ejemplo, después de un `;` de más): `'else' without a previous 'if'`.

Remarcar que **los dos primeros no dan error**. Son los peores: el programa funciona mal y el compilador no avisa.

---

## Bloque 4 — Operadores lógicos y compuertas (35 min) | Pizarrón + Hands-on

### Mensaje central
*"El · es `&&`, el + es `||`, la barra es `!`. Lo que escriben en la teoría lo pueden copiar en C símbolo por símbolo."*

### Dinámica sugerida (10 min): dos columnas
Dividir el pizarrón en dos columnas: **Teoría** y **C**. Ir llenando juntos la tabla de compuertas: dibujo de la compuerta, expresión booleana, expresión en C. Dejar XOR y XNOR para el final y preguntar: *"¿cómo dirían XOR en palabras?"*. Alguien va a decir "una o la otra pero no las dos" (que se puede escribir `(a || b) && !(a && b)`, también válido). Llevarlos a "**son distintas**" → `a != b`. Que lo verifiquen con la tabla.

### Dinámica: predecir antes de ejecutar
Con el [ejemplo 3](ejemplos/03_compuertas/src/main.cpp) cargado y **sin correr**, que cada alumno complete en una hoja qué LEDs (L0–L3) van a estar prendidos para cada una de las 4 combinaciones de A y B. Después corren y verifican fila por fila.

### Puntos a remarcar
- `digitalWrite(L0, a && b)` funciona porque `a && b` vale 0 o 1. Mostrar que es equivalente al `if`/`else`, pero más corto.
- Los paréntesis en `!(a && b)` **importan**: `!a && b` es otra cosa. Hacer que lo prueben y encuentren en qué fila difiere.
- `&&` vs `&`: si alguien escribe uno solo, por ahora con `bool` "funciona". Decir que es otro operador y que lo vemos en la Clase 5. No profundizar.
- `and`, `or`, `not`, `xor` son **palabras reservadas** en el compilador del ESP32 (C++). Si alguien nombra una variable `and` o `xor`, da un error rarísimo. Sugerir nombres como `salAnd`, `salXor`.

---

## Bloque 5 — De Morgan (15 min) | Pizarrón + Hands-on

### Mensaje central
*"En la teoría lo demuestran con tablas. Aquí el ESP32 lo comprueba en vivo."*

### Dinámica
1. Escribir las dos leyes en el pizarrón en notación de la teoría.
2. Traducirlas a C entre todos.
3. Que lo arranquen como ejercicio 3: verificar las 4 combinaciones.
4. Pregunta trampa: *"Si en vez de `!a || !b` escribo `!a && !b`, ¿en qué fila lo descubro?"*. (En A=1 B=0 y A=0 B=1.) Sirve para mostrar que **una sola fila de prueba no alcanza**: hay que recorrer la tabla entera. Otra vez, la necesidad del `for`.

---

## Bloque 6 — Ejercicios (55 min)

- **Ejercicio 1 (mayoría):** exigir que hagan **primero la tabla de verdad en papel** (8 filas) y saquen la expresión. La forma mínima es `A·B + A·C + B·C`. Si alguien llega a la forma canónica completa (4 mintérminos), también es válida: buen momento para comentar que se puede simplificar (Karnaugh en la teoría).
- **Ejercicio 2 (alarma):** el error típico es el orden de los `else if`. Si ponen primero `else if (armada)` antes que la condición de alarma, nunca suena. Hacerles ejecutar a mano con armada = 1 y puerta = 1.
- **Ejercicio 3 (De Morgan):** el que tiene las dos leyes funcionando y entiende por qué, arranca el TP.
- El TP es mucho texto repetido (8 `pinMode`, 8 `digitalWrite`). Igual que en la Clase 1: *"sí, es largo; en la Clase 4 lo acortamos."*

---

## Cierre (5 min)

- Mostrar el TP funcionando: apretar A, B y los dos, y leer en voz alta la tabla de verdad que aparece en los LEDs.
- Recordar cómo se entrega: `SL2026 - Apellido - Clase 3`.
- Adelanto de la Clase 4: *"hoy recorrieron las tablas de verdad apretando botones. La semana que viene el ESP32 las va a imprimir enteras, solo, para cualquier función que le den."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| El LED se prende al **soltar** y se apaga al apretar | Falta invertir la lectura del pull-up (`!` o `== LOW`) |
| El botón lee valores al azar (en la placa real) | Falta `pinMode(…, INPUT_PULLUP)`: entrada flotante |
| La condición del `if` siempre es verdadera y la variable cambia sola | `=` en vez de `==` |
| `suggest parentheses around assignment used as truth value` | Mismo caso: `if (n = 7)` |
| El bloque del `if` se ejecuta **siempre** | `;` después del paréntesis: `if (a);` |
| Solo la primera línea depende del `if`, la segunda se ejecuta siempre | Faltan las llaves `{ }` |
| `'else' without a previous 'if'` | `;` de más o llave mal cerrada antes del `else` |
| `expected '(' before …` | Condición sin paréntesis: `if a && b` |
| Un `else if` nunca se ejecuta | Una condición anterior más general ya la "atrapó" (el orden importa) |
| NAND da resultados raros | Faltan paréntesis: `!a && b` en vez de `!(a && b)` |
| Error extraño al declarar `bool and = …` o `bool xor = …` | `and`, `or`, `not`, `xor` son palabras reservadas |
| No encuentra la tecla `\|` | Teclado latinoamericano: tecla a la izquierda del `1`. Teclado español: `AltGr` + `1` |
