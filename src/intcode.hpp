#include <map>
#include <queue>

struct Param;
struct Inst;

class Program {
    int ip = 0;
    long long relative_base = 0;
    std::queue<long long> inputs;
    std::queue<long long> outputs;
    Inst parse_inst();
    long long eval_param(const Param &param);
    void write_to(const Param &param, long long value);
public:
    bool halted = false;
    /**
     * Number of consecutive times an input was requested but none was
     * available. Resets when input is successfully read.
     */
    int starved_cycles = 0;
    /**
     * If no input is available, this value is used as input.
     * If still set to 0xDEAD, an exception is thrown instead.
     */
    long long default_input = 0xDEAD;
    std::map<long long, long long> memory;
    Program(const std::string &prog_line);
    Program(const Program &) = default;
    Program &operator=(const Program&) = default;
    void send_input(long long value);
    void send_input(const std::string &str);
    long long pop_output();
    std::vector<long long> pop_all_output();
    std::string pop_str_output();
    long long peek_last_output();
    bool has_output() const;
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
