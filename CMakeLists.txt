cmake_minimum_required(VERSION 3.30.0)
project(RoseTinted VERSION 0.0.0 LANGUAGES CXX)

include(Sources.cmake)
find_library(NOSTALGIA Nostalgia REQUIRED)
find_library(NOSTALGIA_DEBUG NostalgiaDebug)
mark_as_advanced(NOSTALGIA)
mark_as_advanced(NOSTALGIA_DEBUG)

add_executable(RoseTinted ${ROSE_TINTED_SOURCES})

option(CMAKE_BUILD_TYPE "Choose between 'Debug' or 'Release'" Release)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate a compile_commands.json file" OFF)
mark_as_advanced(CLEAR CMAKE_EXPORT_COMPILE_COMMANDS)
option(ROSE_TINTED_DEBUG_USE_DEBUG "Use NostalgiaDebug when building the debug version" ON)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(RoseTinted PUBLIC NOSTALGIA_DEBUGGING)
    option(ROSE_TINTED_USE_ADDRESS_SANITIZER "Build with address sanitizer" ON)
    if(ROSE_TINTED_DEBUG_USE_DEBUG)
        if(NOSTALGIA_DEBUG)
            set(NOSTALGIA_LIBRARY NostalgiaDebug)
        else()
            set(NOSTALGIA_LIBRARY Nostalgia)
        endif()
    endif()
    message(STATUS "Debug build using: ${NOSTALGIA_LIBRARY}")
else()
    option(ROSE_TINTED_USE_ADDRESS_SANITIZER "Build with address sanitizer" OFF)
    set(NOSTALGIA_LIBRARY Nostalgia)
endif()

target_include_directories(RoseTinted PRIVATE
    src/
    src/thirdparty/getargs/src
    src/thirdparty/freetype
    /usr/local/include/Nostalgia/thirdparty)
target_link_libraries(RoseTinted PRIVATE getargs PUBLIC ${NOSTALGIA_LIBRARY})
target_compile_features(RoseTinted PUBLIC cxx_std_26)

if(ROSE_TINTED_USE_ADDRESS_SANITIZER)
    target_compile_options(RoseTinted PUBLIC -g -fsanitize=address)
    target_link_options(RoseTinted PUBLIC -g -fsanitize=address)
endif()
