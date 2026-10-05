# Guion Docente — Clase 1

**Documento para el docente.**

---

## Objetivo del docente

Al final de las 3 hs, **todos** los alumnos:
- Tienen PlatformIO funcionando (o, si la PC no lo permite, Wokwi web con la placa de cátedra guardada).
- Corrieron el Blink y lo modificaron.
- Vieron al menos un error de compilación y supieron arreglarlo.

Es la primera vez que programan. **El objetivo emocional importa tanto como el técnico:** que salgan pensando "esto lo puedo hacer". No avanzar al bloque siguiente si hay alumnos trabados en el anterior.

---

## Preparación (1 día antes)

- [ ] Verificar que las PCs del laboratorio tengan VS Code + PlatformIO instalado y **ya compilado una vez** un proyecto para `esp32dev` y otro para `esp32c3` (la primera compilación descarga ~300 MB por placa; hacerlo en clase con 30 PCs a la vez satura la red).
- [ ] Probar que wokwi.com abre desde la red de la facultad (a veces hay proxy). Es el plan B para quien no tenga PlatformIO.
- [ ] Tener la placa de cátedra armada en una cuenta propia, con link público, por si alguien no puede pegar el `diagram.json`.
- [ ] Si se va a mostrar la placa real: llevar un ESP32 armado con el programa del TP ya cargado. Que lo vean funcionando al final "en serio" motiva mucho.
- [ ] Proyector con la letra del editor grande (Ctrl + `+` en el navegador).

---

## Bloque 1 — ¿Qué es programar? (20 min) | Pizarrón

### Mensaje central
*"Un programa es una receta: instrucciones en orden. El micro es un cocinero muy rápido y muy obediente, pero sin sentido común: hace exactamente lo que dice la receta, aunque esté mal."*

### Dinámica sugerida (5 min): "el robot"
Un voluntario hace de ESP32. El resto le dicta instrucciones para que "encienda la luz del aula y vuelva a su lugar". El robot obedece literalmente ("camine" → ¿cuántos pasos? ¿para dónde?). Sirve para mostrar que la computadora no interpreta: ejecuta.

### Puente con Sistemas Lógicos
Dibujar un LED con resistencia conectado a un pin. "En la teoría ponen un 1 o un 0 en la entrada de una compuerta. Hoy vamos a poner un 1 o un 0 en un pin, escribiendo una línea de texto."

---

## Bloque 2 — PlatformIO y Wokwi (20 min) | Hands-on

- Mostrar en el proyector: abrir una carpeta de proyecto, la barra azul (✓ compilar, → cargar, 🔌 monitor) y el selector de placa (`env:esp32dev` / `env:esp32c3`).
- Error típico de PlatformIO: abren la carpeta equivocada (la de la clase o la del repo) y aparece `Arduino.h: No such file`. Hay que abrir la carpeta **del proyecto**, la que tiene el `platformio.ini`.
- Error típico con el C3: compilan con `env:esp32dev` (el de defecto) y la carga falla. Revisar el selector de placa.
- Quien use Wokwi web: crear cuenta **antes** de empezar, porque sin cuenta no puede guardar. Error típico: pegan el `diagram.json` sin borrar el que estaba, o lo pegan en `sketch.ino`.

---

## Bloque 3 — Primer programa (30 min) | Hands-on guiado

- **Escribirlo en vivo en el proyector, letra por letra**, y que lo copien a la par. No pasar el código por chat: tipearlo los obliga a mirar los `;` y las llaves.
- Una vez que funciona, pedir que cambien el texto y le agreguen un segundo renglón.
- Experimento clave: sacar el `\n` y ver qué pasa.
- Experimento: poner un `Serial.printf` dentro de `loop()`. El monitor se inunda de texto. Pregunta: *"¿cuántas veces por segundo se está ejecutando loop()?"* (miles). Esto prepara el terreno para `delay`.

---

## Bloque 4 — Blink (40 min) | Hands-on

- Antes de correrlo, **pedir que predigan** qué va a pasar. Predecir antes de ejecutar es el hábito más importante que hay que formar.
- La pregunta del segundo `delay` borrado es clave: el LED se apaga y se vuelve a prender en microsegundos, así que el ojo lo ve encendido. Conecta con la idea de que `loop()` vuelve a empezar inmediatamente.
- Si hay placas: que lo carguen en la placa real con → . Con el C3, verificar que esté elegido `env:esp32c3`.

---

## Bloque 5 — Errores (20 min) | Hands-on

- Hacerlo como juego: "rompan el programa de 4 maneras distintas y anoten qué dice el error".
- Remarcar que el error **no es un fracaso**. El compilador es un ayudante que avisa antes de que el problema llegue al micro.
- Error típico real: escriben `Serial.Printf` o `digitalwrite`. Que aprendan a sospechar de las mayúsculas.

---

## Bloque 6 — Ejercicios (50 min)

- El ejercicio 3 (barrido) es largo y repetitivo **a propósito**. Cuando se quejen, decir: *"tienen razón, es horrible. En la clase 4 lo van a hacer en 5 líneas."* Genera la necesidad de los bucles.
- Los que terminan rápido arrancan con el TP.

---

## Cierre (5 min)

- Mostrar el TP funcionando en la placa real, si está.
- Recordar cómo se entrega: la carpeta del proyecto, **sin `.pio`**, en un ZIP con el nombre correcto.
- Adelanto de la Clase 2: *"hoy el micro solo repitió cosas. La semana que viene va a recordar números y a contar en binario."*

---

## Errores típicos de la clase

| Síntoma | Causa |
|---|---|
| El monitor serie no muestra nada | Falta `Serial.begin(115200);` |
| Texto todo pegado en un renglón | Falta `\n` |
| LED siempre apagado | Falta `pinMode(…, OUTPUT)`, o pin equivocado |
| LED siempre prendido | Falta el segundo `delay` |
| `was not declared in this scope` | Mayúscula/minúscula mal escrita |
| Código fuera de las llaves | Instrucciones escritas después de la `}` final de `loop()` |
