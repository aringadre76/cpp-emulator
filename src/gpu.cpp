#include "../include/gpu.hpp"
#include "../include/constants.hpp"
#include <iomanip>
#include <iostream>
#include <vector>

// Constructor
GPU::GPU() : window(nullptr), renderer(nullptr), texture(nullptr) {}

// Destructor
GPU::~GPU() { shutdownGraphics(); }

bool GPU::initializeGraphics(std::vector<uint8_t> &memory) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Error: SDL could not initialize! SDL_Error: "
              << SDL_GetError() << std::endl;
    return false;
  }

  // We scale up by 10 so it's easier to see
  window = SDL_CreateWindow("BananaSlug Emulator", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 10,
                            SCREEN_HEIGHT * 10, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "Error: Window could not be created! SDL_Error: "
              << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "Error: Renderer could not be created! SDL_Error: "
              << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                              SCREEN_HEIGHT);
  if (!texture) {
    std::cerr << "Error: Texture could not be created! SDL_Error: "
              << SDL_GetError() << std::endl;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  return true;
}

ControllerState controller;

void GPU::processInput(SDL_Event &event, std::vector<uint8_t> &memory) {
  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
    bool isPressed = (event.type == SDL_KEYDOWN);

    switch (event.key.keysym.sym) {
    case SDLK_a:
      if (isPressed) {
        controller.state |= CONTROLLER_A_MASK;
        controller.grow += 5;
      } else {
        controller.state &= ~CONTROLLER_A_MASK;
      }
      break;
    case SDLK_b:
      if (isPressed) {
        controller.state |= CONTROLLER_B_MASK;
        controller.shrink += 5;
      } else {
        controller.state &= ~CONTROLLER_B_MASK;
      }
      break;
    case SDLK_s:
      if (isPressed) {
        controller.state |= CONTROLLER_SELECT_MASK;
        controller.grow = 0;
        controller.shrink = 0;
      } else {
        controller.state &= ~CONTROLLER_SELECT_MASK;
      }
      break;
    case SDLK_RETURN:
      if (isPressed) {
        controller.state |= CONTROLLER_START_MASK;
        controller.up = 0;
        controller.down = 0;
        controller.left = 0;
        controller.right = 0;
      } else {
        controller.state &= ~CONTROLLER_START_MASK;
      }
      break;
    case SDLK_UP:
      if (isPressed) {
        controller.state |= CONTROLLER_UP_MASK;
        controller.up += 1;
      } else {
        controller.state &= ~CONTROLLER_UP_MASK;
      }
      break;
    case SDLK_DOWN:
      if (isPressed) {
        controller.state |= CONTROLLER_DOWN_MASK;
        controller.down += 1;
      } else {
        controller.state &= ~CONTROLLER_DOWN_MASK;
      }
      break;
    case SDLK_LEFT:
      if (isPressed) {
        controller.state |= CONTROLLER_LEFT_MASK;
        controller.left += 1;
      } else {
        controller.state &= ~CONTROLLER_LEFT_MASK;
      }
      break;
    case SDLK_RIGHT:
      if (isPressed) {
        controller.state |= CONTROLLER_RIGHT_MASK;
        controller.right += 1;
      } else {
        controller.state &= ~CONTROLLER_RIGHT_MASK;
      }
      break;
    default:
      break;
    }
    memory[0x7000] = controller.state;
    // Bottom line used for debugging
    // std::cout << "Button Pressed and now 0x7000 has: " << std::hex <<
    // std::setw(2) << std::setfill('0') << static_cast<int>(memory[0x7000]) <<
    // std::endl;
  }
}

void GPU::handleEvents(bool &quit, std::vector<uint8_t> &memory) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = true;
    } else {
      processInput(event, memory);
    }
  }
}

// void translateVRAMToPixels(uint32_t *vram, uint32_t *pixels) {
//   //uint32_t vram, uint32_tpixels) {
//   // for (int y = 0; y < SCREEN_HEIGHT; ++y) {
//   //   for (int x = 0; x < SCREEN_WIDTH; ++x) {
//   //     int vram_index = y * SCREEN_WIDTH + x;
//   //     int pixel_index = y * SCREEN_WIDTH + x;
//   //     pixels[pixel_index] = vram[vram_index] ? 0xFFFFFFFF : 0x00000000;
//   //   }
//   // }
//   int total_pixels = SCREEN_WIDTH * SCREEN_HEIGHT;
//   for (int i = 0; i < total_pixels; ++i) {
//     pixels[i] = vram[i] ? 0xFFFFFFFF : 0x00000000;
//   }
// }

void GPU::renderGraphics(uint32_t *vram, uint32_t *pixels) {
  bool vram_changed = false;
  int total_pixels = SCREEN_WIDTH * SCREEN_HEIGHT;

  for (int i = 0; i < total_pixels; ++i) {
    uint32_t new_pixel = vram[i] ? 0xFFFFFFFF : 0x00000000;
    if (pixels[i] != new_pixel) {
      pixels[i] = new_pixel;
      vram_changed = true;
    }
  }

  if (vram_changed) {
    SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
}

void GPU::shutdownGraphics() {
  if (texture) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  SDL_Quit();
}

uint8_t GPU::readMemory(uint16_t address, const std::vector<uint8_t> &memory) {
  if (address >= VRAM_START && address <= VRAM_START + VRAM_SIZE - 1) {
    return memory[address];
  } else {
    std::cerr << "Error: Invalid VRAM address access: " << std::hex << address
              << std::endl;
    return 0;
  }
}
