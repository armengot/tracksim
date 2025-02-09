cmake_minimum_required(VERSION 3.12)

# Simple Tracker Simulator project
project(sim)
set(CMAKE_CXX_STANDARD 14)

# OpenCV required
find_package(OpenCV REQUIRED)

# Headers
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/gen)
include_directories(${CMAKE_SOURCE_DIR}/con)

# Internal libraries
add_library(gen STATIC gen/gen.cpp)
add_library(con STATIC con/con.cpp)
add_library(lib STATIC lib/tools.cpp)

# Link OpenCV
target_link_libraries(gen PRIVATE ${OpenCV_LIBS})
target_link_libraries(con PRIVATE ${OpenCV_LIBS})
target_link_libraries(lib PRIVATE ${OpenCV_LIBS})

# Main executable
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE gen con lib ${OpenCV_LIBS})
