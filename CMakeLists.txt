cmake_minimum_required(VERSION 2.8)
project(EulerOps CXX)
set (CMAKE_CXX_STANDARD 11)

# compile
add_subdirectory(src)
set(main_src ./src/main.cpp)
add_executable(${PROJECT_NAME} ${main_src})

# link
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} src ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})