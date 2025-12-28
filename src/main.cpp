#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

void solve1(std::vector<std::string> data);
void solve2(std::vector<std::string> data);

int main(int argc, char **argv) {
    char *day = argv[1];
    char *prob = argv[2];
    const char *input = argc > 3 ? argv[3] : "real";
    std::ostringstream ss;
    ss << "inputs/day" << day << "/" << input << ".txt";
    std::string filename = ss.str();
    std::ifstream fs(filename);
    std::vector<std::string> contents;
    std::string line;
    while (std::getline(fs, line)) {
        contents.push_back(line);
    }
    if (prob[0] == '1') {
        solve1(contents);
    } else if (prob[0] == '2') {
        solve2(contents);
    } else {
        std::cout << "Unrecognized problem " << prob << std::endl;
    }
    return 0;
}
