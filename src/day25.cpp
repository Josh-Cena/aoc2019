#include <iostream>
#include <sstream>
#include "intcode.hpp"

std::string start_prog(Program &prog) {
    prog.run_until_input();
    return prog.pop_str_output();
}

std::string exchange_msg(Program &prog, const std::string &msg) {
    prog.send_input(msg);
    prog.send_input('\n');
    prog.run_until_output();
    prog.run_until_input();
    return prog.pop_str_output();
}

void solve1(std::vector<std::string> data) {
    Program prog(data[0]);
    bool visualize = std::getenv("VISUALIZE") != nullptr;
    if (visualize) {
        std::cout << "\x1b[2J\x1b[1;1H" << start_prog(prog);
        while (!prog.halted) {
            std::string command;
            std::getline(std::cin, command);
            std::cout << "\x1b[2J\x1b[1;1H" << exchange_msg(prog, command);
        }
        return;
    }
    std::vector<std::string> take_everything = {
        "east", "south", "south", "take hologram",
        "north", "north", "west", "south", "take mouse",
        "east", "take shell",
        "west", "west", "take whirled peas",
        "east", "north", "west", "north", "north", "west", "take semiconductor",
        "east", "south", "west", "south", "take hypercube",
        "north", "east", "south", "west", "take antenna",
        "south", "take spool of cat6",
        "north", "west", "south", "south"
    };
    start_prog(prog);
    while (!prog.halted && !take_everything.empty()) {
        exchange_msg(prog, take_everything.front());
        take_everything.erase(take_everything.begin());
    }
    std::vector<std::string> items = {
        "hologram", "mouse", "shell", "whirled peas",
        "semiconductor", "hypercube", "antenna", "spool of cat6"
    };
    int n = items.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        std::vector<std::string> to_drop;
        std::vector<std::string> to_take;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                exchange_msg(prog, "drop " + items[i]);
            } else {
                exchange_msg(prog, "take " + items[i]);
            }
        }
        auto output = exchange_msg(prog, "south");
        if (prog.halted) {
            std::cout << output;
            break;
        }
    }
}

void solve2(std::vector<std::string> data) {
    std::cout << "No such thing, yay" << std::endl;
}
