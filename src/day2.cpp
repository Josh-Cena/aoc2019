#include <iostream>
#include <sstream>

int run_prog(std::vector<int> codes, int noun, int verb) {
    codes[1] = noun;
    codes[2] = verb;
    for (int i = 0; i + 3 < codes.size() && codes[i] != 99; i += 4) {
        int opcode = codes[i];
        int op1 = codes[i + 1];
        int op2 = codes[i + 2];
        int dest = codes[i + 3];
        if (opcode == 1) {
            codes[dest] = codes[op1] + codes[op2];
        } else if (opcode == 2) {
            codes[dest] = codes[op1] * codes[op2];
        } else {
            throw "Invalid opcode";
        }
    }
    return codes[0];
}

void solve1(std::vector<std::string> data) {
    std::vector<int> codes;
    std::stringstream ss(data[0]);
    std::string part;
    while (std::getline(ss, part, ',')) {
        codes.push_back(std::stoi(part));
    }
    int res = run_prog(codes, 12, 2);
    std::cout << res << std::endl;
}

void solve2(std::vector<std::string> data) {
    std::vector<int> codes;
    std::stringstream ss(data[0]);
    std::string part;
    while (std::getline(ss, part, ',')) {
        codes.push_back(std::stoi(part));
    }
    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            int res = run_prog(codes, noun, verb);
            if (res == 19690720) {
                std::cout << 100 * noun + verb << std::endl;
            }
        }
    }
}
