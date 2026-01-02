#include <iostream>
#include <algorithm>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    Program base_prog(data[0]);
    std::vector<int> phase_settings = {0, 1, 2, 3, 4};
    int max_output = 0;
    do {
        std::vector<Program> progs;
        for (int phase : phase_settings) {
            progs.emplace_back(base_prog);
            progs.back().send_input(phase);
        }
        int input_signal = 0;
        for (auto &prog : progs) {
            prog.send_input(input_signal);
            prog.run();
            input_signal = prog.pop_output();
        }
        max_output = std::max(max_output, input_signal);
    } while (std::next_permutation(phase_settings.begin(), phase_settings.end()));
    std::cout << max_output << std::endl;
}

void solve2(std::vector<std::string> data) {
    Program base_prog(data[0]);
    std::vector<int> phase_settings = {5, 6, 7, 8, 9};
    int max_output = 0;
    do {
        std::vector<Program> progs;
        for (int phase : phase_settings) {
            progs.emplace_back(base_prog);
            progs.back().send_input(phase);
        }
        int input_signal = 0;
        bool all_halted = false;
        while (!all_halted) {
            all_halted = true;
            for (auto &prog : progs) {
                prog.send_input(input_signal);
                prog.run_until_output();
                if (prog.halted) break;
                all_halted = false;
                input_signal = prog.pop_output();
            }
        }
        max_output = std::max(max_output, input_signal);
    } while (std::next_permutation(phase_settings.begin(), phase_settings.end()));
    std::cout << max_output << std::endl;
}
