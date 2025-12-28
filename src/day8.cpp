#include <iostream>

int width = 25;
int height = 6;

void solve1(std::vector<std::string> data) {
    auto line = data[0];
    int layer_size = width * height;
    int num_layers = line.size() / layer_size;
    int min_zero_count = INT_MAX;
    int one_count = 0;
    int res = 0;
    for (int layer = 0; layer < num_layers; layer++) {
        int zero_count = 0;
        int one_count = 0;
        int two_count = 0;
        for (int i = 0; i < layer_size; ++i) {
            char pixel = line[layer * layer_size + i];
            if (pixel == '0') {
                zero_count++;
            } else if (pixel == '1') {
                one_count++;
            } else if (pixel == '2') {
                two_count++;
            }
        }
        if (zero_count < min_zero_count) {
            min_zero_count = zero_count;
            res = one_count * two_count;
        }
    }
    std::cout << res << std::endl;
}

void solve2(std::vector<std::string> data) {
    std::vector<int> image(width * height, 2);
    auto line = data[0];
    int layer_size = width * height;
    int num_layers = line.size() / layer_size;
    for (int layer = 0; layer < num_layers; layer++) {
        for (int i = 0; i < layer_size; ++i) {
            char pixel = line[layer * layer_size + i];
            if (image[i] == 2) {
                if (pixel == '0') {
                    image[i] = 0;
                } else if (pixel == '1') {
                    image[i] = 1;
                }
            }
        }
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image[y * width + x] == 0) {
                std::cout << ' ';
            } else if (image[y * width + x] == 1) {
                std::cout << '#';
            } else {
                std::cout << '?';
            }
        }
        std::cout << std::endl;
    }
}
