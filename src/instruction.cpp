#include "../include/instruction.hpp"
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include <iostream>
#include <unordered_map>

void LBU(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
         CPU &cpu, std::vector<uint8_t> &memory, Memory &myMemory) {
  unsigned int address = cpu.readRegister(reg_a) + immediate;
  if (address == 0x7100) {
    // Reading from stdin
    int input = std::cin.get(); // Extract 1 byte
    if (input == EOF) {
      cpu.writeRegister(reg_b, 0);
    } else {
      cpu.writeRegister(reg_b, static_cast<uint8_t>(input));
    }
  } else {
    cpu.writeRegister(reg_b, memory[address]);
  }

  cpu.setPC(cpu.getPC() + 4);
}

void BEQ(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
         CPU &cpu, Memory &myMemory) {
  if (cpu.readRegister(reg_a) == cpu.readRegister(reg_b)) {
    cpu.setPC(cpu.getPC() + 4 + (4 * (int16_t)immediate));
  } else {
    cpu.setPC(cpu.getPC() + 4);
  }
}

void ADDI(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
          CPU &cpu, Memory &myMemory) {
  cpu.writeRegister(reg_b, cpu.readRegister(reg_a) + (int16_t)immediate);
  cpu.setPC(cpu.getPC() + 4);
}

void JAL(unsigned int immediate, CPU &cpu, Memory &myMemory) {
  cpu.writeRegister(31, cpu.getPC() + 4);
  cpu.setPC(4 * immediate);
}
void J(unsigned int immediate, CPU &cpu, Memory &myMemory) {
  cpu.setPC(4 * immediate);
}

void BNE(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
         CPU &cpu, Memory &myMemory) {
  if (cpu.readRegister(reg_a) != cpu.readRegister(reg_b)) {
    cpu.setPC(cpu.getPC() + 4 + (4 * (int16_t)immediate));
  } else {
    cpu.setPC(cpu.getPC() + 4);
  }
}

void SB(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
        CPU &cpu, std::vector<uint8_t> &memory, Memory &myMemory) {
  immediate = (int16_t)immediate;
  myMemory.write8(cpu.readRegister(reg_a) + immediate,
                  cpu.readRegister(reg_b) & 0xFF);

  if ((cpu.readRegister(reg_a) + immediate) == 0x7110) {
    std::cout << ""
              << static_cast<char>(static_cast<int>(myMemory.read8(0x7110)));
  } else if ((cpu.readRegister(reg_a) + immediate) == 0x7120) {
    std::cerr << static_cast<char>(static_cast<int>(myMemory.read8(0x7120)));
  }
  cpu.setPC(cpu.getPC() + 4);
  if ((cpu.readRegister(reg_a) + immediate) >= 0x6100 &&
      ((cpu.readRegister(reg_a) + immediate)) <= 0x6fff) {
    myMemory.loadFlag = true;
  }
}

void SW(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
        CPU &cpu, std::vector<uint8_t> &memory, Memory &myMemory) {

  signed int address = cpu.readRegister(reg_a) + immediate;
  myMemory.write16(address,
                   static_cast<uint16_t>(cpu.readRegister(reg_b) & 0xFFFF));

  if ((cpu.readRegister(reg_a) + immediate) == 0x7110) {
    std::cout << "i"
              << static_cast<char>(static_cast<int>(myMemory.read8(0x7110)));
  } else if ((cpu.readRegister(reg_a) + immediate) == 0x7120) {
    std::cerr << static_cast<char>(static_cast<int>(myMemory.read8(0x7120)));
  }
  cpu.setPC(cpu.getPC() + 4);
  if (address >= 0x6100 && address <= 0x6fff) {
    myMemory.loadFlag = true;
  }
}

void LW(unsigned int reg_a, unsigned int reg_b, unsigned int immediate,
        CPU &cpu, std::vector<uint8_t> &memory, Memory &myMemory) {

  uint16_t address = cpu.readRegister(reg_a) + (uint32_t)immediate;
  cpu.writeRegister(reg_b, myMemory.read16((uint32_t)address));
  cpu.setPC(cpu.getPC() + 4);
}

void SLT(unsigned int reg_a, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c, (cpu.readRegister(reg_a) < cpu.readRegister(reg_b)));
  cpu.setPC(cpu.getPC() + 4);
}

void AND(unsigned int reg_a, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c, cpu.readRegister(reg_a) & cpu.readRegister(reg_b));
  cpu.setPC(cpu.getPC() + 4);
}

void SRL(unsigned int shift, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c, (uint16_t)cpu.readRegister(reg_b) >> shift);
  cpu.setPC(cpu.getPC() + 4);
}

void JR(unsigned int reg_a, CPU &cpu, Memory &myMemory) {

  cpu.setPC(cpu.readRegister(reg_a));
}

void SRA(unsigned int shift, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c, (int16_t)cpu.readRegister(reg_b) >> shift);
  cpu.setPC(cpu.getPC() + 4);
}

void NOR(unsigned int reg_a, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c,
                    ~(cpu.readRegister(reg_a) | cpu.readRegister(reg_b)));
  cpu.setPC(cpu.getPC() + 4);
}

void ADD(unsigned int reg_a, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c, cpu.readRegister(reg_a) + cpu.readRegister(reg_b));
  cpu.setPC(cpu.getPC() + 4);
}

void SUB(unsigned int reg_a, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(
      reg_c, (int16_t)(cpu.readRegister(reg_a) - cpu.readRegister(reg_b)));
  cpu.setPC(cpu.getPC() + 4);
}

void SLL(unsigned int shift, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
         Memory &myMemory) {

  cpu.writeRegister(reg_c, cpu.readRegister(reg_b) << shift);
  cpu.setPC(cpu.getPC() + 4);
}

void OR(unsigned int reg_a, unsigned int reg_b, unsigned int reg_c, CPU &cpu,
        Memory &myMemory) {

  cpu.writeRegister(reg_c, cpu.readRegister(reg_a) | cpu.readRegister(reg_b));
  cpu.setPC(cpu.getPC() + 4);
}

void operation(Instructions *Instruction, CPU &cpu,
               std::vector<uint8_t> &memory, Memory &myMemory) {
  int opcode = (*Instruction).opcode;
  int function = (*Instruction).function;
  switch (opcode) {
  case 0:
    LBU((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
        cpu, memory, myMemory);
    break;
  case 3:
    BEQ((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
        cpu, myMemory);
    break;
  case 13:
    ADDI((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
         cpu, myMemory);
    break;
  case 15:
    JAL((*Instruction).immediate, cpu, myMemory);
    break;
  case 27:
    J((*Instruction).immediate, cpu, myMemory);
    break;
  case 28:
    BNE((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
        cpu, myMemory);
    break;
  case 30:
    SB((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
       cpu, memory, myMemory);
    break;
  case 55:
    SW((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
       cpu, memory, myMemory);
    break;
  case 56:
    LW((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).immediate,
       cpu, memory, myMemory);
    break;
  case 49:
    switch (function) {
    case 0:
      SLT((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).reg_c, cpu,
          myMemory);
      break;
    case 7:
      AND((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).reg_c, cpu,
          myMemory);
      break;
    case 15:
      SRL((*Instruction).shift_value, (*Instruction).reg_b,
          (*Instruction).reg_c, cpu, myMemory);
      break;
    case 18:
      JR((*Instruction).reg_a, cpu, myMemory);
      break;
    case 20:
      SRA((*Instruction).shift_value, (*Instruction).reg_b,
          (*Instruction).reg_c, cpu, myMemory);
      break;
    case 23:
      NOR((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).reg_c, cpu,
          myMemory);
      break;
    case 25:
      ADD((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).reg_c, cpu,
          myMemory);
      break;
    case 26:
      SUB((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).reg_c, cpu,
          myMemory);
      break;
    case 44:
      SLL((*Instruction).shift_value, (*Instruction).reg_b,
          (*Instruction).reg_c, cpu, myMemory);
      break;
    case 51:
      OR((*Instruction).reg_a, (*Instruction).reg_b, (*Instruction).reg_c, cpu,
         myMemory);
      break;
    default:
      std::cerr << "Error: unknown function" << std::endl;
      break;
    }
    break;
  default:
    std::cerr << "Error: unknown opcode" << std::endl;
    break;
  }
}