# Terrarium PLL Synth

Firmware for a PLL synth pedal on Daisy Seed + Terrarium hardware.

## Building

Install an ARM bare-metal toolchain (for example Homebrew `arm-none-eabi-gcc`
or ARM GNU Toolchain from Arm).

    cmake \
        -GNinja \
        -DTOOLCHAIN_PREFIX=<toolchain install prefix> \
        -DCMAKE_TOOLCHAIN_FILE=lib/libDaisy/cmake/toolchains/stm32h750xx.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -B build .
    cmake --build build

Examples for `TOOLCHAIN_PREFIX`:
- Apple Silicon Homebrew: `/opt/homebrew`
- Intel Homebrew: `/usr/local`
- DaisyToolchain bundle: `/Library/DaisyToolchain/0.2.0`

Or, if using VS Code use the task.
