# BananaSlug Emulator

BananaSlug Emulator is a complete emulator for a custom RISC-style architecture, featuring a CPU, GPU, memory management, and ROM loading. It is written in C++17 and uses SDL2 for graphics output.

## Features

- Full CPU implementation with a custom instruction set  
- Memory subsystem with ROM loading and address extraction  
- GPU rendering via SDL2 with a virtual framebuffer  
- Console interface to load and run `.slug` ROM files  
- Automated test suite for verifying emulator behavior  

## Prerequisites

- A C++17-compatible compiler (e.g. `g++`)  
- CMake 3.10 or higher  
- SDL2 development libraries  

On Ubuntu:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libsdl2-dev
```

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The emulator executable will be located at:
```
build/src/Emulator
```

## Running

```bash
./build/src/Emulator path/to/rom.slug
```

Example:

```bash
./build/src/Emulator .github/workflows/roms/hello_world1.slug
```

## Testing

From the project root:

```bash
./tests/run_test.sh tests/hello_world1 build/src/Emulator .github/workflows/roms/hello_world1.slug
./tests/run_test.sh tests/hello_world2 build/src/Emulator .github/workflows/roms/hello_world2.slug
./tests/run_test.sh tests/hello_world3 build/src/Emulator .github/workflows/roms/hello_world3.slug
```

## Project Structure

- **include/** — Header files (constants, CPU, GPU, memory, console, instructions)  
- **src/** — Source files and CMake build script  
- **tests/** — Test cases and test runner script  
- **.github/workflows/** — CI configuration and test scripts  
- **report.md**, **report.pdf** — Project report and documentation  

## Contributing

Contributions are welcome. Please open an issue or submit a pull request.

## License

This project does not specify a license. 