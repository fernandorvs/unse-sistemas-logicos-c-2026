#!/bin/bash
# Compila con PlatformIO todos los sketch.ino del curso (para el docente).
# Uso: desde la carpeta platformio/  ->  ./compilar_todo.sh
# Copia cada sketch a un proyecto temporal como src/main.cpp y lo compila.

cd "$(dirname "$0")"
RAIZ="$(cd .. && pwd)"
TMP="$(mktemp -d)"
cp platformio.ini "$TMP/"
mkdir -p "$TMP/src"
ok=0; fallas=0

while IFS= read -r -d '' sketch; do
  { echo '#include <Arduino.h>'; cat "$sketch"; } > "$TMP/src/main.cpp"
  if pio run -d "$TMP" > "$TMP/salida.log" 2>&1; then
    echo "OK     ${sketch#$RAIZ/}"
    ok=$((ok + 1))
  else
    echo "FALLA  ${sketch#$RAIZ/}"
    grep -E "error|warning" "$TMP/salida.log" | head -10 | sed 's/^/         /'
    fallas=$((fallas + 1))
  fi
done < <(find "$RAIZ/Clases" -name 'sketch.ino' -print0 | sort -z)

echo
echo "Compilaron: $ok   Fallaron: $fallas"
rm -rf "$TMP"
[ "$fallas" -eq 0 ]
