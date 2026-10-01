# Chuleta de C para el ESP32

Todo lo que se ve en el curso, en una página. Entre corchetes, la clase donde se explica.

---

## Estructura de un programa [1]

```c
// Comentario de una línea
const int L0 = 4;            // constantes y variables globales arriba

void setup() {               // se ejecuta UNA vez
  Serial.begin(115200);
  pinMode(L0, OUTPUT);
}

void loop() {                // se repite para siempre
  digitalWrite(L0, HIGH);
  delay(500);
}
```

## Imprimir [1, 2]

```c
Serial.printf("Hola\n");                  // \n = salto de línea
Serial.printf("n = %d\n", n);             // entero con signo
Serial.printf("n = %u\n", n);             // entero sin signo
Serial.printf("n = 0x%02X\n", n);         // hexadecimal, 2 dígitos
Serial.printf("t = %.2f\n", t);           // float con 2 decimales
Serial.printf("%lu ms\n", millis());      // unsigned long
```

## Pines [1, 3]

| Instrucción | Qué hace |
|---|---|
| `pinMode(pin, OUTPUT)` | El pin es salida |
| `pinMode(pin, INPUT_PULLUP)` | El pin es entrada con resistencia a 3,3 V |
| `digitalWrite(pin, HIGH)` / `LOW` | Pone 1 o 0 en la salida |
| `digitalRead(pin)` | Lee la entrada: `HIGH` (1) o `LOW` (0) |
| `delay(ms)` | Espera (¡bloquea!) |
| `millis()` | Milisegundos desde que arrancó (`unsigned long`) |

Pulsador con `INPUT_PULLUP`: **apretado = `LOW`**. Por eso: `bool a = !digitalRead(BOTON_A);`

## Tipos [2]

| Tipo | Rango | Uso |
|---|---|---|
| `bool` | `false` (0) / `true` (1) | Señales lógicas |
| `uint8_t` | 0 … 255 | Un byte, 8 bits, 8 LEDs |
| `int` | ±2.147 millones | Números en general |
| `unsigned long` | 0 … 4.294 millones | Tiempos con `millis()` |
| `float` | con decimales | Mediciones |

## Operadores

| Aritméticos [2] | Relacionales [3] | Lógicos [3] | Bit a bit [5] |
|---|---|---|---|
| `+ - * /` | `==` igual | `&&` Y (AND) | `&` AND bit a bit |
| `%` resto | `!=` distinto | `\|\|` O (OR) | `\|` OR bit a bit |
| `++` `--` | `< > <= >=` | `!` NO (NOT) | `^` XOR bit a bit |
| `+=` `-=` | | | `~` invertir todos |
| | | | `<<` `>>` desplazar |

⚠️ `=` **asigna**, `==` **compara**. ⚠️ `&&` es lógico (todo el valor), `&` es bit a bit.

## Compuertas en C [3]

| Compuerta | Con `bool` | Bit a bit sobre bytes |
|---|---|---|
| AND | `a && b` | `x & y` |
| OR | `a \|\| b` | `x \| y` |
| NOT | `!a` | `~x` |
| NAND | `!(a && b)` | `~(x & y)` |
| NOR | `!(a \|\| b)` | `~(x \| y)` |
| XOR | `a != b` | `x ^ y` |
| XNOR | `a == b` | `~(x ^ y)` |

## Bits [5]

```c
bool bit = (x >> n) & 1;     // leer el bit n
x |=  (1 << n);              // poner en 1 el bit n
x &= ~(1 << n);              // poner en 0 el bit n
x ^=  (1 << n);              // invertir el bit n
uint8_t m = 0b10110001;      // literal binario
uint8_t h = 0xB1;            // literal hexadecimal (el mismo número)
```

## Decisiones [3, 8]

```c
if (a && b) {
  // ...
} else if (a) {
  // ...
} else {
  // ...
}

switch (modo) {
  case 0:  /* ... */  break;
  case 1:  /* ... */  break;
  default: /* ... */  break;
}
```

## Bucles [4]

```c
for (int i = 0; i < 8; i++) {   // i vale 0, 1, ..., 7
  digitalWrite(LEDS[i], HIGH);
}

while (digitalRead(BOTON_A) == HIGH) {
  // espera mientras no se apriete A
}
```

## Funciones [4]

```c
bool compuertaXOR(bool a, bool b) {   // tipo que devuelve, nombre, parámetros
  return a != b;
}

void mostrarByte(uint8_t valor) {     // void = no devuelve nada
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDS[i], (valor >> i) & 1);
  }
}
```

## Arreglos [4, 6]

```c
const int LEDS[8] = {4, 16, 17, 18, 19, 21, 22, 23};   // índices 0 a 7
int notas[5] = {7, 4, 9, 10, 6};
notas[0] = 8;            // el primero es [0]
                         // el último es [4]; notas[5] NO EXISTE
```

## Estado y tiempo [7]

```c
unsigned long ultimo = 0;          // global: recuerda entre vueltas de loop()

void loop() {
  if (millis() - ultimo >= 500) {  // ¿pasaron 500 ms?
    ultimo = millis();
    // hacer algo cada 500 ms, sin bloquear
  }
}
```

## Máquinas de estado [8]

```c
enum Estado { ROJO, VERDE, AMARILLO };
Estado estado = ROJO;

void loop() {
  switch (estado) {
    case ROJO:     /* salidas y transiciones */ break;
    case VERDE:    /* ... */ break;
    case AMARILLO: /* ... */ break;
  }
}
```

---

## Errores más comunes

| Mensaje del compilador | Causa probable |
|---|---|
| `expected ';' before …` | Falta `;` en la línea **anterior** |
| `'xxx' was not declared in this scope` | Nombre mal escrito (¡mayúsculas!) o variable declarada en otro bloque |
| `expected '}' at end of input` | Falta cerrar una llave |
| `expected ')' before …` | Falta cerrar un paréntesis |
| (compila pero el `if` siempre entra) | Usaste `=` en lugar de `==` |
| (compila pero el `if` no hace nada) | Pusiste `;` después del `if (...)` |
