#include <iostream>
#include <algorithm>
#include "intcode.hpp"

bool has_beam(const Program &base_prog, int x, int y) {
    Program prog(base_prog);
    prog.send_input(x);
    prog.send_input(y);
    prog.run();
    return prog.outputs.front() == 1;
}

void solve1(std::vector<std::string> data) {
    Program prog(data[0]);
    int total = 0;
    bool visualize = std::getenv("VISUALIZE") != nullptr;
    for (int x = 0; x < 50; x++) {
        for (int y = 0; y < 50; y++) {
            bool output = has_beam(prog, x, y);
            total += output;
            if (visualize) {
                std::cout << (output ? '#' : '.');
            }
        }
        if (visualize) {
            std::cout << std::endl;
        }
    }
    std::cout << total << std::endl;
}

void solve2(std::vector<std::string> data) {
    Program prog(data[0]);
    // We know that the beam is enclosed by two lines from the origin.
    // We first try to fit these two lines.
    // Start from x=10 since the beam is too narrow before that.
    double top_slope = 0.0;
    double bottom_slope = INFINITY;
    for (int x = 10; x < 50; x++) {
        int low = 50;
        int high = 50;
        for (int y = 0; y < 50; y++) {
            if (has_beam(prog, x, y) && !has_beam(prog, x, y - 1)) {
                low = y;
            }
            if (!has_beam(prog, x, y) && has_beam(prog, x, y - 1)) {
                high = y - 1;
            }
        }
        if (high > low) {
            double top_ratio = static_cast<double>(high) / x;
            double bottom_ratio = static_cast<double>(low) / x;
            top_slope = std::max(top_slope, top_ratio);
            bottom_slope = std::min(bottom_slope, bottom_ratio);
        }
    }
    // Be safe by rounding a bit
    top_slope = std::ceil(top_slope * 1000.0) / 1000.0;
    bottom_slope = std::floor(bottom_slope * 1000.0) / 1000.0;
    int square_size = 100;
    // If a square fits, its diagonal must be touching both lines.
    // The diagonal lies on the line x + y = c for some c.
    // So we solve the equations:
    //   x_min + top_slope * x_min = c
    //   x_max + bottom_slope * x_max = c
    for (int c = 2 * square_size; ; c++) {
        int x_min = std::floor(c / (1.0 + top_slope));
        int x_max = std::ceil(c / (1.0 + bottom_slope));
        if (x_max - x_min >= square_size) {
            int actual_x_min = -1;
            int actual_x_max = -1;
            for (int x = x_min; x <= x_max; x++) {
                if (has_beam(prog, x, c - x)) {
                    actual_x_min = x;
                    break;
                }
            }
            for (int x = x_max; x >= x_min; x--) {
                if (has_beam(prog, x, c - x)) {
                    actual_x_max = x;
                    break;
                }
            }
            if (actual_x_max - actual_x_min + 1 >= square_size) {
                int result_x = actual_x_min;
                int result_y = c - actual_x_max;
                std::cout << (result_x * 10000 + result_y) << std::endl;
                break;
            }
        }
    }
}
