#include <iostream>
#include <algorithm>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    Program base_prog(data[0]);
    std::vector<int> phase_settings = {0, 1, 2, 3, 4};
    int max_output = 0;
    do {
        int input_signal = 0;
        for (int phase : phase_settings) {
            Program prog(base_prog);
            prog.send_input(phase);
            prog.send_input(input_signal);
            prog.run();
            input_signal = prog.outputs.back();
        }
        if (input_signal > max_output) {
            max_output = input_signal;
        }
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
            Program prog(base_prog);
            prog.send_input(phase);
            progs.push_back(prog);
        }
        int input_signal = 0;
        bool all_halted = false;
        while (!all_halted) {
            all_halted = true;
            for (auto &prog : progs) {
                prog.send_input(input_signal);
                prog.run_until_output();
                if (!prog.halted) {
                    all_halted = false;
                }
                if (!prog.outputs.empty()) {
                    input_signal = prog.outputs.back();
                    prog.outputs.pop();
                }
            }
        }
        if (input_signal > max_output) {
            max_output = input_signal;
        }
    } while (std::next_permutation(phase_settings.begin(), phase_settings.end()));
    std::cout << max_output << std::endl;
}
