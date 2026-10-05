# Guion Docente — Clase 6

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Declaran, inicializan y recorren un arreglo sin salirse de los índices.
- Entienden la idea **"combinacional = tabla = arreglo"**: la entrada es el índice, la salida es el contenido.
- Tienen el display de 7 segmentos funcionando en Wokwi con la tabla `SEGMENTOS[16]`.

La clase tiene dos mitades: la primera es **C puro** (arreglos en general), la segunda es **Sistemas Lógicos** (tablas de búsqueda). No saltear la primera para llegar rápido al display: si no entienden el índice 0..N-1, el display no les va a funcionar y no van a saber por qué.

---

## Preparación (1 día antes)

- [ ] Probar el ejemplo 02 en Wokwi web con los archivos de `wokwi/` (ya trae el display). Tener un proyecto propio con `diagram_7seg.json` cargado, con link público (para los que no logren cambiar el diagrama).
- [ ] Llevar impresa o en el pizarrón la tabla de 7 segmentos con el dibujo del display y las letras `a`..`g`.
- [ ] Revisar qué está viendo la teoría esta semana (decodificadores, MUX, ROM) para usar los mismos ejemplos y nombres.
- [ ] Si hay placa real: cargarle el ejemplo 02 y mostrar cómo se "lee" un dígito en los 8 LEDs.

---

## Bloque 1 — El problema de las muchas variables (10 min) | Pizarrón

### Mensaje central
*"Un arreglo es una fila de cajitas numeradas, todas del mismo tipo, con un solo nombre. El número de la cajita es el índice."*

- Escribir en el pizarrón `tempLunes`, `tempMartes`... y preguntar cómo calcularían el promedio de un año. Que lleguen ellos a "necesito una variable con muchos valores".
- Recordar que **ya usaron uno**: `LEDS[8]` en la Clase 4. Hoy lo entienden en serio.

---

## Bloque 2 — Arreglos (35 min) | Pizarrón + hands-on

- Dibujar las cajitas con los índices **arriba** y los valores **adentro**. Insistir en la diferencia entre índice y valor: `temperaturas[3]` **no** es 3, es 38.
- Dinámica rápida (5 min): escribir `int v[5] = {10, 20, 30, 40, 50};` y preguntar en voz alta: ¿cuánto vale `v[0]`? ¿`v[4]`? ¿`v[5]`? ¿`v[2] + v[3]`? ¿`v[v[0] / 10]`? (la última es para los que van rápido: `v[1]` = 20).
- Por qué desde 0: conectar con `L0` y el bit 0. "El índice es el **desplazamiento** desde el principio". Si preguntan, contar que en C el nombre del arreglo es la dirección de memoria del primer elemento, pero **no** entrar en punteros.
- Hacer el ejemplo 01 en vivo. Pedir que **predigan** el promedio antes de correrlo. Que agreguen el mínimo solos (son 6 líneas copiando el máximo).
- **Salirse del arreglo:** mostrarlo en vivo. Descomentar la línea `temperaturas[7]` del ejemplo 01: imprime basura (o 0, depende). Remarcar que **no hubo error**. Esa es la parte peligrosa. Frase para el pizarrón: *"C confía en el programador. Demasiado."*
- `const`: mostrar que `LEDS[0] = 5;` no compila. Es el compilador cuidándolos.

---

## Bloque 3 — Combinacional = tabla (15 min) | Pizarrón (bloque clave)

### Dinámica: "la tabla de verdad se da vuelta"
1. Dibujar a la izquierda una tabla de verdad de un decodificador 2 → 4 como en la teoría.
2. Al lado, dibujar las cajitas de un arreglo de 4 elementos.
3. Unir con flechas: la columna "entrada" (en decimal) son los **índices**; las columnas de salida (como un número) son los **contenidos**.
4. Escribir grande: `salida = TABLA[entrada];`
5. Preguntar: *"¿y si la tabla tuviera 256 filas?"* — "Igual: una línea de código y un arreglo de 256."

### Puente con Sistemas Lógicos
Dibujar una ROM como caja negra: **líneas de dirección** a la izquierda (A0, A1...), **líneas de datos** a la derecha (D0..D7). "La dirección es el índice; el dato es el contenido. Una ROM **es** un arreglo `const` hecho de silicio." Mencionar las FPGA y sus LUT de 4 o 6 entradas: es la misma idea a escala industrial.

---

## Bloque 4 — Decodificador 2 → 4 (20 min) | Hands-on

- Primero que lo escriban **con `if`**: ya saben hacerlo de la Clase 3. Después mostrar la tabla. Que sientan el ahorro.
- `b * 2 + a` con `bool`: aclarar que un `bool` en una cuenta vale 0 o 1. Conectar con la conversión binario → decimal de la Clase 2 ("cada bit por su peso").
- Para los que vienen bien de la Clase 5: `(b << 1) | a` es lo mismo, y para el decodificador alcanza con `1 << entrada` (sin tabla). Es una buena discusión: **a veces hay fórmula, a veces conviene tabla**.
- El ejemplo 03 imprime la comparación de las dos formas al arrancar. Hábito a formar: **verificar una implementación contra otra**.

---

## Bloque 5 — 7 segmentos (30 min) | Hands-on

- Cambiar el diagrama es el paso donde más se traban. Error típico en Wokwi web: pegan el `diagram_7seg.json` **debajo** del anterior, o lo pegan en `sketch.ino`. Recorrer los bancos.
- Recordarles trabajar sobre una **copia** del proyecto (en PlatformIO, copiar la carpeta; en Wokwi web, **Save a copy**): si pisan el proyecto de la Clase 5, pierden el TP.
- Armar **en el pizarrón, entre todos**, el patrón del 2 y del 4 bit por bit (dibujar el display, marcar segmentos, escribir `dp g f e d c b a` arriba de los bits). Después dejarlos armar solos el 7 y el 9 y comparar con la tabla.
- Error clásico: escriben el byte en el orden `a b c d e f g` de izquierda a derecha (al revés). El display muestra cualquier cosa. Recordar que **el bit 0 está a la derecha** en `0b...`.
- Aclarar que en la placa real **no hay display**: el patrón se ve en los LEDs. Si hay placa, mostrarlo.

### Puente con Sistemas Lógicos
El 7447/7448: en la teoría se hace con 7 mapas de Karnaugh. Preguntar: *"¿por qué en la teoría simplificamos y acá no?"*. Respuesta: con compuertas, simplificar ahorra chips; en una memoria, la tabla ocupa lo mismo sea cual sea la función.

---

## Bloque 6 — Multiplexor (15 min) | Hands-on

- Que primero predigan qué va a hacer L7 con cada combinación de A y B mirando los LEDs L0..L3.
- Que cambien el arreglo `datos` y vuelvan a probar.
- Diferencia clave con el decodificador: en el deco la tabla es fija y el índice elige la salida; en el MUX los **datos** son entradas y el índice elige **cuál pasa**. "Indexar un arreglo es multiplexar."
- Comentario para los curiosos: `selAnterior` sirve para imprimir solo cuando cambia algo. Es una variable que **recuerda** el pasado: decirles que es la idea central de la clase que viene.

---

## Bloque 7 — Función de 3 variables con `F[8]` (15 min) | Hands-on

- Que armen el programa a partir del fragmento del README (es corto: pinMode de 3 botones y L0).
- Preguntar qué función es la tabla del README antes de decirlo. Que la prueben con los 3 botones.
- Después, **cambiar la función sin tocar el código**: solo el arreglo. Es la demostración más fuerte de la clase. Si la teoría está viendo una función en particular, usar esa.
- Pregunta para el pizarrón: ¿cuántas funciones distintas de 3 variables hay? → 2⁸ = 256. ¿Y de 4 variables? → 2¹⁶ = 65536. "Todas caben en el mismo programa, cambiando solo el arreglo."

---

## Bloque 8 — Ejercicios (40 min)

- El 1 (BCD con E) es rápido y refuerza que **la decisión está en la tabla** (no usar `if` para elegir el patrón).
- El 2 (codificador de prioridad) es el más "de programación": buscar con un `for` **hacia atrás** y usar `return` dentro del `for`. Muchos van a recorrer de 0 a 7 y quedarse con el último encontrado: también funciona, pero mostrar que buscar desde arriba y salir con `return` es más claro.
- El 3 (Gray) es bueno para conectar con la teoría (mapas de Karnaugh, encoders rotativos). La comparación tabla vs. fórmula es la misma discusión del bloque 4.
- El 4 (comparador) obliga a pensar el índice con dos números de 2 bits: `y * 4 + x`. Si se traban, dibujar la tabla como una matriz de 4 × 4 (filas Y, columnas X).
- Los que terminan arrancan con el TP.

---

## Cierre (5 min)

- Mostrar el TP funcionando: A y B cambian el dígito del display.
- Recordar la entrega: `SL2026 - Apellido - Clase 6`, **con el diagrama del display**.
- Adelanto de la Clase 7: *"Hoy la salida dependía solo de lo que apretaban ahora. La semana que viene el micro va a **recordar** lo que pasó antes: flip-flops y contadores. Y por fin vamos a arreglar ese `delay(250)` de los botones que les prometimos."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| Número basura, o una variable que cambia "sola" | **Índice fuera del arreglo**: `for (int i = 0; i <= N; i++)` en vez de `i < N`, o `valor` que llega a 16 en `SEGMENTOS[16]` |
| El primer o el último elemento "se pierde" | **Contar desde 1**: `for (int i = 1; i <= N; i++)` saltea el `[0]` y lee el `[N]` que no existe |
| `too many initializers` | Pusieron más valores entre llaves que el tamaño declarado |
| Los elementos que faltan valen 0 | Pusieron **menos** valores que el tamaño: C completa con 0 sin avisar |
| `assignment of read-only location` | Intentaron cambiar un elemento de un arreglo `const` |
| El display muestra dibujos raros | Escribieron los bits en orden `abcdefg` de izquierda a derecha; en `0b...` el bit 0 (`a`) va a la **derecha** |
| El display no enciende nada | No cambiaron el `diagram.json`, o el TP muestra el `valor` y no `SEGMENTOS[valor]` |
| El display muestra la entrada en binario, no el dígito | Hicieron `mostrarByte(valor)` en vez de `mostrarByte(SEGMENTOS[valor])` |
| El MUX/decodificador responde "al revés" | Armaron el índice como `a * 2 + b`: el pulsador que pesa 2 es B |
| El contador del TP pasa de 15 a basura | Falta la vuelta `if (valor > 15) valor = 0;` (y lo mismo para `valor < 0`) |
