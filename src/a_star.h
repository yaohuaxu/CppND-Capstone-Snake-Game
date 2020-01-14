#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include "SDL.h"

using std::vector;

enum class State {
    kEmpty = 0,
    kObstacle = 1,
    kClosed = 2,
    kPath = 3
};

class AStar {
public:
    AStar(const std::size_t screen_width, const std::size_t screen_height,
          const std::size_t grid_width, const std::size_t grid_height);
    ~AStar();
    vector<SDL_Point> Search(const SDL_Point &start_point, const SDL_Point &end_point);
private:
    vector<vector<State>> board_;
    int delta_[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
    int Heuristic(const int start_point_x, const int start_point_y, const int end_point_x, const int end_point_y);
    void AddToOpenList(const int x, const int y, const int g_cost, const int h_cost,
                       vector<vector<int>> &open_list, vector<vector<State>> &board);
    void ExpandNeighbors(const vector<int> &current, const SDL_Point &end_point,
                         vector<vector<int>> &open_list, vector<vector<State>> &board_);
    bool IsVaildPoint(const int x, const int y, const vector<vector<State>> &board_);
};

#endif