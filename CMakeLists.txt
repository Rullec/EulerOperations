cmake_minimum_required(VERSION 2.8)
project(EulerOps CXX)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")

if(APPLE)
include_directories(/usr/local/Cellar/glew/2.1.0/include/)
endif()

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(GLEW REQUIRED)
# include_directories(${GLEW_INCLUDE_DIR})

# compile
add_subdirectory(src)
set(main_src ./src/main.cpp)
add_executable(${PROJECT_NAME} ${main_src})

message("GLEW: ${GLEW_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} src ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})