#include <iostream>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    Program prog(data[0]);
    prog.send_input(1);
    prog.run();
    std::cout << prog.pop_output() << std::endl;
}

void solve2(std::vector<std::string> data) {
    Program prog(data[0]);
    prog.send_input(2);
    prog.run();
    std::cout << prog.pop_output() << std::endl;
}
