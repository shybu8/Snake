cmake_minimum_required(VERSION 3.10)
project(snake CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
	src/main.cpp
	src/structures_impl.cpp
	src/resources.cpp
)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/raylib)
add_subdirectory(external/fmt)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE external/raygui/src)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} fmt)

add_executable(res_gen tools/res_gen.cpp)
target_link_libraries(res_gen raylib)

set(RESOURCES
    ${CMAKE_SOURCE_DIR}/resources/apple.png
    ${CMAKE_SOURCE_DIR}/resources/grass.png
    ${CMAKE_SOURCE_DIR}/resources/snake_head.png
    ${CMAKE_SOURCE_DIR}/resources/snake_straight.png
    ${CMAKE_SOURCE_DIR}/resources/snake_tail.png
    ${CMAKE_SOURCE_DIR}/resources/snake_turn.png
)

set(GEN_DIR ${CMAKE_SOURCE_DIR}resources/generated)
set(GENERATED_HEADERS)

foreach(RES ${RESOURCES})
  get_filename_component(NAME_WE "${RES}" NAME_WE)
  set(OUT "${GEN_DIR}/${NAME_WE}.h")
  list(APPEND GENERATED_HEADERS "${OUT}")

  add_custom_command(
    OUTPUT  "${OUT}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${GEN_DIR}"
    COMMAND $<TARGET_FILE:res_gen> "${RES}" "${OUT}"
    DEPENDS res_gen "${RES}"
    VERBATIM
    COMMENT "ExportImageAsCode: ${RES} -> ${OUT}"
  )
endforeach()


add_custom_target(embed_assets ALL DEPENDS ${GENERATED_HEADERS})
add_dependencies(${PROJECT_NAME} embed_assets)
target_include_directories(${PROJECT_NAME} PRIVATE ${GEN_DIR})
source_group("Generated" FILES ${GENERATED_HEADERS})

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
elseif(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
endif()
