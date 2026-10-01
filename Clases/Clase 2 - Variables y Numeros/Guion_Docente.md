# Guion Docente — Clase 2

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Declaran, asignan e imprimen variables con `printf` y el formato correcto.
- Entienden la división entera y el operador `%` (pueden predecir `17 / 5` y `17 % 5`).
- Muestran un número de 0 a 15 en 4 LEDs y saben explicar **por qué** las cuentas `(n / 4) % 2` dan cada bit.
- Vieron un `uint8_t` desbordar y lo relacionan con la aritmética de n bits de la teoría.

La idea fuerte de la clase: **el método de divisiones sucesivas que hacen en papel en la teoría es exactamente el programa que escriben hoy.** Si se llevan solo una cosa, que sea esa.

Todavía **no hay `if` ni bucles**. Si alguien pregunta "¿y si quiero que haga una cosa u otra?", la respuesta es "la semana que viene". Resistir la tentación de adelantar: que todo se resuelva con aritmética es justamente lo que obliga a entender los pesos binarios.

---

## Preparación (1 día antes)

- [ ] Tener abierto en una pestaña el TP de la Clase 1 de referencia, para partir de ahí.
- [ ] Preparar en el pizarrón (o en una diapositiva) la tabla decimal / binario / hexa de 0 a 15. Se usa en tres bloques.
- [ ] Revisar qué vieron en la teoría de Sistemas Lógicos esta semana (¿ya dieron divisiones sucesivas? ¿hexadecimal?). Ajustar el bloque 5 en consecuencia: si ya lo vieron, es un repaso de 5 minutos; si no, darlo bien.
- [ ] Si hay placa real: llevarla con la extensión del TP cargada (contador de 8 bits). Ver los 8 LEDs contando en binario impresiona.

---

## Bloque 1 — Repaso y una pregunta (10 min) | Pizarrón

### Mensaje central
*"El programa de la clase pasada tenía amnesia: no se acordaba de nada. Hoy le damos memoria."*

### Dinámica
Plantear la pregunta del README (cómo contar 0, 1, 2, 3…). Dejar que propongan. Alguien va a decir "escribo un printf por número". Anotarlo en el pizarrón y preguntar: *"¿y hasta mil?"*. Así aparece la necesidad de algo que **guarde** el número y lo **cambie**.

---

## Bloque 2 — Variables (30 min) | Pizarrón + Hands-on

### Mensaje central
*"Una variable es una cajita con nombre. El tipo dice qué tamaño de cajita es y qué entra."*

### Dinámica sugerida (5 min): las cajitas
Dibujar tres cajas en el pizarrón con etiqueta (nombre) y un cartelito arriba (tipo). Ejecutar "a mano", línea por línea, este código, borrando y escribiendo el valor en la caja:

```c
int a = 5;
int b = 3;
a = a + b;
b = a - b;
a = a - b;
```

Preguntar antes de cada línea: *"¿qué hay en cada caja ahora?"*. (Resultado: intercambiaron valores, a = 3 y b = 5.) Es el ejercicio clásico de "hacer de computadora" y deja claro que el `=` es **asignación**, no igualdad.

### Puntos a remarcar
- **`x = x + 1` no es una ecuación.** Insistir. Es el primer gran choque con la matemática.
- Las variables se **declaran una sola vez** (con el tipo). Después se usan sin el tipo. Error típico: escribir `int edad = 20;` dos veces → `redeclaration of 'int edad'`.
- Pedir que **predigan** la salida del [ejemplo 1](ejemplos/01_variables/sketch.ino) antes de correrlo.

### Puente con Sistemas Lógicos
El `uint8_t` es un registro de 8 bits. Escribir en el pizarrón: 8 bits → 2⁸ = 256 combinaciones → 0..255. Mostrar la placa: 8 LEDs = 8 bits = 1 byte. **"Cada vez que declaran un `uint8_t`, imaginen 8 LEDs."**

---

## Bloque 3 — Operaciones y printf (30 min) | Hands-on

### Mensaje central
*"Entre enteros, la división corta. Lo que sobra lo da el `%`."*

### Dinámica: predecir antes de ejecutar
Antes de correr el [ejemplo 2](ejemplos/02_operaciones/sketch.ino), que cada alumno anote en papel qué va a salir en cada línea. Después corren y comparan. **La mayoría va a fallar `17 / 5`** (ponen 3,4). Ese error es el aprendizaje.

Preguntas rápidas para hacer a la clase (a mano alzada):
- `7 / 2` → 3
- `7 % 2` → 1
- `2 / 7` → 0
- `2 % 7` → 2 (este cuesta: "si no entra ninguna vez, sobra todo")
- `20 % 5` → 0

### Puntos a remarcar
- El `%` adentro de `printf` es especial (formatos). Para imprimir el símbolo `%` se escribe `%%`.
- Formato equivocado → número basura. Mostrar a propósito qué pasa con `printf("%d", 3.5)` (imprime cualquier cosa). El compilador de Arduino a veces ni avisa.
- `%X` muestra el **mismo número**, en otra base. La variable no cambia: cambia cómo se la mira. Conecta perfecto con la teoría ("el número es uno solo; decimal, binario y hexa son formas de escribirlo").

---

## Bloque 4 — `const` para los pines (10 min) | Proyector

- Mostrar el TP de la Clase 1 y preguntar *"¿qué LED es el 17?"*. Nadie se acuerda. Reemplazar en vivo por `L2`.
- Probar `L0 = 5;` dentro de `setup()` para que vean el error `assignment of read-only variable`. El compilador como aliado, otra vez.

---

## Bloque 5 — Decimal a binario en los LEDs (35 min) | Pizarrón + Hands-on

### Mensaje central
*"El método de divisiones sucesivas de la teoría, escrito en C, son cuatro líneas."*

### Dinámica sugerida (10 min): de la pizarra al código
1. En el pizarrón, hacer las divisiones sucesivas del 13 **en columna**, como en la teoría. Marcar los restos.
2. Al lado, escribir la columna de C: `13 % 2`, `(13 / 2) % 2`, `(13 / 4) % 2`, `(13 / 8) % 2`. Calcular cada uno con la clase.
3. Unir con flechas cada resto con su expresión. **Son los mismos números.**
4. Preguntar: *"¿por qué dividir por 4 da lo mismo que dividir dos veces por 2?"*. Que lo prueben con un par de números.

### Dinámica: predecir antes de ejecutar
Cambiar `n` en el [ejemplo 3](ejemplos/03_binario_4leds/sketch.ino) a un valor que diga el docente (por ejemplo 6) y que **antes de correr** cada uno dibuje en su hoja cuáles LEDs van a quedar prendidos. Después ▶. Repetir con 10 y con 15.

### Puntos a remarcar
- `digitalWrite` recibe 0 o 1 y `LOW`/`HIGH` **son** 0 y 1. No hay magia.
- Orden de los LEDs: **L0 es el de la derecha**. Algunos van a esperar ver el número "al revés". Volver a la tabla de pesos.
- Pregunta para los rápidos: *"¿qué pasa si ponen n = 20?"*. (Muestra 0100 = 4, porque 20 = 10100 y el bit 4 no tiene LED. Es un adelanto del desborde.)

---

## Bloque 6 — Contar y desbordar (25 min) | Hands-on

### Mensaje central
*"Un `uint8_t` cuenta como un cuentakilómetros: después de 255 viene 0. Es aritmética módulo 256, la misma de la teoría."*

### Dinámica: el error a propósito
Escribir en el proyector el contador con `int contador = 0;` **adentro** de `loop()`. Pedir que predigan. Correr. Imprime 0, 0, 0… Preguntar por qué antes de mostrar la solución. Que alguien explique con la metáfora de la cajita ("se crea de nuevo cada vez").

### Puente con Sistemas Lógicos
En el pizarrón, sumar en binario 11111111 + 1 con acarreo, columna por columna. El acarreo final queda "colgando" fuera de los 8 bits. *"Ese bit no tiene cajita. Se pierde."* Relacionar con el sumador de 8 bits y el *carry out* que ven (o van a ver) en la teoría.

Comparar las dos formas de dar la vuelta:
- `contador = (contador + 1) % 16;` → la vuelta la ponemos **nosotros** (módulo 16 = 4 bits).
- `uint8_t` con `contador++` → la vuelta la pone el **tamaño del tipo** (módulo 256 = 8 bits).

---

## Bloque 7 — Ejercicios (40 min)

- El **ejercicio 2** (horas:minutos:segundos) es el que más cuesta: no saben cómo sacar los minutos. Guiar con la pregunta *"de 3725 segundos, ¿cuántos sobran después de sacar las horas enteras?"* (`3725 % 3600 = 125`) → *"¿y cuántos minutos enteros hay en 125 segundos?"*.
- En el **ejercicio 1**, la división por cero en el ESP32 **reinicia el micro** (aparece un *Guru Meditation Error* en el monitor serie). Está bien que lo vean: aclarar que no rompe nada, y que en la Clase 3 vamos a poder evitarlo con un `if`.
- El **ejercicio 3** es para hacer **primero en papel**. Recorrer los bancos y pedir ver la hoja antes de que corran.
- Los que terminan rápido arrancan con el TP y su extensión.

---

## Cierre (5 min)

- Mostrar el TP funcionando (y, si hay placa real, la extensión con los 8 LEDs contando).
- Recordar cómo se entrega: `SL2026 - Apellido - Clase 2`.
- Adelanto de la Clase 3: *"hoy el programa hace cuentas, pero siempre hace lo mismo. La semana que viene va a **decidir**: vamos a leer los pulsadores y convertir el ESP32 en compuertas AND, OR y XOR."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| El contador imprime siempre 0 | Variable declarada **adentro** de `loop()` |
| `redeclaration of 'int x'` | Se puso el tipo (`int`) dos veces al mismo nombre |
| `'x' was not declared in this scope` | Se usa la variable antes de declararla, o el nombre está mal escrito (mayúsculas) |
| `assignment of read-only variable 'L0'` | Se intentó cambiar una `const` |
| `7 / 2` da 3 | División entera (no es un error del programa, es así) |
| Imprime números raros o gigantes | Formato que no coincide con el tipo: `%d` con un `float` o `%f` con un `int` |
| Falta el `%` o sale texto raro en el monitor | Quiso imprimir el símbolo `%` sin escribir `%%` |
| Los LEDs muestran el número "al revés" | Confundió el orden: L0 es el bit de **menos** peso (derecha) |
| `stray '\303' in program` o similar | Nombre de variable con `ñ` o tilde (según la versión del compilador) |
| El ESP32 se reinicia con *Guru Meditation Error* | División por cero |
| El contador de 4 LEDs pasa de 15 a 16 y los LEDs se apagan | Falta el `% 16` |
