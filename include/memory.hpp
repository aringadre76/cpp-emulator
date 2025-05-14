#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <vector>

class Memory {
public:
  Memory(std::vector<uint8_t> &buffer);
  uint8_t read8(uint32_t address);
  void write8(uint32_t address, uint8_t value);
  uint16_t read16(uint32_t address);
  void write16(uint32_t address, uint16_t value);

  bool extractAddresses(std::vector<uint8_t> &memory, size_t setupAddress,
                        size_t loopAddress, size_t loadAddress,
                        size_t programDataAddress, size_t dataSizeAddress,
                        uint32_t &setupLocation, uint32_t &loopLocation,
                        uint32_t &loadLocation, uint32_t &programLocation,
                        uint32_t &dataSize);

  uint32_t read32(const std::vector<uint8_t> &memory, uint32_t address);

  std::vector<uint8_t> &buffer;

  bool loadFlag = true;
};

#endif