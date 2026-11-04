# Cordo

A modular RISC ISA emulator built on a tail-call interpreter.
Architecture-agnostic core designed as a foundation for multi-ISA emulators.
Written in C23 with GNU extensions.

## Status

Currently supports a subset of **RV32I** as an MVP. Currently implements two instructions from RISC-V (RV32I) - add, addi. More instructions and ISAs sre planed.

The current focus is on validating the core architecture and the tail-call dispatch mechanism.

## Bild

Requires **GCC 14+** (for C23 support and `[[gnu::musttail]]`).
Optimization is strictly required (`-O2` or higher) for tail-call elimination. Compiling without it will cause a stack overflow.

```sh
gcc -std=gnu23 -O3 -Iinc src/*.c -o cordo
```

## License

Distributed under the BSD 3-Clause License. See [LICENSE](LICENSE) for more information.
