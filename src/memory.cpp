#include "../include/memory.hpp"
#include "../include/cpu.hpp"
#include "../include/instruction.hpp"
#include <iostream>

Memory::Memory(std::vector<uint8_t> &buffer) : buffer(buffer) {}

uint8_t Memory::read8(uint32_t address) {
  if (address == 0x7100) {
    int input = std::cin.get(); // Extract 1 byte
    if (input == EOF) {
      return 0;
    } else {
      return static_cast<uint8_t>(input);
    }
  } else {
    return buffer[address];
  }
}

void Memory::write8(uint32_t address, uint8_t value) {
  if (address != 0x7100) {
    buffer[address] = value;
  }
}

uint16_t Memory::read16(uint32_t address) {
  uint16_t value = static_cast<uint16_t>(read8(address)) << 8;
  value |= read8(address + 1);
  return value;
}

void Memory::write16(uint32_t address, uint16_t value) {
  write8(address + 1, static_cast<uint8_t>(value & 0xFF));
  write8(address, static_cast<uint8_t>((value >> 8) & 0xFF));
}

bool Memory::extractAddresses(std::vector<uint8_t> &memory, size_t setupAddress,
                              size_t loopAddress, size_t loadAddress,
                              size_t programDataAddress, size_t dataSizeAddress,
                              uint32_t &setupLocation, uint32_t &loopLocation,
                              uint32_t &loadLocation, uint32_t &programLocation,
                              uint32_t &dataSize) {

  setupLocation = read32(memory, setupAddress);
  loopLocation = read32(memory, loopAddress);
  loadLocation = read32(memory, loadAddress);
  programLocation = read32(memory, programDataAddress);
  dataSize = read32(memory, dataSizeAddress);

  return true;
}

uint32_t Memory::read32(const std::vector<uint8_t> &memory, uint32_t address) {
  if (address + 3 >= memory.size()) {
    throw std::out_of_range("Error: Address out of bounds");
  }
  uint32_t value = 0;
  value |= static_cast<uint32_t>(memory[address]) << 24;
  value |= static_cast<uint32_t>(memory[address + 1]) << 16;
  value |= static_cast<uint32_t>(memory[address + 2]) << 8;
  value |= static_cast<uint32_t>(memory[address + 3]);
  return value;
}
