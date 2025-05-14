#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "constants.hpp"
#include "cpu.hpp"
#include "gpu.hpp"
#include "memory.hpp"
#include <SDL2/SDL.h>
#include <csignal>
#include <iostream>
#include <unordered_map>
#include <vector>

class CONSOLE {
public:
  CONSOLE(const std::string &romPath);
  void runAll();
  static void handleSigint(int sig);
  std::vector<uint8_t> memory;

private:
  void printGlobalDict();
  bool initialize(Memory &myMemory);
  void shutdown();
  bool runLoop(Memory &myMemory);
  bool loadROM(const std::string &filename, std::vector<uint8_t> &memory);
  bool copyDataToRam(std::vector<uint8_t> &memory, const uint32_t &loadLocation,
                     const uint32_t &dataSize, const uint32_t &programLocation);

  CPU myCPU;
  GPU myGPU;
  std::unordered_map<std::string, uint32_t> globalDict;
  uint32_t setupLocation;
  uint32_t loopLocation;
  uint32_t loadLocation;
  uint32_t programLocation;
  uint32_t dataSize;
  static bool quit;
  std::string romPath;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  uint32_t vram[VRAM_SIZE] = {0}, pixels[SCREEN_WIDTH * SCREEN_HEIGHT] = {0};

  bool hws;
};

#endif // CONSOLE_HPP
