project(StackFromQueues)
cmake_minimum_required(VERSION 3.17)

add_subdirectory(DataStructures)
add_executable("${PROJECT_NAME}" "main.cpp")

target_link_libraries("${PROJECT_NAME}" data-structures-library)
set_target_properties("${PROJECT_NAME}" PROPERTIES LINKER_LANGUAGE CXX)
