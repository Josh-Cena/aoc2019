#include <iostream>

std::vector<std::vector<int>> make_nums(int min, int max) {
    std::vector<std::vector<int>> nums;
    for (int a = 1; a <= 9; a++) {
        for (int b = a; b <= 9; b++) {
            for (int c = b; c <= 9; c++) {
                for (int d = c; d <= 9; d++) {
                    for (int e = d; e <= 9; e++) {
                        for (int f = e; f <= 9; f++) {
                            int num = a * 100000 + b * 10000 + c * 1000 +
                                d * 100 + e * 10 + f;
                            if (num < min || num > max) {
                                continue;
                            }
                            nums.push_back({a, b, c, d, e, f});
                        }
                    }
                }
            }
        }
    }
    return nums;
}

void solve1(std::vector<std::string> data) {
    auto line = data[0];
    auto pos = line.find('-');
    int min = std::stoi(line.substr(0, pos));
    int max = std::stoi(line.substr(pos + 1));
    int count = 0;
    for (auto num : make_nums(min, max)) {
        bool has_double = false;
        for (size_t i = 1; i < num.size(); i++) {
            if (num[i] == num[i - 1]) {
                has_double = true;
                break;
            }
        }
        if (has_double) {
            count++;
        }
    }
    std::cout << count << std::endl;
}

void solve2(std::vector<std::string> data) {
    auto line = data[0];
    auto pos = line.find('-');
    int min = std::stoi(line.substr(0, pos));
    int max = std::stoi(line.substr(pos + 1));
    int count = 0;
    for (auto num : make_nums(min, max)) {
        bool has_strict_double = false;
        for (size_t i = 1; i < num.size(); i++) {
            if (num[i] == num[i - 1]) {
                bool left_diff = (i - 2 < 0) || (num[i - 2] != num[i]);
                bool right_diff = (i + 1 >= num.size()) || (num[i + 1] != num[i]);
                if (left_diff && right_diff) {
                    has_strict_double = true;
                    break;
                }
            }
        }
        if (has_strict_double) {
            count++;
        }
    }
    std::cout << count << std::endl;
}
