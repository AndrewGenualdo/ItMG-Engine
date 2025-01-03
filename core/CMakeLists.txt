file(
 GLOB_RECURSE CORE_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)

set(CORE_INC_DIR
 ${CMAKE_CURRENT_SOURCE_DIR}
 CACHE PATH "CORE INCLUDE SOURCE PATH"
)

add_library(core STATIC ${CORE_SRC} ${CORE_INC}
        cobb/core/shader.h
        cobb/core/camera.cpp
        cobb/core/object.cpp
        cobb/shapes/line.cpp
        cobb/misc/texture2d.cpp
        cobb/shapes/geometry.cpp
        cobb/shapes/mesh.cpp
        cobb/shapes/torus.h
        cobb/misc/textRenderer.h
        cobb/shapes/model.h
        cobb/shapes/cube.cpp
        cobb/misc/animation.cpp
        cobb/misc/animation.h
        cobb/core/scene.cpp
        cobb/core/scene.h
        cobb/misc/fontRenderer.cpp
        cobb/misc/fontRenderer.h
)

add_custom_target(copyAssetsLine ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/core/cobb/shapes/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/core/cobb/misc/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

find_package(OpenGL REQUIRED)

target_link_libraries(core PUBLIC IMGUI glm assimp enet)

install (TARGETS core DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)

#Trigger asset copy when testing is built
add_dependencies(core copyAssetsLine)