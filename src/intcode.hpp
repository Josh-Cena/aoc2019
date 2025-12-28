#include <vector>
#include <queue>

class Program {
public:
    std::vector<int> codes;
    int ip;
    std::queue<int> inputs;
    std::queue<int> outputs;
    bool halted;
    Program(const std::string &prog_line);
    Program(const Program&) = default;
    Program& operator=(const Program&) = default;
    void send_input(int value);
    bool run_until_output();
    void run();
};
