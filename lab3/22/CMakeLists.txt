project(SimpleFileSystem)
cmake_minimum_required(VERSION 3.17)
add_compile_options(-std=c++17)

add_subdirectory(FileSystem)
add_subdirectory(DataStructures)
add_executable("${PROJECT_NAME}" "main.cpp")

target_link_libraries("${PROJECT_NAME}" data-structures-library)
target_link_libraries("${PROJECT_NAME}" file-system-binary-tree-library)
set_target_properties("${PROJECT_NAME}" PROPERTIES LINKER_LANGUAGE CXX)
