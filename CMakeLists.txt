# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(projeto_final C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable
add_executable(projeto_final projeto_final.c)

pico_set_program_name(projeto_final "projeto_final")
pico_set_program_version(projeto_final "0.1")

# Generate PIO header
pico_generate_pio_header(projeto_final ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

# Enable/disable output over UART/USB
pico_enable_stdio_uart(projeto_final 0)
pico_enable_stdio_usb(projeto_final 0)

# Add the standard library to the build
target_link_libraries(projeto_final
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_i2c
    hardware_pio
    hardware_clocks
)

# Include directories
target_include_directories(projeto_final PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib  # Certifique-se de que o diretório está correto
)

# Adicionando o arquivo ssd1306.c ao projeto
target_sources(projeto_final PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/ssd1306.c)

# Add extra outputs
pico_add_extra_outputs(projeto_final)