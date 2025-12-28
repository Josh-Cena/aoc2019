#include <vector>
#include <sstream>
#include <cassert>
#include "intcode.hpp"

struct Param {
    int mode;
    int value;
};

struct Inst {
    int opcode;
    std::vector<Param> params;
};

std::vector<int> parse_prog(const std::string &line) {
    std::vector<int> codes;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, ',')) {
        codes.push_back(std::stoi(part));
    }
    return codes;
}

Program::Program(const std::string &prog_line)
    : codes(parse_prog(prog_line)), ip(0), halted(false) {}

int num_params_for_opcode(int opcode) {
    switch (opcode) {
        case 1:
        case 2:
        case 7:
        case 8:
            return 3;
        case 3:
        case 4:
            return 1;
        case 5:
        case 6:
            return 2;
        case 99:
            return 0;
        default:
            throw std::invalid_argument("Unknown opcode");
    }
}

Inst parse_inst(const std::vector<int> &codes, int ip) {
    int raw_opcode = codes[ip];
    Inst inst;
    inst.opcode = raw_opcode % 100;
    int param_modes = raw_opcode / 100;
    int num_params = num_params_for_opcode(inst.opcode);
    for (int i = 0; i < num_params; i++) {
        Param param;
        param.mode = param_modes % 10;
        param.value = codes[ip + 1 + i];
        inst.params.push_back(param);
        param_modes /= 10;
    }
    return inst;
}

int eval_param(const std::vector<int> &codes, const Param &param) {
    if (param.mode == 0) {
        return codes[param.value];
    } else if (param.mode == 1) {
        return param.value;
    } else {
        throw std::invalid_argument("Unknown parameter mode");
    }
}

int write_to(std::vector<int> &codes, const Param &param, int value) {
    if (param.mode != 0) {
        throw std::invalid_argument("Write parameter must be in position mode");
    }
    codes[param.value] = value;
    return 0;
}

void Program::send_input(int value) {
    inputs.push(value);
}

bool Program::run_until_output() {
    assert(!halted);
    int input_idx = 0;
    while (codes[ip] != 99) {
        Inst inst = parse_inst(codes, ip);
        switch (inst.opcode) {
            case 1:
            case 2:
            case 7:
            case 8: {
                int val1 = eval_param(codes, inst.params[0]);
                int val2 = eval_param(codes, inst.params[1]);
                int res = inst.opcode == 1
                    ? val1 + val2
                    : inst.opcode == 2
                    ? val1 * val2
                    : inst.opcode == 7
                    ? (val1 < val2 ? 1 : 0)
                    : (val1 == val2 ? 1 : 0);
                write_to(codes, inst.params[2], res);
                ip += num_params_for_opcode(inst.opcode) + 1;
                break;
            }
            case 3: {
                write_to(codes, inst.params[0], inputs.front());
                inputs.pop();
                ip += num_params_for_opcode(inst.opcode) + 1;
                break;
            }
            case 4: {
                int val = eval_param(codes, inst.params[0]);
                outputs.push(val);
                ip += num_params_for_opcode(inst.opcode) + 1;
                return true;
            }
            case 5:
            case 6: {
                int val = eval_param(codes, inst.params[0]);
                bool should_jump = inst.opcode == 5 ? (val != 0) : (val == 0);
                if (should_jump) {
                    ip = eval_param(codes, inst.params[1]);
                } else {
                    ip += num_params_for_opcode(inst.opcode) + 1;
                }
                break;
            }
            default:
                throw std::invalid_argument("Unknown opcode");
        }
    }
    halted = true;
    return false;
}

void Program::run() {
    while (!halted) {
        run_until_output();
    }
}
