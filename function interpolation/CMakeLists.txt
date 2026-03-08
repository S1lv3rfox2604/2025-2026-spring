cmake_minimum_required(VERSION 3.10)
project(InterpolationProject)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(input)
add_subdirectory(solver)
add_subdirectory(output)

add_executable(main main.cpp)

target_link_libraries(main input solver output)