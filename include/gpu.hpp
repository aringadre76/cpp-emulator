#ifndef GPU_HPP
#define GPU_HPP

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>

class GPU {
public:
  // Constructor and destructor
  GPU();
  ~GPU();

  // Public methods
  bool initializeGraphics(std::vector<uint8_t> &memory);
  void processInput(SDL_Event &event, std::vector<uint8_t> &memory);
  void handleEvents(bool &quit, std::vector<uint8_t> &memory);
  void renderGraphics(uint32_t *vram, uint32_t *pixels);
  void shutdownGraphics();
  uint8_t readMemory(uint16_t address, const std::vector<uint8_t> &memory);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
};

struct ControllerState {
  uint8_t state;
  int grow;
  int shrink;
  int up;
  int down;
  int left;
  int right;
};

#endif // GPU_HPP
