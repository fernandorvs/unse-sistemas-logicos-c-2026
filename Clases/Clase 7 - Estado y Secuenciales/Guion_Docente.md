# Guion Docente — Clase 7

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Entienden que **secuencial = necesita memoria = variable que sobrevive entre vueltas de `loop()`**.
- Detectan flancos con antirrebote usando `seApreto()` y pueden explicar cada línea.
- Usan el patrón `millis() - ultimaVez >= INTERVALO` en lugar de `delay()`.
- Tienen un contador de 8 bits manejado por botones que responde "como se debe" (un apretón = una cuenta).

Es la clase conceptualmente más densa del módulo: tres ideas nuevas (alcance/vida, flanco, tiempo sin bloquear). **Ir despacio y siempre de la mano de la teoría de flip-flops**, que los alumnos ya vieron o están viendo. Si el tiempo aprieta, recortar la galería del bloque 7, nunca los bloques 2 a 4.

---

## Preparación (1 día antes)

- [ ] Tener un proyecto Wokwi con el ejemplo 02 (nivel vs. flanco) listo para proyectar.
- [ ] Revisar en Wokwi el atributo de rebote de los pulsadores (`"bounce"` en el `diagram.json`) para poder mostrar la diferencia con y sin rebote si hace falta. No prometer que el rebote se va a ver siempre en el simulador.
- [ ] **Si hay placa real, llevarla**: el rebote en un pulsador barato se ve seguro con el ejemplo 02. Es la mejor demostración de la clase.
- [ ] Llevar dibujado (o preparar para el pizarrón) el diagrama de tiempos de un flip-flop D disparado por flanco y el de un contador ripple de 3 bits.

---

## Bloque 1 — Combinacional vs. secuencial (15 min) | Pizarrón

### Mensaje central
*"Si para saber la salida alcanza con mirar las entradas, es combinacional. Si además hay que acordarse de algo, es secuencial. Y acordarse, en C, es una variable que no se borra."*

### Dinámica: "¿qué número sigue?"
Decir en voz alta: "Si yo aprieto el botón de un contador, ¿qué número aparece?". Alguien va a decir "depende de en cuál estaba". Exacto: eso es **estado**. Escribir en el pizarrón la tabla de la sección 1 del README.

### Puente con Sistemas Lógicos
Dibujar el modelo general de un secuencial: bloque combinacional + bloque de memoria (flip-flops) realimentado, con el reloj entrando a la memoria. Señalar: "lo combinacional es lo que ya saben programar (Clase 6). Lo nuevo de hoy es la cajita de memoria."

---

## Bloque 2 — Local, global, static (25 min) | Hands-on

- Empezar con el error a propósito: escribir en vivo un contador con `int contador = 0;` **adentro** de `loop()`. Pedir que **predigan** qué imprime. Muchos van a decir 1, 2, 3... Correrlo: imprime 1, 1, 1. Este es el momento "ajá" de la clase.
- Explicar **alcance** (dónde se ve) y **vida** (cuánto dura) por separado. Metáfora: la variable local es un **pizarrón que se borra** al terminar la clase; la global es el **cartel de la pared**, que queda.
- Correr el ejemplo 01. Preguntar: *"¿cuál de las tres es un flip-flop?"* (las que recuerdan).
- `static`: mostrarlo pero no insistir. La regla práctica del curso es **estado en globales, arriba y juntas**.

---

## Bloque 3 — Flanco (25 min) | Pizarrón + hands-on

- Empezar recordando la promesa de la Clase 5: *"les dijimos que el `delay(250)` era provisorio. Hoy se van a dar cuenta de por qué."*
- Sacar el `delay(250)` del patrón viejo en vivo: el contador vuela a miles. Preguntar por qué: `loop()` da miles de vueltas mientras el dedo está abajo.
- Dibujar la señal del pin con los dos flancos (el de bajada al apretar, el de subida al soltar). Remarcar que con `INPUT_PULLUP` apretar = **bajada**.
- Escribir la tabla de 4 filas (antes/ahora) en el pizarrón. Es una tabla de verdad de un **detector de flanco**: que la completen ellos.
- Correr el ejemplo 02 manteniendo A apretado: `porNivel` sube a miles, `porFlanco` sube 1.

### Puente con Sistemas Lógicos
Dibujar un flip-flop D maestro-esclavo y su diagrama de tiempos. *"El flip-flop no mira si el reloj **está** en 1: mira el **momento** en que pasa a 1. Para eso por dentro guarda el valor anterior. `apretadoAntes` hace lo mismo."* Si la teoría vio el detector de flanco con una compuerta AND y un retardo, mostrar que `apretadoAhora && !apretadoAntes` es **la misma ecuación**.

---

## Bloque 4 — Rebote y `millis()` (25 min) | Pizarrón + hands-on

- Dibujar la señal con rebote. Si hay placa real, correr el ejemplo 02 en ella: un apretón cuenta 2, 3 o más flancos. En Wokwi puede verse o no; **no prometer** que se vea.
- `millis()`: es un reloj, no una espera. Mostrar `Serial.printf("%lu\n", millis());` en el loop para que vean el número crecer.
- **`unsigned long`, no `int`:** preguntar cuánto es el máximo de un `int` de 32 bits (2 147 483 647) y pasarlo a días (~24,8). "Un semáforo hecho con `int` se rompe a las 3 semanas y media."
- **Por qué se resta.** Hacer la cuenta en el pizarrón con números chicos, como si `millis()` fuera un `uint8_t` (0..255) para que se entienda:
  - `ultimoCambio = 250`, intervalo = 50. Pasan 20 ms y `millis()` da la vuelta: vale `14`.
  - **Con resta:** `14 - 250` en `uint8_t` = `20` (da la vuelta también). `20 >= 50`: falso, correcto, todavía no pasó el tiempo. Más tarde, `millis() = 44` → `44 - 250 = 50` → `50 >= 50`: verdadero, justo a tiempo.
  - **Con suma:** `250 + 50 = 300` → en `uint8_t` es `44`. Con `millis() = 14`: `14 >= 44` falso, bien... pero un rato **antes**, con `millis() = 251`: `251 >= 44` es **verdadero** apenas 1 ms después. **Falla.**
  - Moraleja: la resta da **el tiempo transcurrido**, y eso es correcto aunque el reloj dé la vuelta.
- Explicar la lógica del antirrebote: "aceptamos un cambio solo si el último cambio aceptado fue hace más de 50 ms". Los rebotes duran pocos ms, así que quedan afuera.

---

## Bloque 5 — `seApreto()` para los tres botones (10 min) | Hands-on

- Elegimos esta forma (arreglos globales + función con índice) porque reutiliza lo de la Clase 6 y es **la misma lógica** del bloque 4: solo cambia `apretadoAntes` por `apretadoAntes[boton]`. Mostrarlo literalmente así, con las dos versiones una al lado de la otra.
- Por qué **no** `static` adentro de la función: sería una sola memoria para los tres botones. Buena pregunta para hacerles antes de decirlo.
- Que la **copien** tal cual. No es un problema que no la escriban de memoria, pero **sí tienen que poder explicarla** en la defensa. Recorrerla línea por línea con todo el grupo.
- Regla: llamar a `seApreto(n)` **una vez por vuelta** por botón. Si la llaman dos veces en la misma vuelta (por ejemplo en dos `if` distintos), la segunda nunca ve el flanco.

---

## Bloque 6 — `millis()` vs. `delay()` (20 min) | Hands-on

- Mostrar en vivo el programa con `delay(1000)` y un botón: apretar rápido varias veces; casi ninguna se registra.
- Escribir el programa de las dos tareas a distinto ritmo. Preguntar: *"¿cómo harían que L0 parpadee cada 500 ms y L1 cada 300 ms con `delay`?"* (No se puede de forma simple.) Con `millis()` son dos `if` independientes.
- Metáfora: `delay()` es quedarse mirando la pava hasta que hierva. `millis()` es mirar la hora de vez en cuando mientras se hacen otras cosas.
- Escribir el patrón de 3 líneas en una esquina del pizarrón y **dejarlo toda la clase**.

### Puente con Sistemas Lógicos
El patrón con `millis()` es un **reloj** de software. Cada tarea con su `ultimaVez` es un circuito con su propio reloj.

---

## Bloque 7 — Galería de secuenciales (25 min) | Hands-on

- Ir uno por uno, siempre **dibujando primero el símbolo del flip-flop en el pizarrón** y después el código. Que vean que el código "es" el símbolo.
- **Latch SR**: que comparen con un flip-flop: el latch responde al nivel (no usa `seApreto`). Preguntar qué pasa con S = R = 1 en la teoría (prohibido) y qué decidimos en el código.
- **D y T**: correr el ejemplo 03. En el D, que cambien A **sin** tocar B: L0 no se entera. Eso es "disparado por flanco".
- **Contador**: mostrar que `uint8_t` desborda solo (Clase 2). Es un contador módulo 256 sin ningún `if`.
- **Registro**: `(registro << 1) | dato`, conectar con la Clase 5.
- **Ripple** (ejemplo 04): dibujar 3 flip-flops T en cascada y su diagrama de tiempos. Que vean en los LEDs que L1 cambia exactamente cuando L0 se apaga. Comparar con `contador++` (sincrónico). Si la teoría ya vio los *glitches* del ripple, mencionarlos.

---

## Bloque 8 — Ejercicios (35 min)

- El 1 (JK) es casi directo de la tabla de la teoría: buena práctica de "tabla → if/else". Los que vienen rápido pueden hacer el JK **con una tabla** al estilo de la Clase 6 (índice = J*2 + K... más el Q anterior: tabla de 8 filas).
- El 2 (registro) es corto; lo interesante es "escribir" un patrón bit a bit.
- El 3 (BCD) obliga a manejar la vuelta a mano (`if (cuenta > 9)`), a diferencia del `uint8_t` que desborda solo. Buen contraste.
- El 4 (tiempo apretado) es el más completo: usa flanco de bajada **y** de subida, y `millis()` para medir. El desafío de la barra es para los más rápidos.
- Los que terminan empiezan con el TP.

---

## Cierre (5 min)

- Mostrar el TP: contar a mano, pasar a AUTO con C, y mientras cuenta solo apretar A y B: responden al instante. Remarcar: **no hay ni un `delay()` en todo el programa**.
- Recordar la entrega: `SL2026 - Apellido - Clase 7`.
- Desde esta clase pueden usar IA como **revisor** (no como autor).
- Adelanto de la Clase 8: *"Hoy el programa tuvo dos modos, MANUAL y AUTO, con un `bool`. ¿Y si tuviera cinco modos? La semana que viene vemos máquinas de estado con `enum` y `switch`, y armamos el entrenador completo."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| El contador siempre vale 1 (o 0) | **Variable local que "se olvida"**: declararon `int contador = 0;` adentro de `loop()`. Tiene que ser global (o `static`) |
| Un apretón suma cientos o miles | **Detectan nivel en vez de flanco**: `if (digitalRead(BOTON_A) == LOW)` sin comparar con el estado anterior |
| Nunca detecta el flanco | Se olvidaron de `apretadoAntes = apretadoAhora;`, o la pusieron **antes** de la comparación |
| Un apretón suma 2 o 3 | **Rebote**: falta el antirrebote, o `ANTIRREBOTE_MS` muy chico |
| El botón responde una vez sí y otra no | Llaman a `seApreto(n)` **dos veces** en la misma vuelta para el mismo botón |
| El tiempo da negativo o raro | **Guardaron `millis()` en un `int`**: tiene que ser `unsigned long` (e imprimirse con `%lu`) |
| Funciona, pero "por casualidad" | Compararon `millis() > ultimaVez + intervalo` en vez de **restar** `millis() - ultimaVez >= intervalo`. Falla al desbordar `millis()` (49,7 días). Explicarlo con el ejemplo de `uint8_t` del bloque 4 |
| Lo de `millis()` funciona bien la primera vez y después pasa en **todas** las vueltas (el LED queda medio encendido, el contador vuela) | Se olvidaron de **anotar la hora**: falta `ultimaVez = millis();` adentro del `if` |
| Lo de `millis()` no pasa **nunca** | Anotan la hora **afuera** del `if` (en cada vuelta), así que la resta nunca llega al intervalo |
| Los botones no responden en modo AUTO | Usaron `delay(500)` para contar solo. Todo el programa se traba |
| Número basura o reinicio del ESP32 | **Índice fuera del arreglo** en `seApreto(3)` (los botones son 0, 1 y 2) |
| El contador BCD llega a 10 | **Contar desde 1** / comparar mal: usaron `if (cuenta > 10)` o `>= 11`. La vuelta tiene que ser `if (cuenta > 9)` |
