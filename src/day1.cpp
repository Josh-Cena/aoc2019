#include <iostream>

void solve1(std::vector<std::string> data) {
    int total = 0;
    for (auto line : data) {
        int mass = std::stoi(line);
        int fuel = mass / 3 - 2;
        total += fuel;
    }
    std::cout << total << std::endl;
}

void solve2(std::vector<std::string> data) {
    int total = 0;
    for (auto line : data) {
        int mass = std::stoi(line);
        int fuel = mass / 3 - 2;
        while (fuel > 0) {
            total += fuel;
            fuel = fuel / 3 - 2;
        }
    }
    std::cout << total << std::endl;
}
