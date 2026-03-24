file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${APP_SOURCES}
        INCLUDE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/view
            ${CMAKE_CURRENT_SOURCE_DIR}/key_manager
            ${CMAKE_CURRENT_SOURCE_DIR}/alarm
            ${CMAKE_CURRENT_SOURCE_DIR}/services
        REQUIRES siera ui)
else()
    add_library(alarmclock-app ${APP_SOURCES})
    add_library(alarmclock::app ALIAS alarmclock-app)

    add_subdirectory(ui)

    target_include_directories(alarmclock-app
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/view>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/key_manager>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/alarm>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/services>)

    target_link_libraries(alarmclock-app
        PUBLIC siera lib-ui)

    target_compile_options(alarmclock-app
        PRIVATE $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra>)
endif()
