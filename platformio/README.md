# Versión PlatformIO

Todo el curso funciona en **Wokwi** desde el navegador, y eso alcanza. Esta carpeta es para quien prefiera trabajar en la PC con **VS Code + PlatformIO**, el mismo entorno que se usa en Sistemas Microprogramables.

Ventajas de PlatformIO:

- Se trabaja sin conexión a internet (después de la instalación).
- Se carga el programa a la placa real con un clic.
- El editor marca errores mientras se escribe y autocompleta nombres.
- Es el entorno que se usa en la industria y en materias posteriores.

---

## Contenido de esta carpeta

```
platformio/
├── platformio.ini     ← configuración: placa ESP32 DevKit, framework Arduino, monitor a 115200
├── src/
│   └── main.cpp       ← el programa (aquí se pega el código de cada clase)
├── diagram.json       ← la placa de cátedra, para simular con Wokwi dentro de VS Code
├── wokwi.toml         ← le indica a Wokwi qué firmware simular
└── compilar_todo.sh   ← (docente) compila todos los programas del curso
```

---

## Instalación (una sola vez)

1. Instalar [VS Code](https://code.visualstudio.com/).
2. En VS Code, abrir *Extensions* (`Ctrl+Shift+X`), buscar **PlatformIO IDE** e instalarla. La primera vez tarda varios minutos en descargar las herramientas.
3. Descargar este repositorio (botón verde **Code → Download ZIP** en GitHub, o `git clone`).
4. En VS Code: *File → Open Folder…* y elegir la carpeta **`platformio/`** (esta carpeta, no la raíz del repo).

La primera compilación descarga el soporte para ESP32 (unos 300 MB) y tarda unos minutos. Las siguientes tardan segundos.

---

## Uso

Los botones de PlatformIO están en la barra azul de abajo:

| Botón | Acción |
|---|---|
| ✓ | Compilar (*Build*) |
| → | Compilar y cargar a la placa (*Upload*) |
| 🔌 | Abrir el monitor serie |
| 🗑 | Limpiar (*Clean*) |

### Pasar un programa de la clase a PlatformIO

Cada clase tiene sus programas en archivos `sketch.ino`. Para usarlos en PlatformIO:

1. Abrir `src/main.cpp`.
2. Borrar todo y pegar el contenido del `sketch.ino`.
3. **Agregar como primera línea:**
   ```c
   #include <Arduino.h>
   ```
4. Compilar (✓).

Esa línea es la única diferencia. Wokwi y Arduino IDE la agregan solos; PlatformIO no.

### La otra diferencia: el orden de las funciones

En un archivo `.cpp`, una función debe estar **escrita antes de usarse** (o declarada con su prototipo arriba). Arduino IDE y Wokwi ordenan eso automáticamente; PlatformIO no.

Todos los programas del curso ya respetan ese orden: primero las funciones, al final `setup()` y `loop()`. Si el compilador muestra
`'mostrarByte' was not declared in this scope`, la solución es mover esa función más arriba o agregar su prototipo:

```c
void mostrarByte(uint8_t valor);   // prototipo: "esta función existe, está más abajo"
```

(Los prototipos se explican en la Clase 4.)

---

## Simular dentro de VS Code (opcional)

Se puede usar el simulador Wokwi sin salir de VS Code:

1. Instalar la extensión **Wokwi Simulator** en VS Code.
2. `F1` → **Wokwi: Request a New License** (gratis, pide iniciar sesión en wokwi.com).
3. Compilar el proyecto (✓).
4. Abrir `diagram.json` en el editor, o `F1` → **Wokwi: Start Simulator**.

Se ve la misma placa de cátedra que en el navegador. Para el display de 7 segmentos de la Clase 6, reemplazar `diagram.json` por el contenido de [`../placa/diagram_7seg.json`](../placa/diagram_7seg.json).

---

## Cargar en la placa real

1. Conectar el ESP32 por USB (con un cable de **datos**, no solo de carga).
2. Botón → (*Upload*). PlatformIO detecta el puerto solo.
3. Botón 🔌 para ver el monitor serie.

Problemas comunes:

| Síntoma | Solución |
|---|---|
| No encuentra el puerto | Probar otro cable USB. Instalar el driver CH340 o CP2102 según el chip de la placa. |
| Se queda en `Connecting.....` | Mantener apretado el botón **BOOT** de la placa hasta que empiece a cargar. |
| El monitor muestra caracteres raros | Verificar que `monitor_speed = 115200` esté en `platformio.ini`. |
| `Arduino.h: No such file` | Se abrió la carpeta equivocada en VS Code: debe ser `platformio/`. |

---

## Para el docente: compilar todo el curso

`compilar_todo.sh` compila, con el compilador real del ESP32, todos los `sketch.ino` de las clases (ejemplos y soluciones). Sirve para verificar que nada se rompió después de editar el material.

```bash
cd platformio
./compilar_todo.sh
```

Requiere PlatformIO Core en la terminal (`pip install platformio`). Funciona en Linux, macOS y en Windows con Git Bash.
