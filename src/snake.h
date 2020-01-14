#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width_(grid_width),
        grid_height_(grid_height),
        head_x_(grid_width / 2),
        head_y_(grid_height / 2) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  void SetDirection(const Direction direction) { direction_ = direction; };
  void SetSpeed(const float speed) { speed_ = speed; };

  Direction GetDirection() const { return direction_; };
  float GetSpeed() const { return speed_; };
  int GetSize() const { return size_; };
  bool GetAlive() const { return alive_; };
  float GetHeadX() const { return head_x_; };
  float GetHeadY() const { return head_y_; };
  std::vector<SDL_Point> GetBody() const { return body_; };

 private:
  bool growing_{false};
  int grid_width_;
  int grid_height_;
  float speed_{0.0f};
  int size_{1};
  bool alive_{true};
  float head_x_;
  float head_y_;
  std::vector<SDL_Point> body_;
  Direction direction_ = Direction::kUp;

  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
};

#endif