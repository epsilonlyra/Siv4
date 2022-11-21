cmake_minimum_required(VERSION 3.10)

project(SIV_4)
set(CMAKE_CXX_STANDARD 17)

set(PROGRAM_SRCS
    main.cpp
    include/source/field_class.cpp
    SFML/
)

include_directories(
    include/
)

# Find version 2.5.0 of the SFML library on this computer,
# but only use the graphics package
# If the package can't be found, terminate the script
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)


# Add the following SFML module to the application:
target_link_libraries(sfml-graphics)


add_executable(amogus ${PROGRAM_SRCS})

