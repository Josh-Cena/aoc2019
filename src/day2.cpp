#include <iostream>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    Program prog(data[0]);
    prog.codes[1] = 12;
    prog.codes[2] = 2;
    prog.run();
    std::cout << prog.codes[0] << std::endl;
}

void solve2(std::vector<std::string> data) {
    Program base_prog(data[0]);
    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            Program prog(base_prog);
            prog.codes[1] = noun;
            prog.codes[2] = verb;
            prog.run();
            if (prog.codes[0] == 19690720) {
                std::cout << 100 * noun + verb << std::endl;
            }
        }
    }
}
