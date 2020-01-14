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
    start.x = 0;
    start.y = 0;
    end.x = 2;
    end.y = 2;
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

bool AStar::IsVaildPoint(const int x, const int y, const vector<vector<State>> &board_) {
    const bool on_board_x = (x >= 0 && x < board_.size());
    const bool on_board_y = (y >= 0 && y < board_[0].size());

    if (on_board_x && on_board_y) {
        return board_[x][y] == State::kEmpty;
    } else {
        return false;
    }
}

void AStar::ExpandNeighbors(Node *current, const SDL_Point &end_point,
                            vector<Node> &open_list, vector<vector<State>> &board_) {
    Node node;

    for (int i = 0;  i < 4; i++) {
        const int x_new = current->x + delta_[i][0];
        const int y_new = current->y + delta_[i][1];
        cout << i << ", current->x: " << current->x << ", current->y: " << current->y << ", delta_[i][0]: " << delta_[i][0] << ", delta_[i][1]: " << delta_[i][1] << ", x_new: " << x_new << ", y_new: " << y_new << endl;
        if (IsVaildPoint(x_new, y_new, board_)) {
            node.parent = current;
            node.x = x_new;
            node.y = y_new;
            node.g_cost = current->g_cost + 1;
            node.h_cost = Heuristic(x_new, y_new, end_point.x, end_point.y);
            current->neighbors.push_back(node);
            cout << ", parent: " << current << endl;
        }
    }

    for (const auto &neighbor : current->neighbors) {
        open_list.push_back(neighbor);
        board_[neighbor.x][neighbor.y] = State::kClosed;
        // cout << "x: " << node.x << ", y: " << node.y << ", parent: " << current << ", neighbor: " << &neighbor << endl;
    }
}

vector<SDL_Point> AStar::Search(const SDL_Point &start_point, const SDL_Point &end_point) {
    vector<Node> open_list{};
    vector<SDL_Point> SDL_Points{};
    SDL_Point SDL_point;

    Node node;
    node.parent = nullptr;
    node.x = start_point.x;
    node.y = start_point.y;
    node.g_cost = 0;
    node.h_cost = Heuristic(start_point.x, start_point.y, end_point.x, end_point.y);
    open_list.push_back(node);

    while (not open_list.empty()) {
        std::sort(open_list.begin(), open_list.end(), [](const Node &a, const Node &b){ return (a.g_cost + a.h_cost) > (b.g_cost + b.h_cost); });
        Node *current = &open_list.back();
        open_list.pop_back();

        board_[current->x][current->y] = State::kPath;

        if (current->x == end_point.x && current->y == end_point.y) {
            Node *node = current;
                            cout << "....x: " << node->x << ", y: " << node->y << ", parent: " << node->parent << endl;
            // while (nullptr != node) {
            //     SDL_point.x = node->x;
            //     SDL_point.y = node->y;
            //     SDL_Points.push_back(SDL_point);

            //     cout << "x: " << node->x << ", y: " << node->y << ", parent: " << node->parent << endl;
            //     node = node->parent;
            // }

            // std::reverse(SDL_Points.begin(), SDL_Points.end());
            cout << "A* search OK!" << endl;

            return SDL_Points;
        }

        ExpandNeighbors(current, end_point, open_list, board_);
    }

    cout << "A* search FAILED!" << endl;

    return SDL_Points;
}