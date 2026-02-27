cmake_minimum_required(VERSION 3.30.0...4.2.1)
project(RoseTinted VERSION 0.0.0 LANGUAGES CXX)

include(Sources.cmake)
find_library(NOSTALGIA Nostalgia REQUIRED PATHS src/lib)
find_library(NOSTALGIA_DEBUG Nostalgiad PATHS src/lib)
if(NOT NOSTALGIA_DEBUG)
    set(NOSTALGIA_DEBUG ${NOSTALGIA})
endif()
mark_as_advanced(NOSTALGIA)
mark_as_advanced(NOSTALGIA_DEBUG)

add_executable(RoseTinted ${ROSE_TINTED_SOURCES})

if(MSVC)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    option(ROSE_TINTED_USE_ADDRESS_SANITIZER "Build with address sanitizer" OFF)
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        option(ROSE_TINTED_USE_ADDRESS_SANITIZER "Build with address sanitizer" ON)
    else()
        option(ROSE_TINTED_USE_ADDRESS_SANITIZER "Build with address sanitizer" OFF)
    endif()
endif()

option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate a compile_commands.json file" OFF)
mark_as_advanced(CLEAR CMAKE_EXPORT_COMPILE_COMMANDS)

target_include_directories(RoseTinted PRIVATE
    src/
    src/thirdparty
    src/thirdparty/freetype
    src/thirdparty/Nostalgia/thirdparty)
target_link_directories(RoseTinted PUBLIC src/lib)
target_link_libraries(RoseTinted PUBLIC $<$<CONFIG:Debug>:${NOSTALGIA_DEBUG},${NOSTALGIA}>)
target_compile_definitions(RoseTinted PUBLIC $<$<CONFIG:Debug>:NOSTALGIA_DEBUG>)
if(ROSE_TINTED_USE_ADDRESS_SANITIZER)
    target_compile_options(RoseTinted PUBLIC $<$<CONFIG:Debug>:-g -fsanitize=address>)
endif()
if(MSVC)
    target_compile_features(RoseTinted PUBLIC cxx_std_23)
else()
    target_compile_features(RoseTinted PUBLIC cxx_std_26)
endif()
