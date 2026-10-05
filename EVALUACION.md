# Evaluación y Régimen

## Filosofía

Se aprende a programar **programando**. No hay parciales escritos: cada clase produce un avance del *Entrenador Lógico* y eso es lo que se evalúa. Al final del módulo se defiende el proyecto integrado.

---

## Qué se entrega cada semana

Cada clase tiene:

- **Ejercicios de clase** (se hacen en el aula, con ayuda): no se entregan, sirven para practicar.
- **TP de la clase** (se marca con ⭐ en el README de cada clase): se entrega antes de la clase siguiente.

### Cómo se entrega un TP

1. Hacer el TP como **proyecto PlatformIO**: copiar la carpeta [`platformio/plantilla/`](platformio/plantilla/) y renombrarla `SL2026-Apellido-ClaseN`. Ver el [instructivo](platformio/README.md#empezar-un-proyecto-nuevo-tp).
2. Probarlo en la placa (DevKit o C3) o en el simulador, pegando el código en [wokwi.com](https://wokwi.com).
3. **Borrar la carpeta `.pio`** del proyecto (pesa cientos de MB y se regenera al compilar).
4. Comprimir la carpeta en un **ZIP** con el mismo nombre (`SL2026-Apellido-ClaseN.zip`) y subirlo al aula virtual (o donde indique la cátedra).

Opcional: quien quiera aprender Git puede además subir su código a un repositorio personal de GitHub. No es obligatorio.

### Reglas de un TP aceptado

- **Compila** en PlatformIO **y funciona** (en la placa o en el simulador).
- Tiene un **comentario al principio** con nombre y apellido, la clase y qué hace el programa.
- Los nombres de variables y funciones **dicen lo que hacen** (`contador`, `estadoBoton`; no `x`, `aaa`).
- Está **indentado** (sangría correcta). En VS Code o en Wokwi: clic derecho → *Format Document*, o `Shift+Alt+F`.

---

## Régimen de regularidad

- **Asistencia mínima:** 75% de las clases (6 de 8).
- **TPs entregados:** al menos 6 de los 7 TPs (clases 1 a 7).

---

## Aprobación del módulo

### 1. Entrenador lógico funcionando (Clase 8)

Debe correr en la placa (DevKit o C3) o en el simulador y tener como mínimo:

- Un **menú** que se recorre con los pulsadores.
- Al menos **4 modos** de los vistos en el curso (binario, compuertas, tabla de verdad, bits, 7 segmentos, contador, FSM).
- Código organizado en **funciones**, sin bloques copiados y pegados.
- Uso de una **máquina de estados** con `enum` y `switch`.

### 2. Defensa oral (10 minutos)

- Demo del entrenador funcionando (3 min).
- Explicación de una parte del código que elige el docente (4 min).
- Una **modificación en vivo** que pide el docente, por ejemplo "que el contador cuente para atrás" o "agregar la compuerta NOR" (3 min).

La modificación en vivo es la prueba de que el código es propio. No hace falta que salga perfecta: se evalúa que el alumno sepa **dónde** y **cómo** modificarlo.

---

## Criterios de evaluación

| Criterio | Peso | Qué se mira |
|---|---|---|
| TPs semanales | 30% | Entregados en fecha y funcionando |
| Entrenador funcionando | 30% | Cumple los mínimos, no se cuelga, responde a los pulsadores |
| Calidad del código | 15% | Funciones, nombres claros, indentación, comentarios útiles |
| Defensa oral | 25% | Explica su código y hace la modificación en vivo |

---

## Bonus opcional

- Pasar el entrenador a la **placa real** y mostrarlo funcionando.
- Agregar un modo extra: juego de "Simón dice", calculadora binaria (suma de dos números de 4 bits), dado electrónico.
- Usar un **display de 7 segmentos real** si se consigue uno.
