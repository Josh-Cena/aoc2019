#include <iostream>
#include <thread>
#include <map>
#include <queue>
#include <random>
#include "intcode.hpp"

void print_map(const std::map<std::pair<int, int>, int> &map, std::pair<int, int> droid_pos) {
    std::cout << "\x1b[2J\x1b[1;1H";
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;
    for (const auto &entry : map) {
        int x = entry.first.first;
        int y = entry.first.second;
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            auto it = map.find({x, y});
            char c = '?';
            if (it != map.end()) {
                int val = it->second;
                if (val == 0) c = '#';
                else if (val == 1) c = '.';
                else if (val == 2) c = 'O';
            }
            if (droid_pos.first == x && droid_pos.second == y) {
                c = 'D';
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int opposite_dir(int dir) {
    if (dir == 1) return 2;
    if (dir == 2) return 1;
    if (dir == 3) return 4;
    if (dir == 4) return 3;
    throw std::invalid_argument("Invalid direction");
}

std::pair<int, int> move_in_dir(std::pair<int, int> pos, int dir) {
    if (dir == 1) return {pos.first, pos.second - 1};
    if (dir == 2) return {pos.first, pos.second + 1};
    if (dir == 3) return {pos.first - 1, pos.second};
    if (dir == 4) return {pos.first + 1, pos.second};
    throw std::invalid_argument("Invalid direction");
}

std::map<std::pair<int, int>, int> distance_from_oxygen(std::vector<std::string> data) {
    Program prog(data[0]);
    std::map<std::pair<int, int>, int> map;
    std::pair<int, int> pos = {0, 0};
    bool visualize = std::getenv("VISUALIZE") != nullptr;
    std::vector<int> directions;
    std::pair<int, int> oxygen_pos;
    while (!prog.halted) {
        prog.run_until_input();
        int direction = -1;
        // Try to find an unexplored direction
        for (int dir = 1; dir <= 4; dir++) {
            std::pair<int, int> npos = move_in_dir(pos, dir);
            if (map.find(npos) == map.end()) {
                direction = dir;
                break;
            }
        }
        // If all directions explored, backtrack
        if (direction == -1) {
            if (directions.empty()) {
                break; // Explored entire area
            }
            int last_dir = directions.back();
            directions.pop_back();
            direction = opposite_dir(last_dir);
            prog.send_input(direction);
            prog.run_until_output();
            prog.outputs.pop(); // Already know what it is
            pos = move_in_dir(pos, direction);
        } else {
            prog.send_input(direction);
            prog.run_until_output();
            long long status = prog.outputs.front(); prog.outputs.pop();
            std::pair<int, int> npos = move_in_dir(pos, direction);
            if (status == 2) {
                pos = npos;
                map[npos] = 2;
                oxygen_pos = npos;
                directions.push_back(direction);
            } else if (status == 1) {
                pos = npos;
                map[npos] = 1;
                directions.push_back(direction);
            } else if (status == 0) {
                map[npos] = 0;
            }
        }
        if (visualize) {
            print_map(map, pos);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    std::queue<std::pair<std::pair<int, int>, int>> q;
    std::map<std::pair<int, int>, int> distances;
    q.push({oxygen_pos, 0});
    distances[oxygen_pos] = 0;
    while (!q.empty()) {
        auto front = q.front(); q.pop();
        auto cpos = front.first;
        int dist = front.second;
        for (int dir = 1; dir <= 4; dir++) {
            auto npos = move_in_dir(cpos, dir);
            if (map[npos] != 0 && distances.find(npos) == distances.end()) {
                distances[npos] = dist + 1;
                q.push({npos, dist + 1});
            }
        }
    }
    return distances;
}

void solve1(std::vector<std::string> data) {
    auto distances = distance_from_oxygen(data);
    std::cout << distances[{0, 0}] << std::endl;
}

void solve2(std::vector<std::string> data) {
    auto distances = distance_from_oxygen(data);
    int max_dist = 0;
    for (const auto &entry : distances) {
        if (entry.second > max_dist) {
            max_dist = entry.second;
        }
    }
    std::cout << max_dist << std::endl;
}
