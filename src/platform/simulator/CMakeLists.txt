find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(alarmclock-simulator main.c)

target_link_libraries(alarmclock-simulator
    PRIVATE alarmclock::app siera ${SDL2_LIBRARIES})

target_include_directories(alarmclock-simulator
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR})

