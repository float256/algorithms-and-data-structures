project(DijkstraAlgorithm)
cmake_minimum_required(VERSION 3.17)
add_compile_options(-std=c++17)
add_compile_options(/std:c++latest)

add_subdirectory(DataStructures)
add_subdirectory(AlgorithmImplementation)

add_executable("${PROJECT_NAME}" main.cpp)

target_link_libraries("${PROJECT_NAME}" data-structures-library)
target_link_libraries("${PROJECT_NAME}" algorithm-implementation-library)
set_target_properties("${PROJECT_NAME}" PROPERTIES LINKER_LANGUAGE CXX)
