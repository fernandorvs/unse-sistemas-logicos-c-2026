# Placa de cátedra

Todas las clases usan el mismo circuito: **ESP32 + 8 LEDs + 3 pulsadores**. Sirve un **ESP32 DevKit** o un **ESP32-C3 Super Mini**.
Se arma una vez (en Wokwi o en protoboard) y sirve para todo el curso.

---

## Pinout

| Elemento | GPIO | Notas |
|---|---|---|
| `L0` (bit 0, el de menos peso) | 4 | LED rojo |
| `L1` | 16 | LED rojo |
| `L2` | 17 | LED rojo |
| `L3` | 18 | LED rojo |
| `L4` | 19 | LED verde |
| `L5` | 21 | LED verde |
| `L6` | 22 | LED verde |
| `L7` (bit 7, el de más peso) | 23 | LED verde |
| Pulsador **A** | 32 | a GND, usa `INPUT_PULLUP` |
| Pulsador **B** | 33 | a GND, usa `INPUT_PULLUP` |
| Pulsador **C** | 25 | a GND, usa `INPUT_PULLUP` |

Los LEDs están ordenados como se escribe un número binario: **`L7` a la izquierda, `L0` a la derecha**.
Los primeros cuatro (`L0`–`L3`) son rojos y los otros cuatro (`L4`–`L7`) son verdes. Así se distinguen los dos *nibbles* (grupos de 4 bits) de un byte.

En el código, el pinout se escribe siempre igual:

```c
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};  // L0 ... L7
const int BOTON_A = 32;
const int BOTON_B = 33;
const int BOTON_C = 25;
```

(Las primeras clases usan los números de pin sueltos. El arreglo `LEDS[8]` aparece en la Clase 4.)

### Con un ESP32-C3 Super Mini

El C3 no tiene los GPIO 16 a 33, así que los LEDs y pulsadores van en estos pines:

| Elemento | En el código (DevKit) | Pin real en el C3 |
|---|---|---|
| `L0` | 4 | 0 |
| `L1` | 16 | 1 |
| `L2` | 17 | 3 |
| `L3` | 18 | 4 |
| `L4` | 19 | 5 |
| `L5` | 21 | 6 |
| `L6` | 22 | 7 |
| `L7` | 23 | 10 |
| Pulsador **A** | 32 | 20 |
| Pulsador **B** | 33 | 21 |
| Pulsador **C** | 25 | 9 (es el botón **BOOT** de la placa; no hace falta cablearlo) |

**El código no cambia**: se sigue escribiendo con los pines del DevKit. Al compilar con el entorno `env:esp32c3` de PlatformIO, el archivo `include/placa_c3.h` de cada proyecto traduce cada pin al del C3.

Se dejan libres el GPIO 8 (LED azul de la placa) y los GPIO 18 y 19 (USB). No apretar el pulsador C (BOOT) mientras se enciende o se resetea la placa: la deja en modo de carga.

---

## En Wokwi

### Crear el proyecto la primera vez

1. Entrar a [wokwi.com](https://wokwi.com) e iniciar sesión (con Google o GitHub). Así se pueden guardar los proyectos.
2. Abrir **[wokwi.com/projects/new/esp32](https://wokwi.com/projects/new/esp32)**.
3. Arriba a la izquierda aparecen dos pestañas: `sketch.ino` (el programa) y `diagram.json` (el circuito).
4. Abrir `diagram.json`, **borrar todo** y pegar el contenido de [`diagram.json`](diagram.json) de esta carpeta.
5. Apretar **Save**. La placa de cátedra queda lista.

Para cada clase nueva: abrir el proyecto, usar **Save a copy** y renombrarlo `SL2026 - Apellido - Clase N`.

### Usar Wokwi

| Acción | Cómo |
|---|---|
| Correr el programa | Botón verde ▶ (compila y arranca la simulación) |
| Detener | Botón ⏹ |
| Ver el monitor serie | Aparece solo, abajo del circuito, cuando el programa imprime algo |
| Apretar un pulsador | Clic sobre el pulsador (mantener apretado = mantener el clic) |
| Formatear el código | Clic derecho en el editor → *Format Document* |

### Variante con display de 7 segmentos (Clase 6)

[`diagram_7seg.json`](diagram_7seg.json) es la misma placa más un display de 7 segmentos de cátodo común, conectado **a los mismos pines que los LEDs**:

| Segmento | a | b | c | d | e | f | g | dp |
|---|---|---|---|---|---|---|---|---|
| LED | L0 | L1 | L2 | L3 | L4 | L5 | L6 | L7 |
| GPIO | 4 | 16 | 17 | 18 | 19 | 21 | 22 | 23 |

Al encender un byte, se ve a la vez en los LEDs y en el display. En la placa real (que no tiene display) se ve en los LEDs.

---

## En la placa real (protoboard)

### Materiales

- 1 × ESP32 DevKit (30 o 38 pines) o ESP32-C3 Super Mini + cable USB de datos
- 8 × LED (4 rojos y 4 verdes, o los que haya)
- 8 × resistencia de 220 Ω a 330 Ω
- 3 × pulsador táctil de 4 patas
- Protoboard + cables

### Conexiones

**Cada LED:**

```
GPIO ──[ 220 Ω ]──►|── GND
                 ánodo  cátodo
              (pata larga)(pata corta, lado plano)
```

**Cada pulsador** (no lleva resistencia, usamos la *pull-up* interna del ESP32):

```
GPIO ──┤ pulsador ├── GND
```

Con `INPUT_PULLUP`, el pin lee `HIGH` (1) cuando el pulsador está **suelto** y `LOW` (0) cuando está **apretado**. Esto se explica en la Clase 3.

> ⚠️ Atención: los pulsadores de 4 patas tienen las patas unidas de a pares. Si el botón parece "siempre apretado", giralo 90° en la protoboard.

### Cargar el programa

Con **PlatformIO** (la forma del curso, sirve para DevKit y C3): abrir la carpeta del proyecto en VS Code, elegir la placa y apretar → . Instructivo completo en [`../platformio/README.md`](../platformio/README.md).

### Alternativa: Arduino IDE (solo DevKit)

Con Arduino IDE no se traducen los pines del C3, así que sirve solo para el DevKit.

1. Instalar [Arduino IDE 2](https://www.arduino.cc/en/software).
2. *File → Preferences → Additional boards manager URLs*: pegar
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. *Tools → Board → Boards Manager*: buscar **esp32** (de Espressif) e instalar.
4. *Tools → Board*: elegir **ESP32 Dev Module**. *Tools → Port*: elegir el puerto COM o `/dev/ttyUSB…` que aparece al conectar la placa.
5. Copiar el contenido de `sketch.ino` de Wokwi, pegarlo en Arduino IDE y apretar **Upload** (→).
6. Abrir el monitor serie (lupa arriba a la derecha) y elegir **115200 baud**.

Si no aparece el puerto: probar otro cable USB (muchos cables solo cargan, no transmiten datos) o instalar el driver CH340 o CP2102 según el chip de la placa.
Si se queda en `Connecting.....`: mantener apretado el botón **BOOT** de la placa hasta que empiece a subir.
