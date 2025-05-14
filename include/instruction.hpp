#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include "cpu.hpp"
#include "memory.hpp"
#include <string>
#include <vector>

class CPU;

struct Instructions {
  unsigned int opcode = 0;
  unsigned int reg_a = 0;
  unsigned int reg_b = 0;
  unsigned int reg_c = 0;
  unsigned int shift_value = 0;
  unsigned int function = 0;
  unsigned int immediate = 0;
};

extern std::vector<Instructions> totalInstructions;
void operation(Instructions *Instruction, CPU &cpu,
               std::vector<uint8_t> &buffer, Memory &myMemory);
void JAL(unsigned int immediate, CPU &cpu, Memory &myMemory);
#endif