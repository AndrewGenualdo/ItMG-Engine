file(
 GLOB_RECURSE TESTING_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE TESTING_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this assignment2's asset folder to bin when it is built
add_custom_target(copyAssetsT ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${TESTING_INC} DESTINATION include/testing)
add_executable(testing ${TESTING_SRC} ${TESTING_INC}
        main.cpp)
target_link_libraries(testing PUBLIC core IMGUI glm)
target_include_directories(testing PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when testing is built
add_dependencies(testing copyAssetsT)