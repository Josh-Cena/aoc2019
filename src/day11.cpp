#include <iostream>
#include <map>
#include "intcode.hpp"

void print_screen(const std::map<std::pair<int, int>, int> &panels) {
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;
    for (const auto &entry : panels) {
        int x = entry.first.first;
        int y = entry.first.second;
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            auto it = panels.find({x, y});
            char c = it != panels.end() && it->second == 1 ? '#' : ' ';
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void paint(std::map<std::pair<int, int>, int> &panels, std::string line) {
    Program prog(line);
    int x = 0, y = 0;
    int dir_x = 0, dir_y = -1;
    while (!prog.halted) {
        prog.send_input(panels.find({x, y}) != panels.end() ? panels[{x, y}] : 0);
        prog.run_until_output();
        if (prog.halted) break;
        panels[{x, y}] = prog.pop_output();
        prog.run_until_output();
        int turn = prog.pop_output();
        if (turn == 0) {
            int tmp = dir_x;
            dir_x = dir_y;
            dir_y = -tmp;
        } else {
            int tmp = dir_x;
            dir_x = -dir_y;
            dir_y = tmp;
        }
        x += dir_x;
        y += dir_y;
    }
}

void solve1(std::vector<std::string> data) {
    std::map<std::pair<int, int>, int> panels;
    paint(panels, data[0]);
    std::cout << panels.size() << std::endl;
}

void solve2(std::vector<std::string> data) {
    std::map<std::pair<int, int>, int> panels;
    panels[{0, 0}] = 1;
    paint(panels, data[0]);
    print_screen(panels);
}
