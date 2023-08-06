cmake_minimum_required(VERSION 2.8)

project(assignment1)

add_executable(COSC363 Assignment 01.out COSC363 Assignment 01.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( COSC363 Assignment 01.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

