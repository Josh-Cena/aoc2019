#include <map>
#include <queue>

struct Param;
struct Inst;

class Program {
    int ip;
    long long relative_base;
    std::queue<long long> inputs;
    Inst parse_inst();
    long long eval_param(const Param &param);
    void write_to(const Param &param, long long value);
public:
    bool halted;
    std::map<long long, long long> memory;
    std::queue<long long> outputs;
    Program(const std::string &prog_line);
    Program(const Program&) = default;
    Program& operator=(const Program&) = default;
    void send_input(long long value);
    void step();
    /**
     * Stops **before** an input
     */
    void run_until_input();
    /**
     * Stops **after** an output
     */
    void run_until_output();
    void run();
};
