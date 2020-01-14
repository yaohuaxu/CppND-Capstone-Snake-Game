#include "a_star.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

AStar::AStar(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height) {
    board_.resize(grid_height);

    for (int i = 0; i < grid_height; i++) {
        board_[i].resize(grid_width);
        for (int j = 0; j < board_[i].size(); j++) {
            board_[i][j] = State::kEmpty;
            cout << static_cast<int>(board_[i][j]) << ", ";
        }
        cout << endl;
    }

    cout << "board_.size(): " << board_.size() << endl;
    cout << "board_[0].size(): " << board_[0].size() << endl;
/****search test****/
    SDL_Point start, end;
    start.x = 1;
    start.y = 1;
    end.x = 12;
    end.y = 12;
    vector<SDL_Point> SDL_Points = Search(start, end);

    cout << "SDL_Points.size(): " << SDL_Points.size() << endl;

    for (int i = 0; i < grid_height; i++) {
        for (int j = 0; j < board_[i].size(); j++) {
            if (State::kPath == board_[i][j]) {
                cout << "*, ";
            } else {
                cout << static_cast<int>(board_[i][j]) << ", ";
            }
        }
        cout << endl;
    }
/****search test****/

    cout << "AStar is Done" << endl;
}

AStar::~AStar() {
    cout << "~AStar is Done" << endl;
}

int AStar::Heuristic(const int start_point_x, const int start_point_y, const int end_point_x, const int end_point_y) {
    return abs(end_point_x - start_point_x) + abs(end_point_y - start_point_y);
}
void AStar::AddToOpenList(const int x, const int y, const int g_cost, const int h_cost,
                          vector<vector<int>> &open_list, vector<vector<State>> &board) {
    open_list.emplace_back(vector<int> { x, y, g_cost, h_cost });
    board[x][y] = State::kClosed;
}

bool AStar::IsVaildPoint(const int x, const int y, const vector<vector<State>> &board_) {
    const bool on_board_x = (x >= 0 && x < board_.size());
    const bool on_board_y = (y >= 0 && y < board_[0].size());

    if (on_board_x && on_board_y) {
        return board_[x][y] == State::kEmpty;
    } else {
        return false;
    }
}

void AStar::ExpandNeighbors(const vector<int> &current, const SDL_Point &end_point,
                            vector<vector<int>> &open_list, vector<vector<State>> &board_) {
    const int x = current[0];
    const int y = current[1];
    const int g_cost = current[2];

    for (int i = 0;  i < 4; i++) {
        const int x_new = x + delta_[i][0];
        const int y_new = y + delta_[i][1];

        if (IsVaildPoint(x_new, y_new, board_)) {
            const int g_new = g_cost + 1;
            const int h_new = Heuristic(x_new, y_new, end_point.x, end_point.y);
            AddToOpenList(x_new, y_new, g_new, h_new, open_list, board_);
        }
    }
}

vector<SDL_Point> AStar::Search(const SDL_Point &start_point, const SDL_Point &end_point) {
    vector<vector<int>> open_list{0};
    vector<SDL_Point> SDL_Points{};
    SDL_Point SDL_point;

    const int g_cost = 0;
    const int h_cost = Heuristic(start_point.x, start_point.y, end_point.x, end_point.y);
    AddToOpenList(start_point.x, start_point.y, g_cost, h_cost, open_list, board_);

    while (not open_list.empty()) {
        std::sort(open_list.begin(), open_list.end(), [](const vector<int> a, const vector<int> b){ return (a[2] + a[3]) > (b[2] + b[3]); });
        const auto current = open_list.back();
        open_list.pop_back();

        board_[current[0]][current[1]] = State::kPath;
        SDL_point.x = current[0];
        SDL_point.y = current[1];
        SDL_Points.emplace_back(SDL_point);

        if (current[0] == end_point.x && current[1] == end_point.y) {
            cout << "A* search OK!" << endl;
            return SDL_Points;
        }

        ExpandNeighbors(current, end_point, open_list, board_);
    }

    cout << "A* search FAILED!" << endl;

    return SDL_Points;
}