#include <iostream>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    auto codes = parse_prog(data[0]);
    codes[1] = 12;
    codes[2] = 2;
    auto res = run_prog(codes, {});
    std::cout << codes[0] << std::endl;
}

void solve2(std::vector<std::string> data) {
    auto codes = parse_prog(data[0]);
    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            std::vector<int> ncodes(codes);
            ncodes[1] = noun;
            ncodes[2] = verb;
            auto res = run_prog(ncodes, {});
            if (ncodes[0] == 19690720) {
                std::cout << 100 * noun + verb << std::endl;
            }
        }
    }
}
