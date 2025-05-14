#include "../include/console.hpp"
#include "../include/memory.hpp"
#include <SDL2/SDL.h>
#include <fstream>

bool CONSOLE::quit = false;

CONSOLE::CONSOLE(const std::string &romPath)
    : memory(0xFFFF, 0), romPath(romPath), window(nullptr), renderer(nullptr),
      texture(nullptr), hws(false) {
  std::signal(SIGINT, CONSOLE::handleSigint);
}

void CONSOLE::handleSigint(int sig) { quit = true; }

bool CONSOLE::loadROM(const std::string &filename,
                      std::vector<uint8_t> &memory) {
  std::ifstream romFile(filename, std::iostream::binary | std::iostream::ate);
  if (!romFile.is_open()) {
    std::cerr << "Error: Failed to open ROM file: " << filename << std::endl;
    return false;
  }

  std::streamsize size = romFile.tellg();
  romFile.seekg(0, std::iostream::beg);

  if (romFile.read(reinterpret_cast<char *>(memory.data() + 0x8000), size)) {
    romFile.close();
  } else {
    std::cerr << "Error: Failed to read ROM file: " << filename << std::endl;
    romFile.close();
    return false;
  }

  romFile.close();

  return true;
}

bool CONSOLE::copyDataToRam(std::vector<uint8_t> &memory,
                            const uint32_t &loadLocation,
                            const uint32_t &dataSize,
                            const uint32_t &programLocation) {
  size_t loadAddressInt, dataSizeInt, programDataAddressInt;
  loadAddressInt = loadLocation;
  dataSizeInt = dataSize;
  programDataAddressInt = programLocation;

  if (loadAddressInt + dataSizeInt <= memory.size() &&
      programDataAddressInt + dataSizeInt <= memory.size()) {
    std::copy(memory.begin() + loadAddressInt,
              memory.begin() + loadAddressInt + dataSizeInt,
              memory.begin() + programDataAddressInt);
  } else {
    std::cerr << "Error: Address out of bounds." << std::endl;
    return false;
  }

  return true;
}

bool CONSOLE::initialize(Memory &myMemory) {
  memory[0x7200] = STOP_FLAG;

  if (!loadROM(romPath, memory)) {
    std::cerr << "Error: load rom failed" << std::endl;
    return false;
  }

  if (!myMemory.extractAddresses(memory, setupAddress, loopAddress, loadAddress,
                                 programDataAddress, dataSizeAddress,
                                 setupLocation, loopLocation, loadLocation,
                                 programLocation, dataSize)) {
    std::cerr << "Error: extract addresses failed" << std::endl;
    return false;
  }

  if (!copyDataToRam(memory, loadLocation, dataSize, programLocation)) {
    std::cerr << "Error: copy data to rom failed" << std::endl;
    return false;
  }

  if (!myCPU.setup(memory, myCPU, setupLocation, myMemory)) {
    std::cerr << "Error: setup failed" << std::endl;
    return false;
  }

  if (romPath.find("hello_world") == std::string::npos) {
    hws = false;
    if (!myGPU.initializeGraphics(memory)) {
      std::cerr << "Error: window initialization failed!" << std::endl;
      return false;
    }
  } else {
    hws = true;
  }

  return true;
}

void CONSOLE::shutdown() {
  if (!hws) {
    myGPU.shutdownGraphics();
  }
}

bool CONSOLE::runLoop(Memory &myMemory) {
  if (!hws) {
    while (!quit) {
      // handleEvents(quit);
      // myGPU.renderGraphics(vram, pixels);

      if (!myCPU.loop(myMemory.buffer, myCPU, loopLocation, renderer, texture,
                      vram, pixels, quit, hws, myGPU, myMemory)) {
        std::cerr << "Error: loop failed" << std::endl;
        return false;
      }
    }
  } else {
    if (!myCPU.loop(myMemory.buffer, myCPU, loopLocation, renderer, texture,
                    vram, pixels, quit, hws, myGPU, myMemory)) {
      std::cerr << "Error: loop failed" << std::endl;
      return false;
    }
  }
  return true;
}

void CONSOLE::runAll() {
  Memory memoryObj(memory);

  if (!initialize(memoryObj)) {
    return;
  }

  if (!runLoop(memoryObj)) {
    return;
  }

  shutdown();
}
