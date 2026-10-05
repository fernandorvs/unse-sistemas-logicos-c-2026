# PlatformIO

Todos los programas del curso (ejemplos y soluciones) son **proyectos PlatformIO**, igual que en Sistemas Microprogramables. Se trabaja en **VS Code + PlatformIO**.

Ventajas de PlatformIO:

- Se trabaja sin conexión a internet (después de la instalación).
- Se carga el programa a la placa real con un clic.
- El editor marca errores mientras se escribe y autocompleta nombres.
- Es el entorno que se usa en la industria y en materias posteriores.

La carpeta [`../wokwi/`](../wokwi/) tiene cada programa como `sketch.ino` con **el mismo código**, para pegarlo en [Wokwi](https://wokwi.com) desde el navegador cuando no se tiene la PC con PlatformIO (ver [`../placa/README.md`](../placa/README.md#en-wokwi)).

---

## Cómo es un proyecto

```
02_blink/
├── platformio.ini      ← configuración: las dos placas (env:esp32dev y env:esp32c3), monitor a 115200
├── src/
│   └── main.cpp        ← el programa
└── include/
    └── placa_c3.h      ← traduce los pines del DevKit a los del C3 (no se toca)
```

La carpeta [`plantilla/`](plantilla/) es un proyecto con esa misma forma y un Blink, para empezar los TPs.

---

## Instalación (una sola vez)

1. Instalar [VS Code](https://code.visualstudio.com/).
2. En VS Code, abrir *Extensions* (`Ctrl+Shift+X`), buscar **PlatformIO IDE** e instalarla. La primera vez tarda varios minutos en descargar las herramientas.
3. Descargar este repositorio (botón verde **Code → Download ZIP** en GitHub, o `git clone`).

La primera compilación de cada placa descarga el soporte para ESP32 (unos 300 MB) y tarda unos minutos. Las siguientes tardan segundos.

---

## Abrir un ejemplo

1. En VS Code: *File → Open Folder…* y elegir **la carpeta del proyecto**, la que tiene el `platformio.ini`. Por ejemplo `Clases/Clase 1 - Hola ESP32/ejemplos/02_blink`.
2. Elegir la placa (ver abajo).
3. Compilar y cargar.

Los botones de PlatformIO están en la barra azul de abajo:

| Botón | Acción |
|---|---|
| ✓ | Compilar (*Build*) |
| → | Compilar y cargar a la placa (*Upload*) |
| 🔌 | Abrir el monitor serie |
| 🗑 | Limpiar (*Clean*) |

---

## Empezar un proyecto nuevo (TP)

1. Copiar la carpeta [`plantilla/`](plantilla/) completa y renombrar la copia, por ejemplo `SL2026-Apellido-Clase3`.
2. Abrir la copia en VS Code (*File → Open Folder…*).
3. Escribir el programa en `src/main.cpp`. La primera línea tiene que ser siempre:
   ```c
   #include <Arduino.h>
   ```
4. Compilar (✓).

Para entregarlo: **borrar la carpeta `.pio`** (pesa cientos de MB y se regenera sola al compilar) y comprimir la carpeta del proyecto en un ZIP. Ver [`../EVALUACION.md`](../EVALUACION.md#cómo-se-entrega-un-tp).

### El orden de las funciones

En un archivo `.cpp`, una función debe estar **escrita antes de usarse** (o declarada con su prototipo arriba). Wokwi y Arduino IDE ordenan eso automáticamente; PlatformIO no.

Todos los programas del curso respetan ese orden: primero las funciones, al final `setup()` y `loop()`. Si el compilador muestra
`'mostrarByte' was not declared in this scope`, la solución es mover esa función más arriba o agregar su prototipo:

```c
void mostrarByte(uint8_t valor);   // prototipo: "esta función existe, está más abajo"
```

(Los prototipos se explican en la Clase 4.)

---

## Dos placas: ESP32 DevKit y ESP32-C3 Super Mini

Todos los proyectos compilan para las dos placas que se usan en la facultad (las mismas de Sistemas Microprogramables). Se elige en la barra azul de abajo, en el selector de entorno:

| Entorno | Placa |
|---|---|
| `env:esp32dev` (por defecto) | ESP32 DevKit, la placa de cátedra |
| `env:esp32c3` | ESP32-C3 Super Mini |

**El código es el mismo para las dos placas.** Está escrito con los pines del DevKit; en el entorno `esp32c3`, el archivo `include/placa_c3.h` traduce cada pin al que corresponde en el C3. Tabla de pines y conexiones del C3 en [`../placa/README.md`](../placa/README.md#con-un-esp32-c3-super-mini).

En el C3 el monitor serie va por el USB de la placa. Al arrancar, el programa espera hasta 3 segundos a que se abra el monitor, para no perder lo que imprime `setup()`.

---

## Simular sin placa

Los proyectos PlatformIO son para cargar en la placa real. Para simular, se usa Wokwi en el navegador con los mismos programas de [`../wokwi/`](../wokwi/) (`sketch.ino` + `diagram.json`; ver [`../wokwi/README.md`](../wokwi/README.md)).

---

## Cargar en la placa real

1. Conectar el ESP32 por USB (con un cable de **datos**, no solo de carga).
2. Elegir el entorno de la placa (`env:esp32dev` o `env:esp32c3`).
3. Botón → (*Upload*). PlatformIO detecta el puerto solo.
4. Botón 🔌 para ver el monitor serie.

Problemas comunes:

| Síntoma | Solución |
|---|---|
| No encuentra el puerto | Probar otro cable USB. Instalar el driver CH340 o CP2102 según el chip de la placa. |
| Se queda en `Connecting.....` | Mantener apretado el botón **BOOT** de la placa hasta que empiece a cargar. |
| C3: no carga o no aparece el puerto | Mantener apretado **BOOT**, apretar y soltar **RST**, soltar **BOOT**, y volver a cargar. |
| C3: carga pero no hace nada, o el monitor no muestra nada | Verificar que esté elegido el entorno `env:esp32c3`, no `env:esp32dev`. |
| El monitor muestra caracteres raros | Verificar que `monitor_speed = 115200` esté en `platformio.ini`. |
| `Arduino.h: No such file` | Se abrió la carpeta equivocada en VS Code: debe ser la del proyecto, la que tiene el `platformio.ini`. |

---

## Para el docente: compilar todo el curso

`compilar_todo.sh` compila, con el compilador real del ESP32, todos los proyectos de las clases (ejemplos y soluciones), para las dos placas. Además verifica que en cada proyecto:

- `platformio.ini` e `include/placa_c3.h` sean iguales a los de `plantilla/`,
- `wokwi/…/sketch.ino` sea idéntico a su `src/main.cpp`.

Sirve para verificar que nada se rompió después de editar el material. **Si se edita un ejemplo, cambiar `src/main.cpp` y copiarlo encima de su `wokwi/…/sketch.ino`** (o al revés).

```bash
cd platformio
./compilar_todo.sh            # las dos placas
./compilar_todo.sh esp32c3    # una sola
```

Requiere PlatformIO Core en la terminal (`pip install platformio`). Funciona en Linux, macOS y en Windows con Git Bash.
