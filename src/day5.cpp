#include <iostream>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    auto codes = parse_prog(data[0]);
    auto res = run_prog(codes, {1});
    std::cout << res.back() << std::endl;
}

void solve2(std::vector<std::string> data) {
    auto codes = parse_prog(data[0]);
    auto res = run_prog(codes, {5});
    std::cout << res.back() << std::endl;
}
