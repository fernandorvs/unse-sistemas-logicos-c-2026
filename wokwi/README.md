# Programas para Wokwi web

Los mismos ejemplos y soluciones de [`Clases/`](../Clases/), listos para correr en [Wokwi](https://wokwi.com) desde el navegador: sin instalar nada y sin placa.

Cada carpeta tiene dos archivos:

```
wokwi/Clase 1 - Hola ESP32/ejemplos/02_blink/
├── sketch.ino     ← el programa (idéntico al src/main.cpp del proyecto PlatformIO)
└── diagram.json   ← la placa de cátedra (con display de 7 segmentos en las Clases 6 y 8)
```

## Cómo usarlos

1. Entrar a [wokwi.com](https://wokwi.com) e iniciar sesión.
2. Abrir **[wokwi.com/projects/new/esp32](https://wokwi.com/projects/new/esp32)**.
3. En la pestaña `sketch.ino`: borrar todo y pegar el `sketch.ino` de la carpeta.
4. En la pestaña `diagram.json`: borrar todo y pegar el `diagram.json` de la carpeta.
5. ▶ para correr. **Save** para guardar el proyecto en tu cuenta.

Detalles de la placa y de Wokwi en [`../placa/README.md`](../placa/README.md).

## Para el docente

Si se edita un programa, el `sketch.ino` de acá y el `src/main.cpp` del proyecto en `Clases/` tienen que quedar iguales. `platformio/compilar_todo.sh` lo verifica.
