# Add the executable target for your project
add_executable(Emulator main.cpp cpu.cpp instruction.cpp gpu.cpp console.cpp memory.cpp)

# Let the build system know where to find the header files
target_include_directories(Emulator PRIVATE ../include)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2
target_link_libraries(Emulator PRIVATE ${SDL2_LIBRARIES})

# Add profiling flags
target_compile_options(Emulator PRIVATE -pg)
target_link_libraries(Emulator PRIVATE -pg)
