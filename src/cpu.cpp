#include "../include/cpu.hpp"
#include "../include/gpu.hpp"
#include "../include/memory.hpp"
#include <SDL2/SDL.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

CPU::CPU() {
  PC = 0x0000; // Start PC at 0x0000 as per the documentation
  zero_register = 0;

  stack_pointer_register = 0x6100;

  for (int i = 0; i < 32; ++i) {
    registers[i] = 0;
  }

  // Initialize Stack Pointer Register to 0x6100
  registers[29] = 0x6100;
}

CPU::~CPU() {
  // Clean up any resources if needed
}

int16_t CPU::readRegister(int registerNum) { return registers[registerNum]; }

void CPU::writeRegister(int registerNum, int16_t registerVal) {
  registers[registerNum] = registerVal;
}

uint16_t CPU::getPC() { return PC; }

void CPU::setPC(uint16_t newPC) { PC = newPC; }
bool CPU::extractInstructions(const std::vector<uint8_t> &memory,
                              Instructions &instruction) {
  uint32_t pc = CPU::PC;
  uint32_t data = (memory[pc] << 24) | (memory[pc + 1] << 16) |
                  (memory[pc + 2] << 8) | memory[pc + 3];

  instruction.opcode = (data >> 26) & 0x3F;

  if (instruction.opcode != 49) {
    instruction.reg_a = (data >> 21) & 0x1F;
    instruction.reg_b = (data >> 16) & 0x1F;
    instruction.immediate = data & 0xFFFF;
  } else {
    instruction.reg_a = (data >> 21) & 0x1F;
    instruction.reg_b = (data >> 16) & 0x1F;
    instruction.reg_c = (data >> 11) & 0x1F;
    instruction.shift_value = (data >> 6) & 0x1F;
    instruction.function = data & 0x3F;
  }

  return true;
}

void printVRAM(std::vector<uint8_t> &memory) {
  const uint16_t start = 0x6100;
  const uint16_t endd = 0x6FFF;
  const int bytesPerLine = 16;
  const int groupSize = 4;

  for (uint16_t address = start; address <= endd; ++address) {
    if ((address - start) % bytesPerLine == 0) {
      std::cout << std::setw(4) << std::setfill('0') << std::hex << address
                << ": ";
    }

    std::cout << std::setw(2) << std::setfill('0') << std::hex
              << (int)memory[address];

    if ((address - start + 1) % groupSize == 0) {
      std::cout << ' ';
    }

    if ((address - start + 1) % bytesPerLine == 0) {
      std::cout << std::endl;
    }
  }

  // If the last line isn't complete, print a new line
  if ((endd - start + 1) % bytesPerLine != 0) {
    std::cout << std::endl;
  }
  std::cout << std::endl << std::endl << std::endl << std::endl;
}

bool CPU::setup(std::vector<uint8_t> &memory, CPU &myCPU,
                const uint32_t &setupLocation, Memory &myMemory) {

  myCPU.PC = 0xfffc;
  JAL(setupLocation / 4, myCPU, myMemory);

  while (myCPU.PC >= 0x8000) {
    Instructions instruction;
    myCPU.extractInstructions(memory, instruction);

    operation(&instruction, myCPU, memory, myMemory);

    if (myCPU.PC == 0x0000) {
      break;
    }
  }

  myCPU.PC = 0x0000;

  return true;
}

void CPU::temp(std::vector<uint8_t> &memory, CPU &myCPU,
               const uint32_t &loopLocation, SDL_Renderer *renderer,
               SDL_Texture *texture, uint32_t *vram, uint32_t *pixels,
               bool &quit, bool &slug, GPU &myGPU, Memory &myMemory, int i,
               Instructions &instruction) {

  i++;

  myCPU.extractInstructions(memory, instruction);

  operation(&instruction, myCPU, memory, myMemory);

  if (slug == false) {
    myGPU.handleEvents(quit, memory);

    if (myMemory.loadFlag == true) {
      myMemory.loadFlag = false;
      for (int i = 0; i < VRAM_SIZE; ++i) {
        vram[i] = myGPU.readMemory(VRAM_START + i, memory);
      }
      myGPU.renderGraphics(vram, pixels);
    }
  }

  if (myCPU.PC == 0x0000) {
    myCPU.PC = 0xfffc;
    JAL(loopLocation / 4, myCPU, myMemory);
    return;
  }
}

bool CPU::loop(std::vector<uint8_t> &memory, CPU &myCPU,
               const uint32_t &loopLocation, SDL_Renderer *renderer,
               SDL_Texture *texture, uint32_t *vram, uint32_t *pixels,
               bool &quit, bool &slug, GPU &myGPU, Memory &myMemory) {

  myCPU.PC = 0xfffc;
  JAL(loopLocation / 4, myCPU, myMemory);

  int i = 0;
  Instructions instruction;
  while (!quit && memory[0x7200] == STOP_FLAG) {
    temp(memory, myCPU, loopLocation, renderer, texture, vram, pixels, quit,
         slug, myGPU, myMemory, i, instruction);
    // i++;

    // myCPU.extractInstructions(memory, instruction);

    // operation(&instruction, myCPU, memory, myMemory);

    // if (slug == false) {
    //   myGPU.handleEvents(quit, memory);

    //     if (myMemory.loadFlag == true) {
    //       myMemory.loadFlag = false;
    //       for (int i = 0; i < VRAM_SIZE; ++i) {
    //         vram[i] = myGPU.readMemory(VRAM_START + i, memory);
    //       }
    //       myGPU.renderGraphics(vram, pixels);
    //     }
    // }

    // if (myCPU.PC == 0x0000) {
    //   myCPU.PC = 0xfffc;
    //   JAL(loopLocation / 4, myCPU, myMemory);
    //   continue;
    // }
  } // end of while loop

  myCPU.PC = 0x0000;
  return true;
}
