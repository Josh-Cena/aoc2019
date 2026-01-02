#include <vector>
#include <map>
#include <sstream>
#include <cassert>
#include "intcode.hpp"

struct Param {
    int mode;
    long long value;
};

struct Inst {
    int opcode;
    std::vector<Param> params;
};

std::map<long long, long long> parse_prog(const std::string &line) {
    std::map<long long, long long> memory;
    std::stringstream ss(line);
    std::string part;
    long long address = 0;
    while (std::getline(ss, part, ',')) {
        memory[address++] = std::stoll(part);
    }
    return memory;
}

Program::Program(const std::string &prog_line)
    : memory(parse_prog(prog_line)) {}

int num_params_for_opcode(int opcode) {
    switch (opcode) {
        case 1:
        case 2:
        case 7:
        case 8:
            return 3;
        case 3:
        case 4:
        case 9:
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

Inst Program::parse_inst() {
    long long raw_opcode = memory[ip];
    Inst inst;
    inst.opcode = raw_opcode % 100;
    int param_modes = raw_opcode / 100;
    int num_params = num_params_for_opcode(inst.opcode);
    for (int i = 0; i < num_params; i++) {
        Param param;
        param.mode = param_modes % 10;
        param.value = memory[ip + 1 + i];
        inst.params.push_back(param);
        param_modes /= 10;
    }
    return inst;
}

long long Program::eval_param(const Param &param) {
    if (param.mode == 1) {
        return param.value;
    } else if (param.mode == 0 || param.mode == 2) {
        long long addr = param.value;
        if (param.mode == 2) {
            addr += relative_base;
        }
        if (addr < 0) {
            throw std::invalid_argument("Negative memory access");
        } else if (memory.find(addr) == memory.end()) {
            return 0;
        }
        return memory[addr];
    } else {
        throw std::invalid_argument("Unknown parameter mode");
    }
}

void Program::write_to(const Param &param, long long value) {
    if (param.mode == 1) {
        throw std::invalid_argument("Write parameter cannot be immediate");
    }
    long long addr = param.value;
    if (param.mode == 2) {
        addr += relative_base;
    }
    if (addr < 0) {
        throw std::invalid_argument("Negative memory access");
    }
    memory[addr] = value;
}

void Program::send_input(long long value) {
    starved_cycles = 0;
    inputs.push(value);
}

void Program::send_input(const std::string &str) {
    for (char c : str) {
        inputs.push(static_cast<long long>(c));
    }
}

long long Program::pop_output() {
    if (outputs.empty()) {
        throw std::runtime_error("No output available");
    }
    long long value = outputs.front();
    outputs.pop();
    return value;
}

std::vector<long long> Program::pop_all_output() {
    std::vector<long long> all_outputs;
    while (!outputs.empty()) {
        all_outputs.push_back(outputs.front());
        outputs.pop();
    }
    return all_outputs;
}

std::string Program::pop_str_output() {
    std::stringstream result;
    while (!outputs.empty()) {
        result << static_cast<char>(outputs.front());
        outputs.pop();
    }
    return result.str();
}

long long Program::peek_last_output() {
    if (outputs.empty()) {
        throw std::runtime_error("No output available");
    }
    return outputs.back();
}

bool Program::has_output() const {
    return !outputs.empty();
}

void Program::step() {
    assert(!halted);
    Inst inst = parse_inst();
    switch (inst.opcode) {
        case 1:
        case 2:
        case 7:
        case 8: {
            long long val1 = eval_param(inst.params[0]);
            long long val2 = eval_param(inst.params[1]);
            long long res = inst.opcode == 1
                ? val1 + val2
                : inst.opcode == 2
                ? val1 * val2
                : inst.opcode == 7
                ? (val1 < val2 ? 1 : 0)
                : (val1 == val2 ? 1 : 0);
            write_to(inst.params[2], res);
            ip += num_params_for_opcode(inst.opcode) + 1;
            break;
        }
        case 3: {
            long long input = default_input;
            if (inputs.empty()) {
                if (default_input == 0xDEAD)
                    throw std::runtime_error("Input expected but not available");
                starved_cycles++;
            } else {
                starved_cycles = 0;
                input = inputs.front();
                inputs.pop();
            }
            write_to(inst.params[0], input);
            ip += num_params_for_opcode(inst.opcode) + 1;
            break;
        }
        case 4: {
            long long val = eval_param(inst.params[0]);
            outputs.push(val);
            ip += num_params_for_opcode(inst.opcode) + 1;
            break;
        }
        case 5:
        case 6: {
            long long val = eval_param(inst.params[0]);
            bool should_jump = inst.opcode == 5 ? (val != 0) : (val == 0);
            if (should_jump) {
                ip = eval_param(inst.params[1]);
            } else {
                ip += num_params_for_opcode(inst.opcode) + 1;
            }
            break;
        }
        case 9: {
            long long val = eval_param(inst.params[0]);
            relative_base += val;
            ip += num_params_for_opcode(inst.opcode) + 1;
            break;
        }
        case 99:
            halted = true;
            break;
        default:
            throw std::invalid_argument("Unknown opcode");
    }
}

void Program::run_until_input() {
    while (!(halted || parse_inst().opcode == 3)) {
        step();
    }
}

void Program::run_until_output() {
    while (!(halted || !outputs.empty())) {
        step();
    }
}

void Program::run() {
    while (!halted) {
        step();
    }
}
