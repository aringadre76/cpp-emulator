cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Emulator VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the source directory
add_subdirectory(src)

# If you have tests, you can include them like this
# enable_testing()
# add_subdirectory(test)
