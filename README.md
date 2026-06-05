# Terrarium PLL Synth

Firmware for a PLL synth pedal on Daisy Seed + Terrarium hardware.

## Building

    cmake \
        -GNinja \
        -DTOOLCHAIN_PREFIX=/usr/local \
        -DCMAKE_TOOLCHAIN_FILE=lib/libDaisy/cmake/toolchains/stm32h750xx.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -B build .
    cmake --build build

Or, if using VS Code use the task.
