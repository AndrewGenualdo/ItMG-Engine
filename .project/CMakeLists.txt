file(
        GLOB_RECURSE PROJECT_INC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.h *.hpp
)

file(
        GLOB_RECURSE ASSIGNMENT1_SRC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.c *.cpp
)
#Copies this project's asset folder to bin when it is built
add_custom_target(copyAssetsProject ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${PROJECT_INC} DESTINATION include/project)
add_executable(project ${ASSIGNMENT1_SRC} ${PROJECT_INC}
        player.h
        level.h
        platform.h
        player.cpp
        client.h
        server.h
        client.cpp
        server.cpp
        menu.h
        game.h
        menu.cpp
        hitbox.h
        hitbox.cpp
        chatBox.h)
target_link_libraries(project PUBLIC core IMGUI glm assimp enet)
target_include_directories(project PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when project is built
add_dependencies(project copyAssetsProject)