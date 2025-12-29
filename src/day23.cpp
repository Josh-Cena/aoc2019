#include <iostream>
#include "intcode.hpp"

void solve1(std::vector<std::string> data) {
    std::vector<Program> network;
    for (int i = 0; i < 50; i++) {
        network.emplace_back(data[0]);
        network[i].send_input(i);
        network[i].default_input = -1;
    }
    while (true) {
        for (int i = 0; i < 50; i++) {
            Program &prog = network[i];
            prog.step();
            if (!prog.outputs.empty()) {
                long long dest = prog.outputs.front();
                prog.outputs.pop();
                prog.run_until_output();
                long long x = prog.outputs.front();
                prog.outputs.pop();
                prog.run_until_output();
                long long y = prog.outputs.front();
                prog.outputs.pop();
                if (dest == 255) {
                    std::cout << y << std::endl;
                    return;
                }
                network[dest].send_input(x);
                network[dest].send_input(y);
            }
        }
    }
}

void solve2(std::vector<std::string> data) {
    std::vector<Program> network;
    for (int i = 0; i < 50; i++) {
        network.emplace_back(data[0]);
        network[i].send_input(i);
        network[i].default_input = -1;
    }
    std::pair<long long, long long> nat_packet = {-1, -1};
    long long last_y_sent = -1;
    while (true) {
        for (int i = 0; i < 50; i++) {
            Program &prog = network[i];
            prog.step();
            if (!prog.outputs.empty()) {
                long long dest = prog.outputs.front();
                prog.outputs.pop();
                prog.run_until_output();
                long long x = prog.outputs.front();
                prog.outputs.pop();
                prog.run_until_output();
                long long y = prog.outputs.front();
                prog.outputs.pop();
                if (dest == 255) {
                    nat_packet = {x, y};
                } else {
                    network[dest].send_input(x);
                    network[dest].send_input(y);
                }
            }
        }
        bool all_idle = true;
        for (int i = 0; i < 50; i++) {
            if (network[i].starved_cycles < 2) {
                all_idle = false;
                break;
            }
        }
        if (all_idle && nat_packet.first != -1) {
            network[0].send_input(nat_packet.first);
            network[0].send_input(nat_packet.second);
            network[0].starved_cycles = 0;
            if (nat_packet.second == last_y_sent) {
                std::cout << nat_packet.second << std::endl;
                return;
            }
            last_y_sent = nat_packet.second;
        }
    }
}
