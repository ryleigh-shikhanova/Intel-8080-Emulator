# Intel 8080 Emulator

A C++ implementation of the Intel 8080 CPU architecture, created to explore
instruction decoding, register and flag behavior, memory operations, I/O, and
low-level software design.

> [!IMPORTANT]
> This project is undergoing a CLI-first modernization. The CPU contains decode
> paths for the 256-byte opcode table, but instruction behavior has not yet been
> validated by a comprehensive automated test suite. Do not treat the current
> version as a cycle-accurate or correctness-certified emulator.

## Project goals

- Model the Intel 8080 registers, flags, memory, stack, and instruction cycle.
- Keep the CPU core independent from its user interfaces and attached devices.
- Provide deterministic execution through step and bounded-run operations.
- Verify instruction behavior with automated tests and diagnostic programs.
- Support repeatable builds on Windows and Linux.
- Integrate with a future Intel 8080 assembler written in Rust.

## Current functionality

- Decode cases for all 256 opcode values
- Intel 8080 register and flag state
- 16-bit program counter and stack pointer
- Program loading from hexadecimal text
- Memory reads and writes
- Input and output device abstractions
- Register, flag, instruction, and memory inspection helpers
- Experimental PixelGameEngine GUI
- Example countdown program

The presence of an opcode case does not yet guarantee that its behavior, flags,
timing, or edge cases are correct.

## Current interface

The present executable launches an experimental GUI that displays CPU state and
provides run, stop, reset, memory-view, and basic I/O controls. The example
program is currently embedded in the application source.

The GUI is considered legacy functionality. It is being replaced by a small
command-line application so the emulator can be built, tested, and exercised
without graphical dependencies. The original GUI version is preserved by the
`gui-v1.0` tag.

## Building the current version

### Requirements

- Windows 10 or later
- Visual Studio 2022
- Desktop development with C++ workload
- A compiler with C++20 support

### Build steps

```text
1. Clone the repository.
2. Open 8080.sln in Visual Studio.
3. Select the x64 Debug or Release configuration.
4. Build the solution.
5. Run the 8080 project.
```

The current build is Visual Studio-specific. A CMake-based cross-platform build
is planned.

## Repository structure

```text
8080/
|-- CPU/           Intel 8080 state and instruction implementation
|-- Debugger/      Register, flag, and memory inspection helpers
|-- IO/            I/O device interfaces and command-line device code
|-- Include/       PixelGameEngine and QuickGUI headers
|-- Test Programs/ Example Intel 8080 programs
`-- 8080.cpp       Current GUI application and execution entry point
```

## Known limitations

- No automated instruction-level test suite
- Several instructions and flag behaviors require verification
- No verified cycle accuracy
- The GUI and CPU execution code share state without complete synchronization
- The current worker thread does not have a clean shutdown mechanism
- The memory implementation requires correction and modernization
- The application is coupled to the experimental GUI
- The default build is limited to Visual Studio on Windows
- Program input is not yet exposed through a general-purpose CLI

These limitations are documented so that project progress can be evaluated from
reproducible evidence rather than unsupported accuracy claims.

## Modernization roadmap

### 1. CPU correctness and ownership

- Replace raw memory allocation with a fixed-size 64 KiB container.
- Give registers and flags explicit initial values.
- Define safe copy and move behavior.
- Correct instruction and flag edge cases.

### 2. Automated verification

- Add unit tests for individual instructions.
- Add table-driven tests for flags and arithmetic edge cases.
- Run compatible Intel 8080 diagnostic programs.
- Add sanitizer-enabled test builds.

### 3. Interface separation

- Build the CPU core as an interface-independent library.
- Add deterministic `step()` and bounded `run()` operations.
- Implement a CLI for loading programs, tracing instructions, and dumping state.
- Keep any future GUI as an optional frontend.

### 4. Portable development workflow

- Add CMake configuration.
- Build and test on Windows and Linux.
- Add GitHub Actions continuous integration.
- Document reproducible examples and expected output.

## Planned CLI experience

The CLI is expected to support workflows similar to:

```console
i8080 run program.bin --origin 0x0100 --max-instructions 100000
i8080 trace program.bin --origin 0x0100
i8080 inspect --registers --memory 0x0000:0x00ff
```

These commands describe the intended interface and are not implemented in the
current version.

## Project status

This repository began as a computer architecture learning project and is now
being developed into a tested systems-programming portfolio project. The next
milestone is a portable CPU library with automated tests and a minimal CLI.
