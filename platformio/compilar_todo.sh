#!/bin/bash
# Compila con PlatformIO todos los proyectos del curso (para el docente).
# Uso: desde la carpeta platformio/
#   ./compilar_todo.sh              -> compila para las dos placas
#   ./compilar_todo.sh esp32dev     -> solo ESP32 DevKit
#   ./compilar_todo.sh esp32c3      -> solo ESP32-C3 Super Mini
#
# Además verifica que en cada proyecto:
#   - platformio.ini sea igual al de plantilla/ (salvo la 1.ª línea, el título),
#   - include/placa_c3.h sea igual al de plantilla/,
#   - wokwi/.../sketch.ino (para Wokwi web) sea idéntico a su src/main.cpp.
# Para no recompilar el framework 67 veces, compila el src/main.cpp de cada
# proyecto dentro de un único proyecto temporal.

cd "$(dirname "$0")"
PLANTILLA="$(pwd)/plantilla"
RAIZ="$(cd .. && pwd)"
ENTORNOS=("$@")
[ $# -eq 0 ] && ENTORNOS=(esp32dev esp32c3)

TMP="$(mktemp -d)"
cp "$PLANTILLA/platformio.ini" "$TMP/"
cp -r "$PLANTILLA/include" "$TMP/"
mkdir -p "$TMP/src"
ok=0; fallas=0

proyectos=("$PLANTILLA")
while IFS= read -r -d '' ini; do
  proyectos+=("$(dirname "$ini")")
done < <(find "$RAIZ/Clases" -name platformio.ini -print0 | sort -z)

for p in "${proyectos[@]}"; do
  if ! tail -n +2 "$p/platformio.ini" | cmp -s - <(tail -n +2 "$PLANTILLA/platformio.ini"); then
    echo "DISTINTO  ${p#$RAIZ/}/platformio.ini"; fallas=$((fallas + 1))
  fi
  if ! cmp -s "$p/include/placa_c3.h" "$PLANTILLA/include/placa_c3.h"; then
    echo "DISTINTO  ${p#$RAIZ/}/include/placa_c3.h"; fallas=$((fallas + 1))
  fi
  [ "$p" = "$PLANTILLA" ] && continue
  w="$RAIZ/wokwi/${p#$RAIZ/Clases/}"
  if ! cmp -s "$w/sketch.ino" "$p/src/main.cpp"; then
    echo "DISTINTO  ${w#$RAIZ/}/sketch.ino no es igual a ${p#$RAIZ/}/src/main.cpp"; fallas=$((fallas + 1))
  fi
done

for entorno in "${ENTORNOS[@]}"; do
  echo "=== $entorno ==="
  for p in "${proyectos[@]}"; do
    cp "$p/src/main.cpp" "$TMP/src/main.cpp"
    if pio run -d "$TMP" -e "$entorno" > "$TMP/salida.log" 2>&1; then
      echo "OK     ${p#$RAIZ/}"
      ok=$((ok + 1))
    else
      echo "FALLA  ${p#$RAIZ/}"
      grep -E "error|warning" "$TMP/salida.log" | head -10 | sed 's/^/         /'
      fallas=$((fallas + 1))
    fi
  done
  echo
done

echo "Compilaron: $ok   Fallaron: $fallas"
rm -rf "$TMP"
[ "$fallas" -eq 0 ]
