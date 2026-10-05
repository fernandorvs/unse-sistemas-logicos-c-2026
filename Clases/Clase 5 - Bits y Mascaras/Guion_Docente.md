# Guion Docente — Clase 5

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Escriben un byte en `0b` y `0x` y lo ven en los LEDs con `mostrarByte`.
- Distinguen `&` de `&&` y pueden dar un ejemplo donde dan distinto.
- Encienden, apagan, invierten y leen un bit con máscaras.
- Tienen el registro del TP respondiendo al menos a uno de los tres botones.

Esta clase es **muy de pizarrón**: los operadores bit a bit se entienden haciendo las cuentas a mano, columna por columna, antes de tocar el teclado. La ventaja es que en la teoría ya hicieron tablas de compuertas: hoy es aplicarlas 8 veces en paralelo.

---

## Preparación (1 día antes)

- [ ] Preparar 3 o 4 pares de bytes para hacer `&`, `|`, `^` a mano en el pizarrón (por ejemplo `0b11001100` y `0b10101010`, que dan las cuatro combinaciones de bits en las columnas).
- [ ] Tener el TP de referencia corriendo en Wokwi para mostrar al final.
- [ ] Si hay placa real: cargar el ejemplo 04 (contador de 8 bits). Ver contar en binario con LEDs reales a 100 ms impresiona.
- [ ] Tener a mano un integrado 74LS08 o su hoja de datos, para mostrar "4 AND en un chip" como analogía de `&`.

---

## Bloque 1 — Un byte = 8 LEDs (25 min) | Pizarrón + hands-on

### Mensaje central
*"Hasta ahora sacábamos los bits dividiendo. Pero el número ya ESTÁ en binario adentro del micro. Hoy aprendemos a mirarlo directamente."*

### Dinámica de pizarrón
Dibujar 8 casilleros con los pesos arriba (128 … 1) y los nombres de LED abajo (L7 … L0), con los 4 de la izquierda en verde y los 4 de la derecha en rojo (como la placa). Escribir `0b10110001`, pasarlo a hexa agrupando de a 4 y a decimal sumando pesos. Pedir a un alumno que diga qué LEDs se encienden.

### Hands-on
- Ejemplo 01: que cambien los patrones por sus propios bytes (su DNI módulo 256, su edad, etc.) y **predigan** qué LEDs se encienden antes de correrlo.
- `mostrarByte` e `imprimirBinario`: por ahora se usan como "caja negra". Decir explícitamente: *"en 40 minutos van a entender cada carácter de esta línea"*. No intentar explicar `(valor >> i) & 1` todavía.

---

## Bloque 2 — Operadores bit a bit (35 min) | Pizarrón

### Mensaje central
*"`&` es una AND por cada columna. Ocho compuertas trabajando a la vez, sin hablarse entre ellas."*

### Dinámica de pizarrón: "las 8 compuertas"
Escribir `x` y `y` uno arriba del otro, alineados, y resolver `x & y`, `x | y`, `x ^ y` y `~x` **columna por columna**, con distintos alumnos pasando a hacer cada uno. Después dibujar el 74LS08 y unir con flechas: bit 0 de x y bit 0 de y → compuerta → bit 0 del resultado.

### Puente con Sistemas Lógicos
Volver a la tabla de compuertas de la Clase 3 y agregarle una columna "bit a bit". Que ellos la completen: AND → `&`, OR → `|`, NOT → `~`, XOR → `^`, NAND → `~(x & y)`.

### El error clásico (10 min)
Escribir en el pizarrón `5 & 2` y `5 && 2` y pedir que voten cuánto da cada uno **antes** de correr el ejemplo 02. Casi todos se equivocan en el `5 & 2`. Remarcar la regla: **condiciones con `&&`, bits con `&`**.

- Mostrar también que `~x` impreso directo con `%X` sale `FFFFFF33`. Explicar en una frase que C hace la cuenta en `int` (32 bits) y que por eso guardamos el resultado en `uint8_t`. No profundizar en promoción de tipos.

---

## Bloque 3 — Desplazamientos (25 min) | Pizarrón + hands-on

### Mensaje central
*"Correr a la izquierda es multiplicar por 2, igual que agregar un cero en decimal es multiplicar por 10."*

### Dinámica de pizarrón
Escribir `22` en binario y correrlo de a un lugar, anotando el decimal al lado: 22, 44, 88, 176, 96 (¡se cayó un bit!). Preguntar por qué el último no es 352. (No entra en 8 bits: desborde, Clase 2.)

### Puente con Sistemas Lógicos
El shift es un **registro de desplazamiento**: cada bit pasa a la celda vecina. Si ya vieron flip-flops D en la teoría, dibujar 4 en cadena; si no, dejarlo como adelanto.

### Hands-on
- Ejemplo 03 (barrido con `<<`). Pregunta: *"¿qué hay que cambiar para que vaya al revés?"* (arrancar en `0b10000000` y usar `>>`).
- Ahora sí, explicar `(valor >> i) & 1` con el dibujo de la guía. Cerrar el círculo con `mostrarByte`.
- Rotación: hacer en el pizarrón `(x << 1)`, `(x >> 7)` y el OR, con un byte que tenga el bit 7 en 1.

### Errores a anticipar
- Precedencia: escribir `if (x & 1 == 0)` en el pizarrón y preguntar qué hace. Mostrar que `==` se evalúa primero. Regla del curso: **siempre paréntesis** en operaciones de bits.
- `1 << n + 1`: se lee como `1 << (n + 1)`.

---

## Bloque 4 — Máscaras (35 min) | Pizarrón + hands-on

### Mensaje central
*"Una máscara es un byte que dice 'a estos bits los toco, a estos no'. OR para encender, AND con el complemento para apagar, XOR para invertir."*

### Dinámica de pizarrón
Armar en el pizarrón la tabla de las 4 operaciones (leer, poner en 1, poner en 0, invertir) y para cada una hacer un ejemplo a mano con `x = 0b10110001` y `n = 4`. La de "poner en 0" es la difícil: dibujar los tres pasos (`1 << 4`, `~(1 << 4)`, `x & máscara`).

### Puente con Sistemas Lógicos
Escribir al lado de cada operación el teorema de Boole que la justifica: **A + 1 = 1**, **A + 0 = A**, **A · 0 = 0**, **A · 1 = A**, **A ⊕ 1 = A'**, **A ⊕ 0 = A**. *"Las máscaras no son un truco de programadores: son los teoremas que ven en la teoría."*

### Hands-on
- Partiendo del ejemplo 01, que prendan L4 con `|=`, lo apaguen con `&= ~`, y lo hagan titilar con `^=` dentro de `loop()`.
- Ejemplo 04 (contador): comentar la trampa del `for` con `uint8_t` y `<= 255`.

---

## Bloque 5 — Pulsadores provisorios (10 min) | Proyector

- Mostrar el patrón `if (a) { acción; delay(250); }` y **después** mostrar qué pasa sin el `delay` (el registro cambia miles de veces).
- Dejar claro que es provisorio: *"esto es una curita. En la Clase 7 lo hacemos bien, con flancos."* Si alguien pregunta cómo detectar "el momento en que se aprieta", felicitarlo y decirle que es exactamente el tema de la Clase 7.

---

## Bloque 6 — Ejercicios (50 min)

- El Ejercicio 1 (paridad) conecta bien con la teoría: si ya vieron códigos detectores de error, mencionarlo; si no, contar en un minuto para qué sirve el bit de paridad.
- En el Ejercicio 2 (auto fantástico) el error típico es que los LEDs de los extremos queden el doble de tiempo, o que el 1 se "caiga" y quede todo apagado. Que lo depuren imprimiendo el byte con `imprimirBinario`.
- El Ejercicio 3 (complemento a 2) es **breve**: solo que vean aparecer el negativo. Si en la teoría todavía no se vio, dejarlo como opcional.
- Los que terminan rápido arrancan con el TP.

---

## Cierre (5 min)

- Mostrar el TP funcionando. Preguntar: *"si aprieto A ocho veces, ¿qué valor queda?"* (el mismo: rotar 8 veces un byte da una vuelta completa). *"¿Y B dos veces?"* (el mismo: NOT de NOT = involución).
- Recordar la entrega (`SL2026 - Apellido - Clase 5`).
- Adelanto de la Clase 6: *"hoy guardamos un byte. La semana que viene guardamos una lista de bytes, y con eso armamos un decodificador de 7 segmentos."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| El `if` entra cuando no debería (o al revés) | `&` en lugar de `&&` (o `\|` en lugar de `\|\|`) en una condición |
| `if (x & 1 == 0)` nunca entra | Precedencia: `==` va antes que `&`. Va `if ((x & 1) == 0)` |
| El shift da un número raro | Falta paréntesis: `1 << n + 1` es `1 << (n + 1)` |
| `~x` imprime `FFFFFF..` | `~` trabaja en `int`; guardar el resultado en `uint8_t` antes de imprimir |
| Apagar un bit apaga todos | Escribieron `x &= (1 << n)` en lugar de `x &= ~(1 << n)` |
| La rotación pierde el bit 7 | Hicieron solo `x << 1`, sin el `\| (x >> 7)` |
| El binario impreso sale "espejado" respecto de los LEDs | `imprimirBinario` recorre de 0 a 7 en lugar de 7 a 0 |
| El `for` de 0 a 255 nunca termina | Variable `uint8_t` con `<= 255`; usar `int` |
| Un LED de más hace cosas raras | `for` con `i <= 8`: se sale del arreglo `LEDS` |
| Al apretar un botón el valor salta muchas veces | Falta el `delay(250)` del patrón provisorio |
| El `for` se ejecuta una sola vez | `;` después del paréntesis del `for` |
| `rotarIzquierda` devuelve cualquier cosa | Falta el `return` |
