include(${CMAKE_CURRENT_LIST_DIR}/file_list_gen.cmake)

# Generate UI source files from XML when any .xml changes
# Guarded: IDF runs this file in script mode to resolve deps — add_custom_command
# is not scriptable, so skip the generation target in that context.
if(NOT CMAKE_SCRIPT_MODE_FILE)
    find_program(NODE_EXECUTABLE node REQUIRED)
    file(GLOB_RECURSE UI_XML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.xml")
    set(UI_GENERATE_STAMP "${CMAKE_BINARY_DIR}/ui-generate.stamp")

    add_custom_command(
        OUTPUT ${UI_GENERATE_STAMP}
        COMMAND ${NODE_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/lved-cli.js generate ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E touch ${UI_GENERATE_STAMP}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${UI_XML_SOURCES}
        COMMENT "Generating UI sources from XML"
    )
    add_custom_target(ui-generate DEPENDS ${UI_GENERATE_STAMP})
endif()

if(ESP_PLATFORM)
    idf_component_register(
        COMPONENT_NAME ui
        SRCS ${PROJECT_SOURCES}
        INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        REQUIRES siera)
else()
    add_library(lib-ui ${PROJECT_SOURCES})
    add_dependencies(lib-ui ui-generate)
    target_include_directories(lib-ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(lib-ui PUBLIC lvgl)
endif()
