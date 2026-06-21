# Roadmap de traps en Axiom

Esta nota resume el plan conceptual para aterrizar traps/exceptions/interrupciones en Axiom sin saltar demasiado rápido a una implementación completa.

## 1. Modelo mental general

Boot se siente lineal:

```text
reset / entry address
  -> assembly
  -> set stack
  -> call C
```

Traps son una entrada no lineal al kernel:

```text
código normal ejecutándose
  -> algo ocurre
  -> CPU guarda cierta información mínima en CSRs
  -> CPU salta a mtvec
  -> tu código debe guardar el resto
  -> handler decide qué hacer
  -> volver o detener
```

Regla clave:

> En RISC-V, el hardware no guarda todo el contexto por ti. Solo guarda lo mínimo para que tú puedas decidir qué pasó.

## 2. Trap, exception e interrupt

En RISC-V, `trap` es el término general.

```text
trap
├── exception   síncrona
└── interrupt   asíncrona
```

### Exception

Una exception ocurre como consecuencia directa de la instrucción actual.

Ejemplos:

- instrucción ilegal;
- `ecall`;
- acceso inválido a memoria;
- instrucción mal alineada.

Si ejecutas la misma instrucción otra vez en el mismo estado, normalmente obtienes la misma exception.

### Interrupt

Una interrupt ocurre por un evento externo o temporizado, no necesariamente causado por la instrucción actual.

Ejemplos:

- timer interrupt;
- external interrupt;
- software interrupt;
- UART en el futuro, indirectamente mediante controlador de interrupciones.

La instrucción actual no es “culpable” en el mismo sentido. La CPU simplemente fue interrumpida entre instrucciones.

## 3. Qué hace la CPU automáticamente

Cuando ocurre un trap en machine mode, la CPU hace algo conceptualmente parecido a:

```text
mepc    = dirección relacionada con donde ocurrió el trap
mcause  = causa del trap
mtval   = valor extra, si aplica
mstatus = actualiza bits de estado/interrupt enable
pc      = mtvec
```

La CPU no guarda automáticamente registros generales como:

- `ra`;
- `sp`;
- `a0`;
- `t0`;
- `s0`;
- etc.

Eso queda para tu `trap_vector`.

Hay dos niveles de contexto:

```text
Contexto guardado por hardware:
- mepc
- mcause
- mtval
- parte de mstatus

Contexto que debes guardar tú si quieres volver correctamente:
- registros generales x1-x31
- quizá CSRs relevantes
- quizá información de stack/kernel/thread en el futuro
```

## 4. Qué es `mtvec`

`mtvec` significa `machine trap-vector base-address register`.

Es un CSR que le dice a la CPU:

> Cuando ocurra un trap en machine mode, salta aquí.

Por eso normalmente haces conceptualmente:

```text
mtvec = &trap_vector
```

`mtvec` no debería pensarse como “el handler C”. Es mejor pensarlo como:

```text
mtvec -> punto de entrada assembly para traps
```

Ese punto de entrada assembly tiene responsabilidades distintas a una función C normal.

## 5. Por qué `mtvec` normalmente apunta a assembly

Al entrar al trap:

- no sabes si los registros tienen valores importantes;
- no puedes asumir que `a0`, `a1`, `t0`, etc. están libres;
- si llamas a C directamente, el compilador va a usar registros siguiendo la ABI;
- eso puede destruir el estado del código interrumpido.

Flujo típico:

```text
trap ocurre
  -> CPU salta a mtvec
  -> trap_vector.s guarda registros necesarios
  -> lee o prepara CSRs
  -> llama a trap_handler en C
  -> C decide qué hacer
  -> assembly restaura registros
  -> mret
```

Visualmente:

```text
normal code
   |
   | trap
   v
mtvec / trap_vector.s
   |
   | save context
   v
trap_handler(...)
   |
   | decide
   v
restore context
   |
   | mret
   v
normal code, or halt
```

## 6. Qué significa “contexto”

Contexto significa:

> Todo el estado necesario para poder continuar como si el trap hubiera sido una llamada invisible.

Mínimamente, para volver al código interrumpido, necesitas preservar los registros que ese código esperaba conservar.

En RISC-V RV64 tienes registros generales:

```text
x0   zero
x1   ra
x2   sp
x3   gp
x4   tp
x5   t0
x6   t1
x7   t2
x8   s0/fp
x9   s1
x10  a0
x11  a1
x12  a2
x13  a3
x14  a4
x15  a5
x16  a6
x17  a7
x18  s2
...
x27  s11
x28  t3
x29  t4
x30  t5
x31  t6
```

Pero no todos tienen la misma semántica según la ABI.

## 7. Dónde leer qué registros guardar

Leer la RISC-V psABI:

- https://github.com/riscv-non-isa/riscv-elf-psabi-doc

Busca la tabla de integer registers.

La tabla describe, entre otras cosas:

| Registro | ABI name | Preserved across calls? |
|---|---|---|
| `x1` | `ra` | no |
| `x2` | `sp` | yes |
| `x5-x7` | `t0-t2` | no |
| `x10-x17` | `a0-a7` | no |
| `x8-x9` | `s0-s1` | yes |
| `x18-x27` | `s2-s11` | yes |
| `x28-x31` | `t3-t6` | no |

Cuidado: esa tabla habla de llamadas normales a funciones.

Un trap no es una llamada normal.

## 8. Qué registros debe guardar un trap

Depende del tipo de trap handler.

### Opción A: handler que nunca vuelve

Si el trap solo imprime error y hace `panic`/halt, puedes guardar muy poco.

Flujo conceptual:

```text
trap
  -> leer mcause/mepc/mtval
  -> imprimir
  -> halt forever
```

Si nunca vas a volver con `mret`, no necesitas restaurar contexto completo.

Esto sirve para:

- illegal instruction;
- fallos graves;
- debugging inicial.

### Opción B: handler que vuelve

Si quieres hacer:

```asm
ecall
# continuar aquí después del trap
```

Entonces necesitas preservar lo suficiente para que el código continúe correctamente.

Regla práctica:

> Si el trap puede volver al código interrumpido, guarda todos los registros generales que tu trap path pueda alterar.

Como vas a llamar C, y C puede alterar caller-saved registers, lo más simple para un kernel educativo es:

```text
guardar x1-x31 en un trap_frame
leer CSRs
llamar C con puntero al trap_frame
C puede modificar mepc si quiere saltar instrucción
restaurar x1-x31
mret
```

`x0` no se guarda porque siempre es cero.

No es la implementación más optimizada, pero sí es clara.

## 9. Qué debería tener un trap frame inicial

Conceptualmente:

```text
trap_frame:
  ra
  sp
  gp
  tp
  t0
  t1
  t2
  s0
  s1
  a0
  a1
  ...
  t6
  mepc
  mstatus
  mcause
  mtval
```

Los registros generales son el estado del programa.

Los CSRs responden:

```text
mepc    ¿dónde estaba?
mcause  ¿por qué entré?
mtval   ¿hay dato adicional?
mstatus ¿en qué estado estaban ciertas banderas?
```

## 10. Dónde leer los tipos de excepción

Leer la RISC-V Privileged Specification:

- https://riscv.org/technical/specifications/

Busca la tabla de `mcause`.

Algunos códigos importantes:

```text
0   Instruction address misaligned
1   Instruction access fault
2   Illegal instruction
3   Breakpoint
4   Load address misaligned
5   Load access fault
6   Store/AMO address misaligned
7   Store/AMO access fault
8   Environment call from U-mode
9   Environment call from S-mode
11  Environment call from M-mode
12  Instruction page fault
13  Load page fault
15  Store/AMO page fault
```

En RV64, `mcause` tiene un bit alto que distingue interrupt vs exception:

```text
if highest_bit(mcause) == 1:
    trap is interrupt
else:
    trap is exception
```

Y el resto de bits son el código de causa.

En RV64:

```text
interrupt_bit = 1 << 63
cause_code    = mcause & ~(1 << 63)
```

Para Axiom:

- `mcause` puede ser `u64`, porque es un CSR RV64;
- para direcciones como enteros, usar `uptr`;
- para tamaños o índices, usar `usize`.

## 11. Fase 1: solo observar traps

Objetivo:

> Cuando ocurra un trap, imprimir información útil y detener.

No intentes volver todavía.

Qué necesitas entender:

- cómo se escribe `mtvec`;
- cómo se lee `mcause`;
- cómo se lee `mepc`;
- cómo se lee `mtval`;
- cómo distinguir interrupt vs exception.

Flujo:

```text
trap ocurre
  -> trap_vector
  -> trap_handler
  -> leer CSRs
  -> imprimir:
       mcause
       interrupt?
       cause code
       mepc
       mtval
  -> halt
```

Success criteria:

```text
ecall produce:
- mcause = 11, si estás en M-mode
- interrupt = false
- mepc apunta a la instrucción ecall
- mtval probablemente 0
```

En esta fase, como no vuelves, puedes ignorar temporalmente el trap frame completo.

## 12. Fase 2: manejar `ecall` de forma controlada

Objetivo:

> Reconocer solo `ecall from M-mode`, avanzar `mepc` y volver.

Regla:

```text
advance mepc by 4 only for known fixed-size ecall
```

En RISC-V normal, `ecall` es una instrucción de 32 bits.

Entonces para `ecall`:

```text
mepc = mepc + 4
```

Pero para illegal instruction, page fault, access fault, interrupt, etc., no hagas eso ciegamente.

Success criteria:

```text
antes
  print("before ecall")
  ecall
  print("after ecall")

después
  ves ambos prints
```

Pero esto requiere más cuidado con contexto, porque estás regresando.

## 13. Fase 3: diseñar trap frame

Objetivo:

> Hacer explícito qué estado guarda tu trap vector.

Leer:

1. RISC-V psABI, tabla de registros.
2. RISC-V privileged spec, traps y CSRs.
3. Tu propio assembly actual: `src/kernel/trap_vector.s`.

Preguntas que debes responder:

- ¿Dónde voy a poner el trap frame?
  - ¿en el stack actual?
  - ¿en un stack de kernel?
  - ¿global temporal?
- ¿Qué registros guardo?
- ¿En qué orden?
- ¿Cómo garantizo alineación de stack?
- ¿Cómo paso `trap_frame *` a C?
- ¿C puede modificar `mepc` dentro del trap frame?
- ¿Quién escribe `mepc` de vuelta antes de `mret`?

Para un primer kernel educativo, la respuesta más simple suele ser:

```text
guardar trap_frame en el stack actual
pasar sp como trap_frame * a C
C modifica tf->mepc si corresponde
assembly restaura CSRs y registros
mret
```

Limitación: “stack actual” será menos adecuado cuando existan user mode o procesos. Por ahora, en machine mode simple, puede ser aceptable como paso de aprendizaje.

## 14. Qué leer exactamente, en orden

### Lectura 1: RISC-V Privileged Spec — Machine traps

Buscar:

- `mtvec`;
- `mcause`;
- `mepc`;
- `mtval`;
- `mstatus`;
- `mret`;
- Machine trap handling;
- tabla de exception codes.

Preguntas mientras lees:

1. ¿Qué escribe el hardware automáticamente?
2. ¿Qué NO escribe el hardware?
3. ¿Qué contiene `mepc` para exceptions?
4. ¿Cómo se codifica interrupt vs exception en `mcause`?
5. ¿Qué hace `mret`?

No necesitas leer toda la spec.

### Lectura 2: RISC-V psABI — Register convention

Buscar:

- integer register convention;
- stack alignment;
- caller-saved vs callee-saved.

Preguntas:

1. ¿Qué registros puede destruir una función C?
2. ¿Qué registros debe preservar una función C?
3. ¿Qué registros se usan para argumentos?
4. ¿Qué alineación espera el stack antes de llamar a C?

Esto explica por qué llamar C desde trap assembly sin guardar contexto es peligroso.

### Lectura 3: RISC-V Assembly Programmer's Manual

Buscar:

- nombres ABI de registros;
- instrucciones CSR:
  - `csrr`;
  - `csrw`;
  - `csrrw`;
- pseudo-instructions;
- `mret`;
- `call`;
- stack frame basics.

## 15. Modelo mental: trap como “interrupción invisible”

Una función normal tiene contrato:

```text
caller:
  prepara argumentos
  call function

callee:
  preserva s-registers
  puede modificar a/t-registers
  ret
```

Un trap no tiene ese contrato.

El código interrumpido no hizo:

```text
call trap_handler
```

Por eso no preparó nada.

Entonces tu trap vector debe crear artificialmente un entorno seguro:

```text
trap_vector:
  construir frame
  guardar estado
  llamar C
  restaurar estado
  mret
```

## 16. No mezclar todavía syscalls, procesos y kernel mode

Conceptualmente, `ecall` puede ser una syscall en un OS con user mode.

Pero Axiom ahora está en machine mode. Todavía no hay:

- user mode;
- procesos;
- address spaces;
- scheduler;
- S-mode;
- kernel/user boundary real.

Entonces por ahora `ecall` no es una syscall real. Mejor verlo como:

```text
ecall = excepción controlada para probar trap path
```

Más adelante:

```text
user program
  -> ecall
  -> kernel trap handler
  -> syscall dispatch
  -> return to user
```

Pero todavía no conviene cargar esa complejidad.

## 17. Ejercicio concreto: tabla manual de traps

Antes de tocar más código, crear una pequeña nota con:

```text
mcause raw:
interrupt bit:
cause code:
meaning:
should return?:
should advance mepc?:
```

Llenar estos casos:

| Caso | Interrupt? | Cause code | ¿Avanzar `mepc`? | Acción inicial |
|---|---:|---:|---:|---|
| `ecall` en M-mode | no | 11 | sí, +4 | volver o imprimir |
| illegal instruction | no | 2 | no | reportar y halt |
| breakpoint | no | 3 | depende | reportar |
| timer interrupt | sí | 7 aprox. en machine timer | no | atender interrupt |
| external interrupt | sí | 11 aprox. en machine external | no | atender interrupt |

Nota: los códigos de interrupt y exception comparten número, por eso primero debes separar el bit alto.

## 18. Qué implementar primero

No implementar trap frame completo todavía.

Primero:

1. Leer `mcause`, `mepc`, `mtval`.
2. Imprimir los valores.
3. Decodificar:
   - interrupt bit;
   - cause code.
4. Para unknown trap: halt.
5. Para `ecall from M-mode`: opcionalmente avanzar `mepc`.

Si vas a retornar del `ecall`, revisar con cuidado `trap_vector.s`, porque ahí entra preservar registros.

## 19. Resumen corto

```text
mtvec:
  dónde entra la CPU al ocurrir un trap

mepc:
  dónde estaba ejecutando antes del trap

mcause:
  por qué entró

mtval:
  dato extra sobre el trap

trap_vector.s:
  puente assembly que guarda contexto y llama C

trap_handler.c:
  lógica que decide qué hacer

mret:
  volver usando mepc/mstatus
```

Plan sano:

```text
1. Observar y reportar traps.
2. Manejar ecall como caso conocido.
3. Diseñar trap_frame.
4. Solo después pensar en interrupciones reales/syscalls/procesos.
```
