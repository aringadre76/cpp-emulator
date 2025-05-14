#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstddef> // For size_t
#include <iostream>
#include <stdint.h>

#define STOP_FLAG 5
#define VRAM_START 0x6100
#define VRAM_SIZE 0xF00
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 60

#define CONTROLLER_A_MASK ((uint8_t)0x80)
#define CONTROLLER_B_MASK ((uint8_t)0x40)
#define CONTROLLER_SELECT_MASK ((uint8_t)0x20)
#define CONTROLLER_START_MASK ((uint8_t)0x10)
#define CONTROLLER_UP_MASK ((uint8_t)0x08)
#define CONTROLLER_DOWN_MASK ((uint8_t)0x04)
#define CONTROLLER_LEFT_MASK ((uint8_t)0x02)
#define CONTROLLER_RIGHT_MASK ((uint8_t)0x01)

constexpr size_t setupAddress = 0x81e0;
constexpr size_t loopAddress = 0x81e4;
constexpr size_t loadAddress = 0x81e8;
constexpr size_t programDataAddress = 0x81ec;
constexpr size_t dataSizeAddress = 0x81f0;

#endif // CONSTANTS_HPP