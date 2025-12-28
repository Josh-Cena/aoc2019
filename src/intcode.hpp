#include <map>
#include <queue>

struct Param;
struct Inst;

class Program {
    Inst parse_inst();
    long long eval_param(const Param &param);
    void write_to(const Param &param, long long value);
public:
    int ip;
    bool halted;
    long long relative_base;
    std::map<long long, long long> memory;
    std::queue<long long> inputs;
    std::queue<long long> outputs;
    Program(const std::string &prog_line);
    Program(const Program&) = default;
    Program& operator=(const Program&) = default;
    void send_input(long long value);
    bool run_until_output();
    void run();
};
