# Defensa del Entrenador Lógico

**Guía para el alumno.** Leerla entera antes del día de la defensa.

La defensa no es un examen sorpresa: es una charla de 10 minutos sobre el código **propio**. Quien lo escribió y lo entiende ya tiene la mitad hecha. Esta guía es para la otra mitad: saber qué se pregunta y practicar.

---

## Formato (10 minutos)

| Parte | Tiempo | Qué hace el alumno | Qué mira el docente |
|---|---|---|---|
| 1. Demo | 3 min | Muestra el entrenador funcionando en Wokwi (o en la placa): recorre los modos con C y en cada uno muestra qué hacen A y B | Que cumpla los mínimos, que no se cuelgue, que responda a los botones |
| 2. Explicación | 4 min | El docente señala una parte del código (una función, un `case`, una línea) y el alumno la explica | Que entienda qué hace, por qué está ahí y qué pasaría si se la cambia |
| 3. Modificación en vivo | 3 min | El docente pide un cambio chico y el alumno lo hace ahí mismo | Que sepa **dónde** tocar y **cómo**. No hace falta que salga perfecto |

Requisitos mínimos del entrenador (de [EVALUACION.md](../../EVALUACION.md)):

- Un **menú** que se recorre con los pulsadores.
- Al menos **4 modos** de los vistos en el curso.
- Código organizado en **funciones**, sin bloques copiados y pegados.
- Una **máquina de estados con `enum` y `switch`**.

> La regla del curso: hay que poder explicar **cada línea** del programa. Si no se puede, no se aprueba. No importa si el código es corto o simple: importa que sea propio.

---

## Cómo prepararse

### Una semana antes

- [ ] El entrenador cumple los 4 mínimos y está entregado (link de Wokwi `SL2026 - Apellido - Entrenador`).
- [ ] Se pasó el checklist "Antes de entregar" del [README de la clase](README.md#antes-de-entregar-revisar).
- [ ] Se leyó el código **de arriba a abajo en voz alta**, explicando cada línea como si se le contara a un compañero. Donde hubo trabas, ahí hay que estudiar.
- [ ] Se dibujó en papel el **diagrama de estados** del menú y del modo FSM (semáforo, cerradura…). Lo pueden pedir.
- [ ] Se hicieron al menos 5 modificaciones del banco de abajo, sin mirar ninguna solución.

### El día de la defensa

- [ ] Wokwi abierto con el proyecto, **ya probado** esa mañana.
- [ ] Antes de la modificación en vivo, hacer **Save a copy** (o tener el código copiado en otro lado): si algo sale mal, se vuelve atrás sin problemas.
- [ ] Si se lleva la placa real: cargada con la última versión y con un cable USB que transmita datos.
- [ ] Monitor serie visible: la mitad de la demo se ve ahí.

### Consejos para la demo (3 min)

- Tener un **guion**: "Empieza en BINARIO, cuenta solo; A pausa, B reinicia. Con C paso a COMPUERTAS…". No improvisar el orden.
- Mostrar lo que hace bien: secuencias superpuestas del detector, el desborde del contador (255 + 1 = 0), que el menú responde aunque el semáforo esté en el medio del amarillo.
- Si algo falla en la demo, decirlo y explicar por qué se cree que falla. Saber diagnosticar también suma.

### Consejos para la modificación en vivo (3 min)

1. **Repetir el pedido** con palabras propias: "O sea, que con A reste en vez de sumar".
2. **Decir en voz alta dónde se va a tocar** antes de tocar: "Esto está en `modoContador`, en el `if (flancoA)`". Esto ya es la mitad de la nota de esta parte.
3. Hacer el cambio **más chico posible**. Compilar. Probar.
4. Si no compila, leer el **primer** error (Clase 1). Si no sale en los 3 minutos, explicar qué falta: se evalúa el razonamiento.

---

## Preguntas frecuentes

No son todas, pero son las más comunes. Quien puede responder estas con su código abierto está bien preparado.

**Sobre C en general**

1. ¿Qué diferencia hay entre `setup()` y `loop()`? ¿Cuántas veces por segundo se ejecuta `loop()`?
2. ¿Por qué `contador` es `uint8_t` y no `int`? ¿Qué pasa cuando vale 255 y se le suma 1?
3. ¿Qué diferencia hay entre una variable **global** y una **local**? ¿Por qué esta variable es global?
4. ¿Qué hace `(valor >> i) & 1` dentro de `mostrarByte`?
5. ¿Qué diferencia hay entre `&&` y `&`? ¿Y entre `=` y `==`?
6. ¿Para qué sirve esta función? ¿Qué recibe y qué devuelve?

**Sobre entradas y tiempo**

7. ¿Por qué el botón apretado se lee como `LOW`? ¿Qué es `INPUT_PULLUP`?
8. ¿Qué es un **flanco**? ¿Por qué el contador no suma mil veces cuando se mantiene A apretado?
9. ¿Qué es el **rebote**? Explicar `seApreto` (la de la Clase 7) línea por línea: ¿para qué sirven `apretadoAntes` y `ultimoCambio`?
10. ¿Por qué no se usó `delay()`? ¿Qué pasaría si se pusiera `delay(2000)` en el semáforo?
11. ¿Qué significa `millis() - inicioEstado >= TIEMPO_VERDE`?
12. ¿Por qué `seApreto` se llama una sola vez por botón, al principio del `loop()`? ¿Qué es `BOT_A`?

**Sobre máquinas de estado**

13. ¿Qué es un `enum`? ¿Cuánto vale `MODO_TABLA`? ¿Y `CANTIDAD_MODOS`?
14. ¿Qué pasa si se borra este `break`? (Lo pueden hacer probar.)
15. Dibujar el diagrama de estados del semáforo / cerradura / menú.
16. ¿La máquina es de Moore o de Mealy? ¿Por qué?
17. ¿Dónde se reinician las variables de un modo? ¿Qué pasaría si no se reiniciaran?
18. ¿Qué hace `(Modo)((modo + 1) % CANTIDAD_MODOS)`?

---

## Banco de modificaciones en vivo

Ordenadas por dificultad. El docente elige una según cómo venga la defensa. **Practicarlas**: tomar el código propio, hacer la modificación, probarla y deshacerla.

La columna "Dónde se toca" es una pista para practicar; en la defensa, encontrar el lugar es parte de lo que se evalúa.

### Nivel 1 — Un número o una línea

| # | Pedido | Dónde se toca |
|---|---|---|
| 1 | "Que el contador cuente para atrás con A y para adelante con B." | En el modo contador: intercambiar `++` y `--` |
| 2 | "Que el modo binario cuente el doble de rápido." | La constante del intervalo (`PASO_BINARIO_MS`) |
| 3 | "Que el verde del semáforo dure 2 segundos." | La constante `TIEMPO_VERDE` |
| 4 | "Que el registro arranque en `10101010`." | Donde se reinicia el registro al entrar al modo (`0xAA`) |
| 5 | "Que el número de modo se vea 2 segundos al cambiar." | La constante del cartel (`CARTEL_MS`) |

### Nivel 2 — Unas líneas en un solo lugar

| # | Pedido | Dónde se toca |
|---|---|---|
| 6 | "Agregar la compuerta NOR" (o la que falte). | `evaluar()` e `imprimirNombre()` (un `case` o un `else if` nuevo) y donde se muestran las compuertas |
| 7 | "Que el registro rote a la derecha en vez de a la izquierda." | El modo registro: invertir los desplazamientos `<<` y `>>` |
| 8 | "Que el contador sea decimal: después de 9 vuelve a 0." | El modo contador: un `if` después del `++` (y el caso del `--`) |
| 9 | "Que el 7 segmentos solo muestre de 0 a 9." | El modo 7 segmentos: el `% 16` pasa a `% 10` (ojo con el "−1") |
| 10 | "Que el entrenador arranque en el modo 3." | `setup()`: el modo con el que se llama a `entrarModo` |
| 11 | "Que en cada cambio de modo se imprima también cuántos milisegundos pasaron desde que arrancó." | `entrarModo()`: un `printf` con `millis()` y `%lu` |

### Nivel 3 — Tocar varias partes o agregar un estado

| # | Pedido | Dónde se toca |
|---|---|---|
| 12 | "Agregar al semáforo un estado ROJO_AMARILLO de 1 s entre el rojo y el verde." | El `enum` del semáforo, un `case` nuevo, la flecha que salía del ROJO, y el nombre al imprimir |
| 13 | "Agregar un modo nuevo APAGADO al menú, que apague todos los LEDs." | El `enum Modo` (antes de `CANTIDAD_MODOS`), un `case` en el `switch` del `loop()` y otro en `entrarModo()` |
| 14 | "Que en el semáforo la luz amarilla titile en vez de quedar fija." | El `case` del amarillo: usar `(enEstado / 250) % 2` para alternar |
| 15 | "Que el detector (o la cerradura) reconozca otra secuencia / clave." | La tabla de transiciones (los `if` de cada `case`) o el arreglo `CLAVE` |
| 16 | "Que B en el modo compuertas invierta todas las salidas mientras está apretado." | El modo compuertas: aplicar `~` al byte de salidas antes de mostrarlo |

> Si el entrenador no tiene alguno de esos modos, el docente adapta el pedido a los modos que sí tiene.

---

## Rúbrica

La defensa vale el **25%** de la nota del módulo; el entrenador funcionando, otro 30%, y la calidad del código, 15% (ver [EVALUACION.md](../../EVALUACION.md)).

| Criterio | Excelente | Bien | Suficiente | Insuficiente |
|---|---|---|---|---|
| **Demo** | Recorre todos los modos con un guion claro, explica qué hace cada botón, muestra casos interesantes | Muestra todos los modos, con alguna duda | Muestra los mínimos, algo improvisado | No llega a los mínimos o se cuelga |
| **Explicación** | Explica qué hace, por qué, y qué pasaría si se cambia; usa vocabulario de la materia (flanco, estado, transición, máscara) | Explica qué hace y por qué, con alguna imprecisión | Explica qué hace pero no por qué | No puede explicar la parte señalada |
| **Modificación en vivo** | Encuentra el lugar enseguida, la hace y la prueba | Encuentra el lugar y la hace con alguna ayuda | Encuentra el lugar y explica qué haría, aunque no llegue a terminarla | No sabe dónde tocar |
| **Conexión con la teoría** | Relaciona solo con diagramas de estado, Moore/Mealy, compuertas, contadores | Relaciona cuando se le pregunta | Relaciona con ayuda | No relaciona |

**Condición para aprobar:** poder explicar el código propio y saber **dónde** tocarlo. Un entrenador que funciona perfecto pero que no se puede explicar **no aprueba**. Uno con 4 modos simples que se entienden de punta a punta, sí.
