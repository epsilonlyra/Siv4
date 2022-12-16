# I don't know how it works, just copy - pasted
# All credit goes to  mister Dane Bulat

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Set project name and version
project(SIV4 VERSION 0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "-O3")


# Find SFML shared libraries
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# Compile executable
add_executable(SIV4 main.cpp  include/source/simulation.cpp include/source/errors.cpp)

# Set include directory search paths
target_include_directories(SIV4
  PRIVATE
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )


# Link executable to required SFML modules
target_link_libraries(SIV4 sfml-graphics sfml-audio)



