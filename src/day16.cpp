#include <iostream>
#include <cassert>

void solve1(std::vector<std::string> data) {
    std::vector<int> numbers;
    int base_pattern[] = {0, 1, 0, -1};
    for (char c : data[0]) {
        numbers.push_back(c - '0');
    }
    for (int phase = 0; phase < 100; phase++) {
        std::vector<int> new_numbers(numbers.size());
        for (size_t i = 0; i < numbers.size(); i++) {
            int sum = 0;
            int repeat_count = 0;
            for (size_t j = 0; j < numbers.size(); j++) {
                sum += numbers[j] * base_pattern[((j + 1) / (i + 1)) % 4];
            }
            new_numbers[i] = std::abs(sum) % 10;
        }
        numbers = new_numbers;
    }
    for (int i = 0; i < 8; i++) {
        std::cout << numbers[i];
    }
    std::cout << std::endl;
}

void solve2(std::vector<std::string> data) {
    // pattern of b[-1] = [0, ..., 0, 1]
    // pattern of b[-2] = [0, ..., 0, 1, 1]
    // ...
    // Since the offset happens to be in the second half,
    // each element from the end is just the sum of all elements after it
    std::string line = data[0];
    int nums_len = line.size() * 10000;
    int offset = std::stoi(line.substr(0, 7));
    assert(offset > nums_len / 2);
    std::vector<int> last_few(nums_len - offset);
    for (int i = offset; i < nums_len; i++) {
        last_few[i - offset] = line[i % line.size()] - '0';
    }
    for (int phase = 0; phase < 100; phase++) {
        int sum = 0;
        for (int i = last_few.size() - 1; i >= 0; i--) {
            sum = (sum + last_few[i]) % 10;
            last_few[i] = sum;
        }
    }
    for (int i = 0; i < 8; i++) {
        std::cout << last_few[i];
    }
    std::cout << std::endl;
}
