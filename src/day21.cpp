#include <iostream>
#include "intcode.hpp"

// Each time, we are jumping to the D tile, which needs to be ground.
// We jump whenever there is a hole in A, B, or C.
// In conclusion:
// J = (not A or not B or not C) and D
//   = not (A and B and C) and D
void solve1(std::vector<std::string> data) {
    Program prog(data[0]);
    std::string spring_code =
        "NOT T T\n"
        "AND A T\n"
        "AND B T\n"
        "AND C T\n"
        "NOT T J\n"
        "AND D J\n"
        "WALK\n";
    for (char c : spring_code) {
        prog.send_input(c);
    }
    prog.run();
    std::cout << prog.outputs.back() << std::endl;
}

// With additional range, we can fix the following issue:
// 01..4..7.9
// If we decide to jump at 0, then at 4, we won't be able to either jump or walk.
// To fix this, we need to ensure that either E or H is also ground when we jump.
// In conclusion:
// J = not (A and B and C) and D and (E or H)
void solve2(std::vector<std::string> data) {
    Program prog(data[0]);
    std::string spring_code =
        "NOT T T\n"
        "AND A T\n"
        "AND B T\n"
        "AND C T\n"
        "NOT T J\n"
        "AND D J\n"
        // Assumes T to be false. If T is true, then J is definitely false,
        // so it's not a problem.
        "OR E T\n"
        "OR H T\n"
        "AND T J\n"
        "RUN\n";
    for (char c : spring_code) {
        prog.send_input(c);
    }
    prog.run();
    std::cout << prog.outputs.back() << std::endl;
}
