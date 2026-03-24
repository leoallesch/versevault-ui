cmake_minimum_required(VERSION 3.20)

set(SIERA_ENABLE_LVGL ON CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -- ESP-IDF build (idf.py sets ESP_PLATFORM=1) --
if(ESP_PLATFORM)
    # Point LVGL at the embedded-specific config before any component is loaded.
    set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/src/platform/esp32/lv_conf_esp32.h" CACHE STRING "" FORCE)

    set(EXTRA_COMPONENT_DIRS
        ${CMAKE_SOURCE_DIR}/src/app
        ${CMAKE_SOURCE_DIR}/src/app/ui
        ${CMAKE_SOURCE_DIR}/src/platform/esp32
        ${CMAKE_SOURCE_DIR}/lib/siera
        ${CMAKE_SOURCE_DIR}/lib/siera/src/ui/lvgl/lvgl)
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(alarmclock)
    return()
endif()

# -- Simulator build --

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(alarmclock)

set(SIERA_DRIVER_SIMULATOR ON CACHE BOOL "" FORCE)
set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/src/platform/simulator/lv_conf_simulator.h" CACHE STRING "" FORCE)


add_subdirectory(lib/siera)
add_subdirectory(src/app)
add_subdirectory(src/platform)
