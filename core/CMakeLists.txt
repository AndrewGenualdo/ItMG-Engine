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
        cobb/shader.cpp
        cobb/shader.hpp
        cobb/window.cpp
        cobb/window.hpp
        cobb/objects/2d/texture2d.cpp
        cobb/objects/2d/texture2d.hpp
        cobb/objects/3d/cube.cpp
        cobb/objects/3d/cube.hpp
        cobb/objects/3d/camera.cpp
        cobb/objects/3d/camera.hpp
        cobb/objects/3d/object.cpp
        cobb/objects/3d/object.hpp
        cobb/objects/1d/line.cpp
        cobb/objects/1d/line.hpp
        cobb/objects/1d/fadeLine.cpp
        cobb/objects/1d/fadeLine.hpp
        cobb/objects/2d/triangle.cpp
        cobb/objects/2d/triangle.hpp
        cobb/objects/2d/fadeTriangle.cpp
        cobb/objects/2d/fadeTriangle.hpp
        cobb/objects/3d/lightSource.cpp
        cobb/objects/3d/lightSource.hpp
        cobb/objects/3d/mesh.cpp
        cobb/objects/3d/mesh.hpp
        cobb/objects/3d/cubeMap.cpp
        cobb/objects/3d/cubeMap.hpp
        cobb/objects/3d/geometry.cpp
        cobb/objects/3d/geometry.hpp
)

add_custom_target(copyAssetsLine ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/cobb/objects/1d/assets/
        ${CMAKE_CURRENT_SOURCE_DIR}/cobb/objects/2d/assets/
        ${CMAKE_CURRENT_SOURCE_DIR}/cobb/objects/3d/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

find_package(OpenGL REQUIRED)
target_link_libraries(core PUBLIC IMGUI glm assimp)

install (TARGETS core DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)

#Trigger asset copy when testing is built
add_dependencies(core copyAssetsLine)