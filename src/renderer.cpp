#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      grid_width_(grid_width),
      grid_height_(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window_ = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window_) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer_) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window_);
  SDL_Quit();
}

void Renderer::Render(Snake &snake, const SDL_Point &food, const std::vector<SDL_Point> &path) {
  SDL_Rect block;
  block.w = screen_width_ / grid_width_;
  block.h = screen_height_ / grid_height_;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer_, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer_);

  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

  for (int i = 0; i < grid_height_ + 1; i++) {
      SDL_RenderDrawLine(sdl_renderer_, 0, i * block.h, screen_width_, i * block.h);
  }

  for (int i = 0; i < screen_width_ + 1; i++) {
      SDL_RenderDrawLine(sdl_renderer_, i * block.w, 0, i * block.w, screen_height_);
  }

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.GetBody()) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer_, &block);
  }

  // Render snake's A* path
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x4F, 0xFF, 0x4F);
  for (SDL_Point const &point : path) {
    block.x = point.y * block.w;
    block.y = point.x * block.h;
    SDL_RenderFillRect(sdl_renderer_, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.GetHeadX()) * block.w;
  block.y = static_cast<int>(snake.GetHeadY()) * block.h;
  if (snake.GetAlive()) {
    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window_, title.c_str());
}
