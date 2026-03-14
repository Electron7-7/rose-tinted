cmake_minimum_required(VERSION 3.30.0...4.2.1)
project(RoseTinted VERSION 0.0.0 LANGUAGES CXX)

include(Sources.cmake)
find_library(NOSTALGIA Nostalgia PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
find_library(NOSTALGIA_DEBUG Nostalgiad PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
if(NOT NOSTALGIA_DEBUG)
    if(NOT NOSTALGIA)
        message(FATAL_ERROR "Nostalgia library not found")
    else()
        message(STATUS "Could not find the debug version of Nostalgia; debug builds will use the release version")
        set(NOSTALGIA_DEBUG ${NOSTALGIA})
    endif()
endif()

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/assets)

add_custom_command(OUTPUT models.hpp
    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR="${ASSETS_DIR}" -P ${ASSETS_DIR}/EmbedAssets.cmake
    DEPENDS ${ASSETS_DIR}/models/cockpit.obj)

add_executable(RoseTinted ${ROSE_TINTED_SOURCES} models.hpp)

if(MSVC)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    target_compile_features(RoseTinted PUBLIC cxx_std_23)
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    option(ROSE_TINTED_USE_ADDRESS_SANITIZER "Build with address sanitizer" OFF)
    target_compile_features(RoseTinted PUBLIC cxx_std_26)
endif()

option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate a compile_commands.json file" OFF)
mark_as_advanced(CLEAR CMAKE_EXPORT_COMPILE_COMMANDS)

target_include_directories(RoseTinted PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    src/
    src/thirdparty
    src/thirdparty/freetype
    src/thirdparty/Nostalgia/thirdparty)
target_link_libraries(RoseTinted PUBLIC $<$<CONFIG:Debug>:${NOSTALGIA_DEBUG}> $<$<CONFIG:Release>:${NOSTALGIA}>)
target_compile_definitions(RoseTinted PUBLIC $<$<CONFIG:Debug>:NOSTALGIA_DEBUGGING>)
target_compile_options(RoseTinted PUBLIC $<$<BOOL:${ROSE_TINTED_USE_ADDRESS_SANITIZER}>:-g -fsanitize=address>)
target_link_options(RoseTinted PUBLIC $<$<BOOL:${ROSE_TINTED_USE_ADDRESS_SANITIZER}>:-g -fsanitize=address>)
