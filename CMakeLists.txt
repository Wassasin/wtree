cmake_minimum_required(VERSION 2.8)

project(wtree)

include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE WTREE_SOURCES src/*.cpp)
add_executable(wtree
	${WTREE_SOURCES}
)

add_definitions("-Wall -Wextra -Weffc++ -std=c++0x -pedantic -g -O3")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
target_link_libraries(wtree ${GLUT_glut_LIBRARY})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(wtree ${OPENGL_glu_LIBRARY})
