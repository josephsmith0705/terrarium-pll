cmake_minimum_required(VERSION 3.20)
project(TerrariumPLL VERSION 1.0.0)

set(FIRMWARE_NAME TerrariumPLL)
set(FIRMWARE_SOURCES
    main.cpp
    syscalls.c
    util/Led.h
    util/Led.cpp
    util/PLL.h
    util/Terrarium.h
    util/Terrarium.cpp
)
set(LIBDAISY_DIR ${CMAKE_SOURCE_DIR}/lib/libDaisy)
include(${LIBDAISY_DIR}/cmake/default_build.cmake)

# Keep libDaisy pristine: generate a local linker-script variant compatible with
# GNU ld versions that reject the '(READONLY)' output-section attribute.
set(LIBDAISY_LINKER_SCRIPT ${LIBDAISY_DIR}/core/STM32H750IB_flash.lds)
set(COMPAT_LINKER_SCRIPT ${CMAKE_BINARY_DIR}/STM32H750IB_flash.compat.lds)
file(READ ${LIBDAISY_LINKER_SCRIPT} _ldscript_contents)
string(REPLACE "(READONLY)" "" _ldscript_contents "${_ldscript_contents}")
file(WRITE ${COMPAT_LINKER_SCRIPT} "${_ldscript_contents}")

get_target_property(_firmware_link_options ${FIRMWARE_NAME} LINK_OPTIONS)
if(_firmware_link_options)
    list(TRANSFORM _firmware_link_options REPLACE
        "${LIBDAISY_LINKER_SCRIPT}" "${COMPAT_LINKER_SCRIPT}")
    set_target_properties(${FIRMWARE_NAME} PROPERTIES
        LINK_OPTIONS "${_firmware_link_options}"
    )
endif()

set_target_properties(${FIRMWARE_NAME} PROPERTIES
    LINK_DEPENDS ${COMPAT_LINKER_SCRIPT}
)

target_include_directories(${FIRMWARE_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

set_target_properties(${FIRMWARE_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

target_link_options(${FIRMWARE_NAME} PRIVATE
    -flto=auto
)

if(NOT PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    # Git auto-ignore out-of-source build directory
    file(GENERATE OUTPUT .gitignore CONTENT "*")
endif()

