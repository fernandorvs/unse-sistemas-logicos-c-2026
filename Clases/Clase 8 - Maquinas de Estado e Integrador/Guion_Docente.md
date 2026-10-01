# Guion Docente — Clase 8

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Pasaron al menos un diagrama de estados a código con `enum` + `switch` + `millis()`.
- Tienen el esqueleto del entrenador con menú (C cambia de modo) y **al menos 2 modos** funcionando.
- Saben cómo es la defensa y ensayaron una modificación en vivo con un compañero.

Es la última clase. Hay ansiedad por la defensa y alumnos con TPs atrasados. **El objetivo emocional:** que salgan con un plan concreto ("me faltan estos 2 modos y sé cómo traerlos") y no con la sensación de que el integrador es un monstruo. El mensaje que hay que repetir: *"el entrenador no es un programa nuevo: es pegar con un menú lo que ya hicieron, de a un modo por vez".*

---

## Preparación (1 día antes)

- [ ] Tener abierto en Wokwi el [`entrenador_completo`](referencia/entrenador_completo/sketch.ino) funcionando, para la demo inicial.
- [ ] Tener los cuatro ejemplos cargados en pestañas: menú, semáforo, detector, cerradura.
- [ ] Imprimir (o tener en el proyector) el [banco de modificaciones](DEFENSA.md#banco-de-modificaciones-en-vivo): se usa en el ensayo en parejas.
- [ ] Revisar qué TPs entregó cada alumno: el que no tiene TP 5 o 7 no va a poder "traer" esos modos. Pensar de antemano qué modos le conviene integrar.
- [ ] Definir y anunciar las **fechas de defensa** y el cronograma (ver más abajo).
- [ ] Pizarrón libre para dibujar diagramas de estado grandes.

---

## Bloque 1 — `enum` y `switch` (15 min) | Pizarrón + proyector

### Arranque (2 min): la demo
Mostrar el entrenador completo funcionando: recorrer los 7 modos con C. *"Esto es lo que van a tener en dos semanas. Y casi todo ya lo escribieron ustedes."* Que vean el número de modo en los LEDs y el semáforo funcionando mientras igual responde a C.

### `enum` (5 min)
- Escribir en el pizarrón `if (estado == 2)` y preguntar: *"¿qué es 2?"*. Nadie sabe. Después reescribirlo con `enum`.
- Remarcar que el `enum` **es** la asignación de estados de la teoría: el compilador elige los códigos.
- El cast `(Modo)` genera preguntas. Respuesta corta: *"en C++ el compilador no deja meter un entero en un `Modo` sin que se le prometa que es válido. Los paréntesis son la promesa."* No profundizar.

### `switch` (8 min)
- Mostrar el ejemplo 01 en vivo. Después **borrar un `break` a propósito** y correrlo: al cambiar de modo se imprime el nombre de dos modos. Preguntar *"¿por qué?"* antes de explicar.
- Insistir en que compila sin errores: es un error de lógica, no de sintaxis. Es el error número uno de la defensa.

---

## Bloque 2 — Máquinas de estado (45 min) | Pizarrón → código

### Del diagrama al código: el semáforo (20 min)
- **Dibujar el diagrama primero**, con los alumnos: ¿qué estados hay? ¿qué se prende en cada uno? ¿cuándo cambia? Después la tabla de transiciones.
- Aplicar la receta de 5 pasos **en vivo, en el proyector**, escribiendo el código mirando el dibujo: un `enum` por circulito, un `case` por circulito, un `if` por flecha. Que vean que es mecánico.
- Pregunta clave: *"¿y si lo hacemos con `delay(5000)`?"*. Funciona... hasta que quieran agregar un botón. Conectar con la Clase 7: el semáforo con `delay` está sordo 5 s.
- Si sobra tiempo: agregar un `Serial.printf` fuera del `switch` que imprima `millis()` cada segundo, para mostrar que el `loop()` sigue girando.

### Detector de secuencia y Moore/Mealy (15 min)
- Los detectores de secuencia ya los vieron en la teoría (o los van a ver). **Preguntar a los alumnos** cómo era el diagrama antes de mostrarlo.
- Correr el ejemplo 03 y apretar `A A A B`: que vean que detecta. Preguntar por qué VIO_AA + A vuelve a VIO_AA y no a ESPERANDO.
- Los LEDs verdes "uno por estado" son una codificación *one-hot*: si lo vieron en la teoría, mencionarlo.
- Moore vs Mealy: la tabla del README alcanza. Lo importante en C es **dónde va la salida**: afuera (según `estado`) o dentro del `if` de la transición.

### Cerradura (5 min de demo)
- Correr el ejemplo 04 sin explicar el código línea por línea. Probar la clave bien, después 3 veces mal para ver la alarma.
- Mensaje: *"el estado dice qué hace la máquina; las variables extra dicen cuánto lleva"*. Si alguien pregunta "¿por qué no un estado por cada tecla?", calcular juntos cuántos estados harían falta.

### Ejercicios
Los ejercicios no entran en el horario de clase. Presentarlos como **candidatos al modo 7** del entrenador: el que quiera, en vez del semáforo básico, integra el semáforo peatonal o la expendedora.

---

## Bloque 3 — Integrador (90 min) | Trabajo asistido

Es el bloque central. Los alumnos trabajan en **su** proyecto; el docente recorre los bancos.

### Primeros 10 min: todos juntos
- Que cada uno complete en papel la tabla del paso 0 (modos, qué hace A, qué hace B). **No dejar que abran Wokwi antes de tenerla.** El que planifica 10 minutos ahorra una hora.
- Mostrar el orden del archivo (paso 5) en el entrenador completo, haciendo scroll por las secciones.

### Los siguientes 80 min: recorrer los bancos
- **Primer objetivo de todos:** el esqueleto del paso 1 compilando, con modos vacíos y C cambiando de modo. No aceptar "ya traje 3 modos pero el menú no funciona".
- Después, un modo por vez. Cada modo nuevo: compilar, probar, guardar (**Save**). Recordarlo en voz alta cada 20 minutos.
- Los que van atrasados (sin TP 5, 6 o 7): que integren los modos que sí tienen (binario, compuertas, tabla de verdad) + el semáforo del ejemplo 02 **reescrito por ellos**. Con eso llegan a los 4 mínimos.
- Los que van adelantados: el modo 7 con un ejercicio (semáforo peatonal, expendedora), o un modo bonus (dado, Simón dice, calculadora binaria).
- Cuando alguien muestre algo que funciona, pedirle: *"¿Me explica esta línea?"*. Es entrenamiento para la defensa y detecta temprano el código copiado.

### Qué mirar al recorrer
| Si se observa… | Preguntar… |
|---|---|
| `delay` dentro de un modo | "Apriete C mientras corre. ¿Responde?" |
| `seApreto` llamado dentro de los modos | "¿Cuántas veces por vuelta se lee el botón A?" |
| Un modo de 60 líneas | "¿Qué parte de esto se repite en otro modo?" |
| Variables `x`, `aux`, `cont2` | "Si lo leo yo, ¿sé qué es?" |
| Todo el código pegado de la referencia | Ver más abajo: cómo detectar código no propio |
| Una `seApreto` "mejorada" o distinta a la de la Clase 7 | "¿Por qué la cambió?" Debería ser la misma, copiada tal cual |

---

## Bloque 4 — Ensayo de defensa en parejas (30 min)

- Explicar el formato de la defensa (5 min) con [`DEFENSA.md`](DEFENSA.md): demo 3 min, explicación 4 min, modificación 3 min. Mostrar la rúbrica.
- **Parejas (2 × 10 min):** uno hace de docente, el otro defiende. El "docente" elige una modificación de nivel 1 o 2 del banco y señala una función para explicar. Con cronómetro. Después se cambian.
- Cierre del ensayo (5 min): preguntar qué les costó. Casi siempre es "encontrar dónde tocar" o "explicar `seApreto`". Repasar esos dos puntos.

---

## Cierre (incluido en el bloque 4)

- Recordar la entrega: link `SL2026 - Apellido - Entrenador` antes de la fecha de defensa.
- Recordar la regla de la IA: revisor sí, autor no. En la modificación en vivo no hay IA.
- Mensaje final: *"En la clase 1 prendieron un LED. Hoy tienen un programa de 300 líneas con una máquina de estados. Eso es programar."*

---

## Conducción de las defensas

### Organización
- **10 minutos por alumno + 5 de margen** para cambiar de alumno y anotar. Son 4 alumnos por hora: planificar las fechas en función de eso.
- Llamar por orden de lista y publicar el cronograma con anticipación. El que espera puede ir probando su entrenador, pero **no** en la máquina del que defiende.
- Tener a mano una planilla con: modos presentados, parte explicada, modificación pedida, nota de cada criterio de la rúbrica y una línea de observaciones. Llenarla **durante** la defensa, no al final del día.
- Usar cronómetro visible. Si la demo se pasa de 3 min, cortarla con amabilidad: "perfecto, ya vi suficiente".

### Parte 2: qué pedir que expliquen
Elegir según lo que muestre la demo:
- Si la demo fue sólida: pedir lo "difícil" — `seApreto` (es la de la Clase 7: tienen que poder explicarla), el `millis() - inicio >= ...`, el cast `(Modo)`, el `switch` del menú.
- Si la demo fue floja: empezar por algo seguro (`mostrarByte`, un modo simple) para que se afirme, y después subir.
- Siempre al menos una pregunta "¿qué pasaría si…?" (si borro este `break`, si pongo un `delay` aquí, si no reinicio esta variable). Es la que separa memorizar de entender.

### Parte 3: cómo elegir la modificación en vivo
| Cómo viene el alumno | Qué pedir |
|---|---|
| Inseguro, demo justa | Nivel 1 (una constante, intercambiar `++`/`--`). Si sale rápido, una segunda de nivel 2 |
| Normal | Nivel 2 |
| Muy sólido, código prolijo | Nivel 3 (agregar un estado o un modo) |
| Sospecha de código no propio | Una de nivel 1 o 2 **en una parte que no esté en la referencia** (ver abajo) |

Adaptar el pedido a **sus** modos: si no tiene semáforo, no pedirle el estado ROJO_AMARILLO. No hace falta que la modificación salga perfecta: se evalúa que sepa **dónde** y **cómo** tocar. Darle a mitad de tiempo una pista si está trabado ("¿en qué función está el contador?"); que la necesite baja un escalón en la rúbrica, no lo desaprueba.

### Cómo detectar código no propio (sin ser hostil)
El objetivo no es "atrapar" a nadie: es verificar que aprendió. Tono de charla, nunca de interrogatorio.
- **Señales:** código idéntico a la referencia (mismos nombres, mismos comentarios), estilo muy distinto al de sus TPs, construcciones que no se vieron en el curso (`String`, `?:`, `struct`, punteros, `Serial.println`), comentarios en inglés o "demasiado perfectos".
- **Preguntas que lo revelan sin acusar:** "¿Por qué eligió resolverlo así?", "¿Qué probó antes que no funcionó?", "Si se cambia este nombre por otro, ¿qué más hay que cambiar?", "¿Qué hace esta línea que no vimos en clase?".
- **La modificación en vivo es la prueba definitiva:** quien escribió el código encuentra el lugar en segundos, aunque después dude en cómo hacerlo. Quien lo copió busca con la vista por todo el archivo.
- Si se confirma que no puede explicar su código: no discutir en el momento. Decir con calma *"No está pudiendo explicar su código, que es la condición para aprobar. Le propongo otra fecha: traiga un entrenador más simple, pero escrito por usted"*. Un entrenador con 4 modos simples y propios aprueba.
- Usar la referencia de la cátedra como base **está permitido** si la entendió y la adaptó: lo que no se acepta es no poder explicarla.

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| Al estar en un modo, también se ejecuta "un pedazo" del siguiente | Falta un `break` al final de un `case` |
| El entrenador arranca en un modo raro, o el semáforo arranca sin luces | La variable de estado no está inicializada (`Modo modo;` sin `= MODO_BINARIO`), o no se llama a `entrarModo()` en `setup()` |
| C no responde (o responde "a veces") en un modo | Hay un `delay` dentro de ese modo, o un `while` esperando un botón: el `loop()` no vuelve a leer C |
| Un botón "a veces no funciona" en un modo | El flanco se lee en dos lugares distintos (`seApreto(BOT_A)` en el `loop()` y otra vez dentro del modo): la primera llamada actualiza `apretadoAntes` y se come el evento |
| Al volver a un modo, arranca donde había quedado (o el semáforo aparece en el amarillo) | Las variables del modo no se reinician al entrar: falta su `case` en `entrarModo()` |
| El semáforo cambia de estado al instante, sin esperar | No se actualiza `inicioEstado` al cambiar de estado (usar siempre `cambiarEstado()`) |
| El monitor serie se inunda de texto | `printf` dentro de un modo sin condición: imprimir solo cuando algo cambia |
| `'modoContador' was not declared in this scope` | La función del modo está escrita **debajo** del `loop()` que la usa |
| `invalid conversion from 'int' to 'Modo'` | Falta el `(Modo)` al calcular el modo siguiente |
| El menú tiene 7 modos pero C recorre solo 6 (o 8) | `CANTIDAD_MODOS` no está último en el `enum`, o se usó un número a mano (`% 6`) |
| En COMPUERTAS no pasa nada al mantener A | Se usó `flancoA` (un instante) en vez de leer el **nivel** del botón |
