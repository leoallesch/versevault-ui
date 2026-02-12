cmake_minimum_required(VERSION 3.16)

project(versevault-ui
    VERSION 0.1.0
    LANGUAGES C CXX
    DESCRIPTION "VerseVault UI Application"
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Enable siera UI module (includes LVGL)
set(SIERA_ENABLE_UI ON CACHE BOOL "" FORCE)
set(SIERA_DRIVER_SIMULATOR ON CACHE BOOL "" FORCE)

add_subdirectory(lib/siera)

# Generate UI source files from XML (only when XML changes)
find_program(NODE_EXECUTABLE node REQUIRED)
file(GLOB_RECURSE UI_XML_SOURCES "${CMAKE_SOURCE_DIR}/src/ui/*.xml")
set(UI_GENERATE_STAMP "${CMAKE_BINARY_DIR}/ui-generate.stamp")

add_custom_command(
    OUTPUT ${UI_GENERATE_STAMP}
    COMMAND ${NODE_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/lved-cli.js generate ${CMAKE_SOURCE_DIR}/src/ui
    COMMAND ${CMAKE_COMMAND} -E touch ${UI_GENERATE_STAMP}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${UI_XML_SOURCES}
    COMMENT "Generating UI sources from XML"
)
add_custom_target(ui-generate DEPENDS ${UI_GENERATE_STAMP})

add_subdirectory(src/ui)
add_dependencies(lib-ui ui-generate)

# Application library (platform-agnostic)
add_library(versevault-app
    src/view/demo_view.c
)

target_link_libraries(versevault-app
    PUBLIC
        siera::ui
        lib-ui
)

target_include_directories(versevault-app
    PUBLIC
        src/view
        src/app
)

# Simulator target
add_executable(versevault-simulator
    src/platform/simulator/main.c
    src/platform/simulator/display_simulator.c
)

target_link_libraries(versevault-simulator
    PRIVATE
        versevault-app
        siera::driver
        ${SDL2_LIBRARIES}
)

target_include_directories(versevault-simulator
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        src/platform/simulator
)

