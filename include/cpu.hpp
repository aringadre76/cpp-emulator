#ifndef CPU_HPP
#define CPU_HPP

#include "constants.hpp"
#include "gpu.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>

struct Instructions;

class CPU {
public:
  // Initialize CPU registers and special-purpose registers
  CPU();
  ~CPU();

  bool extractInstructions(const std::vector<uint8_t> &memory,
                           Instructions &instruction);

  int16_t readRegister(int registerNum);
  void writeRegister(int registerNum, int16_t registerVal);

  uint16_t getPC();
  void setPC(uint16_t newPC);

  bool setup(std::vector<uint8_t> &memory, CPU &myCPU,
             const uint32_t &setupLocation, Memory &myMemory);
  bool loop(std::vector<uint8_t> &memory, CPU &myCPU,
            const uint32_t &loopLocation, SDL_Renderer *renderer,
            SDL_Texture *texture, uint32_t *vram, uint32_t *pixels, bool &quit,
            bool &slug, GPU &myGPU, Memory &myMemory);

  uint16_t zero_register;
  uint16_t stack_pointer_register;
  void temp(std::vector<uint8_t> &memory, CPU &myCPU,
            const uint32_t &loopLocation, SDL_Renderer *renderer,
            SDL_Texture *texture, uint32_t *vram, uint32_t *pixels, bool &quit,
            bool &slug, GPU &myGPU, Memory &myMemory, int i,
            Instructions &instruction);

private:
  uint16_t PC;
  int16_t registers[32];
};

#endif
