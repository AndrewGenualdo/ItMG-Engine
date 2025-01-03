cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Include guards
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

project(Engine-2)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/libs)

include(external/cpm.cmake)

# add libraries
include(external/glfw.cmake)
include(external/imgui.cmake)
include(external/glm.cmake)
include(external/assimp.cmake)
# include(external/freetype.cmake)
include(external/enet.cmake)


# Disable treating warnings as errors for Assimp specifically
# This allows it to run on CLion
if(TARGET assimp)
  target_compile_options(assimp PRIVATE -Wno-error)
endif()
if(TARGET enet)
  target_compile_options(enet PRIVATE -Wno-error)
endif()



add_subdirectory(core)
add_subdirectory(.project)