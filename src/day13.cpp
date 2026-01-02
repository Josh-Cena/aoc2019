#include <iostream>
#include <thread>
#include <cstdlib>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    Program prog(data[0]);
    prog.run();
    int block_count = 0;
    std::vector<long long> outputs = prog.pop_all_output();
    for (int i = 2; i < outputs.size(); i += 3) {
        long long tile_id = outputs[i];
        if (tile_id == 2) {
            block_count++;
        }
    }
    std::cout << block_count << std::endl;
}

void print_screen(const std::map<std::pair<int, int>, int> &screen) {
    std::cout << "\033[2J\033[H";
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;
    int score = -1;
    for (const auto &entry : screen) {
        int x = entry.first.first;
        int y = entry.first.second;
        if (x == -1 && y == 0) {
            score = entry.second;
            continue;
        }
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }
    std::cout << "Score: " << score << std::endl;
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            auto it = screen.find({x, y});
            int tile_id = (it != screen.end()) ? it->second : 0;
            char c = ' ';
            if (tile_id == 1) c = '#';
            else if (tile_id == 2) c = '*';
            else if (tile_id == 3) c = '-';
            else if (tile_id == 4) c = 'O';
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void solve2(std::vector<std::string> data) {
    Program prog(data[0]);
    prog.memory[0] = 2;
    std::map<std::pair<int, int>, int> screen;
    bool visualize = std::getenv("VISUALIZE") != nullptr;
    while (true) {
        prog.run_until_input();
        int ball_pos;
        int paddle_pos;
        int score;
        std::vector<long long> outputs = prog.pop_all_output();
        for (int i = 0; i < outputs.size(); i += 3) {
            long long x = outputs[i];
            long long y = outputs[i + 1];
            long long tile_id = outputs[i + 2];
            screen[{(int)x, (int)y}] = (int)tile_id;
            if (tile_id == 3) {
                paddle_pos = (int)x;
            } else if (tile_id == 4) {
                ball_pos = (int)x;
            } else if (x == -1 && y == 0) {
                score = (int)tile_id;
            }
        }
        if (visualize) {
            print_screen(screen);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        if (prog.halted) {
            std::cout << score << std::endl;
            break;
        }
        long long input;
        if (ball_pos < paddle_pos) {
            input = -1;
        } else if (ball_pos > paddle_pos) {
            input = 1;
        } else {
            input = 0;
        }
        prog.send_input(input);
        prog.step();
    }
}
