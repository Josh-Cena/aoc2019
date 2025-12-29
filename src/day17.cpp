#include <iostream>
#include <queue>
#include <cassert>
#include "intcode.hpp"

std::vector<std::vector<char>> get_map(const std::string &line) {
    Program prog(line);
    prog.run();
    std::vector<std::vector<char>> map = {{}};
    while (prog.outputs.size() > 0) {
        char ch = prog.outputs.front();
        prog.outputs.pop();
        if (ch == '\n') {
            map.push_back({});
        } else {
            map.back().push_back(ch);
        }
    }
    // Remove TWO trailing new lines
    map.pop_back();
    map.pop_back();
    return map;
}

void solve1(std::vector<std::string> data) {
    auto map = get_map(data[0]);
    bool visualize = std::getenv("VISUALIZE") != nullptr;
    if (visualize) {
        for (int r = 0; r < map.size(); r++) {
            for (int c = 0; c < map[r].size(); c++) {
                std::cout << map[r][c];
            }
            std::cout << std::endl;
        }
    }
    int total = 0;
    for (int r = 1; r < map.size() - 1; r++) {
        for (int c = 1; c < map[r].size() - 1; c++) {
            if (
                map[r][c] == '#' &&
                map[r-1][c] == '#' &&
                map[r+1][c] == '#' &&
                map[r][c-1] == '#' &&
                map[r][c+1] == '#'
            ) {
                total += r * c;
            }
        }
    }
    std::cout << total << std::endl;
}

std::vector<std::string> split(const std::vector<std::string> &data, const std::string &delim) {
    std::vector<std::string> result;
    for (const auto &line : data) {
        size_t start = 0;
        size_t end = line.find(delim);
        while (end != std::string::npos) {
            if (end != start) {
                result.push_back(line.substr(start, end - start));
            }
            start = end + delim.length();
            end = line.find(delim, start);
        }
        if (start >= line.length()) continue;
        result.push_back(line.substr(start, end));
    }
    return result;
}

std::tuple<std::string, std::string, std::string> find_functions(const std::string &path) {
    for (int lenA = 1; lenA <= 21; lenA++) {
        std::string funcA = path.substr(0, lenA);
        if (funcA.back() != ',') continue;
        auto aReplaced = split({path}, funcA);
        for (int lenB = 1; lenB <= 21; lenB++) {
            if (lenB > aReplaced[0].length()) break;
            std::string funcB = aReplaced[0].substr(0, lenB);
            if (funcB.back() != ',') continue;
            auto bReplaced = split(aReplaced, funcB);
            std::string funcC = bReplaced[0];
            if (funcC.length() > 21) continue;
            auto cReplaced = split(bReplaced, funcC);
            if (cReplaced.size() > 0) continue;
            return {funcA, funcB, funcC};
        }
    }
    throw std::invalid_argument("No valid functions found");
}

void solve2(std::vector<std::string> data) {
    auto map = get_map(data[0]);
    int x = 0, y = 0;
    int dir_x = 0, dir_y = -1;
    for (int r = 0; r < map.size(); r++) {
        for (int c = 0; c < map[r].size(); c++) {
            if (map[r][c] == '^') {
                x = c;
                y = r;
                break;
            }
        }
    }
    int w = map[0].size();
    int h = map.size();
    std::string path;
    while (true) {
        int steps = 0;
        while (
            y + dir_y < h &&
            x + dir_x < w &&
            y + dir_y >= 0 &&
            x + dir_x >= 0 &&
            map[y + dir_y][x + dir_x] == '#'
        ) {
            x += dir_x;
            y += dir_y;
            steps++;
        }
        if (steps > 0) {
            path += std::to_string(steps) + ",";
        }
        bool turned = false;
        for (int turn = -1; turn <= 1; turn += 2) {
            int new_dir_x = dir_y * turn;
            int new_dir_y = dir_x * -turn;
            if (
                y + new_dir_y < h &&
                x + new_dir_x < w &&
                y + new_dir_y >= 0 &&
                x + new_dir_x >= 0 &&
                map[y + new_dir_y][x + new_dir_x] == '#'
            ) {
                path += (turn == -1 ? "R," : "L,");
                dir_x = new_dir_x;
                dir_y = new_dir_y;
                turned = true;
                break;
            }
        }
        // Walked the whole path
        if (!turned) {
            break;
        }
    }
    auto [funcA, funcB, funcC] = find_functions(path);
    std::string main_routine;
    std::string temp = path;
    while (temp.length() > 0) {
        if (temp.substr(0, funcA.length()) == funcA) {
            main_routine += "A,";
            temp = temp.substr(funcA.length());
        } else if (temp.substr(0, funcB.length()) == funcB) {
            main_routine += "B,";
            temp = temp.substr(funcB.length());
        } else if (temp.substr(0, funcC.length()) == funcC) {
            main_routine += "C,";
            temp = temp.substr(funcC.length());
        } else {
            assert(false);
        }
    }
    main_routine.pop_back();
    funcA.pop_back();
    funcB.pop_back();
    funcC.pop_back();
    Program prog(data[0]);
    prog.memory[0] = 2;
    for (char ch : main_routine) {
        prog.send_input((long long)ch);
    }
    prog.send_input((long long)'\n');
    for (char ch : funcA) {
        prog.send_input((long long)ch);
    }
    prog.send_input((long long)'\n');
    for (char ch : funcB) {
        prog.send_input((long long)ch);
    }
    prog.send_input((long long)'\n');
    for (char ch : funcC) {
        prog.send_input((long long)ch);
    }
    prog.send_input((long long)'\n');
    prog.send_input((long long)'n');
    prog.send_input((long long)'\n');
    prog.run();
    std::cout << prog.outputs.back() << std::endl;
}
