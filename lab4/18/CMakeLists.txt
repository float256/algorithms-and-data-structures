project(DijkstraAlgorithm)
cmake_minimum_required(VERSION 3.17)
add_compile_options(-std=c11)

add_subdirectory(DataStructures)
add_executable("${PROJECT_NAME}" "main.c")

target_link_libraries("${PROJECT_NAME}" data-structures-library)
set_target_properties("${PROJECT_NAME}" PROPERTIES LINKER_LANGUAGE C)
