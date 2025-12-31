#include <iostream>
#include <map>

// k * x + b (mod n)
class LinearFunction {
    long long mod_mul(long long a, long long b) const {
        // Not portable, but works
        return ((__int128_t)a * b) % mod;
    }

    long long mod_add(long long a, long long b) const {
        return (a + b) % mod;
    }

    long long mod_pow(long long base, long long exp) const {
        long long res = 1;
        while (exp > 0) {
            if (exp & 1) res = mod_mul(res, base);
            base = mod_mul(base, base);
            exp >>= 1;
        }
        return res;
    }

    long long mod_inv(long long n) const {
        if (n < 0) n = (n % mod + mod) % mod;
        return mod_pow(n, mod - 2);
    }
public:
    long long k;
    long long b;
    long long mod;
    // f(g(x)) = k_f * (k_g * x + b_g) + b_f = (k_f * k_g) * x + (k_f * b_g + b_f)
    LinearFunction compose(const LinearFunction &g) const {
        long long new_k = mod_mul(k, g.k);
        long long new_b = mod_add(mod_mul(k, g.b), b);
        return {new_k, new_b, mod};
    }
    LinearFunction pow(long long exponent) const {
        if (exponent == 0) {
            return {1, 0, mod};
        } else if (exponent == 1) {
            return *this;
        } else if (exponent % 2 == 0) {
            LinearFunction half = this->pow(exponent / 2);
            return half.compose(half);
        } else {
            LinearFunction half = this->pow(exponent / 2);
            return this->compose(half.compose(half));
        }
    }
    long long apply(long long x) const {
        return mod_add(mod_mul(k, x), b);
    }
    long long reverse_apply(long long y) const {
        // Solve for x in y = k * x + b (mod n)
        // => x = (y - b) * k_inv (mod n), where k_inv is the modular inverse of k mod n
        long long k_inv = mod_inv(k);
        return mod_mul(mod_add(y, -b), k_inv);
    }
};

// The three functions are essentially (under modulo n):
// deal_into_new_stack(x) = -x - 1
// cut_k(x) = x - k
// deal_with_increment_k(x) = x * k
// Applying them in sequence is function composition.
LinearFunction parse_instructions(
    const std::vector<std::string>& data,
    long long mod
) {
    LinearFunction total_function = {1, 0, mod};
    for (const auto& line : data) {
        if (line == "deal into new stack") {
            LinearFunction inst = {-1, -1, mod};
            total_function = inst.compose(total_function);
        } else if (line.substr(0, 4) == "cut ") {
            long long n = std::stoll(line.substr(4));
            LinearFunction inst = {1, -n, mod};
            total_function = inst.compose(total_function);
        } else if (line.substr(0, 20) == "deal with increment ") {
            long long n = std::stoll(line.substr(20));
            LinearFunction inst = {n, 0, mod};
            total_function = inst.compose(total_function);
        }
    }
    return total_function;
}

void solve1(std::vector<std::string> data) {
    long long pos = 2019;
    long long num_cards = 10007;
    LinearFunction fn = parse_instructions(data, num_cards);
    long long final_pos = fn.apply(pos);
    std::cout << final_pos << std::endl;
}

void solve2(std::vector<std::string> data) {
    long long pos = 2020;
    long long num_cards = 119315717514047;
    long long cycles = 101741582076661;
    LinearFunction fn = parse_instructions(data, num_cards);
    LinearFunction fn_pow = fn.pow(cycles);
    long long initial_pos = fn_pow.reverse_apply(pos);
    std::cout << initial_pos << std::endl;
}
