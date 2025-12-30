#include <iostream>
#include <cstdio>
#include <set>
#include <numeric>

struct Moon {
    int x, y, z;
    int vx = 0, vy = 0, vz = 0;
    Moon(int x, int y, int z) : x(x), y(y), z(z) {}
    void apply_gravity(const Moon &other) {
        if (x < other.x) vx++;
        else if (x > other.x) vx--;
        if (y < other.y) vy++;
        else if (y > other.y) vy--;
        if (z < other.z) vz++;
        else if (z > other.z) vz--;
    }
    void apply_velocity() {
        x += vx;
        y += vy;
        z += vz;
    }
    int energy() const {
        return (std::abs(x) + std::abs(y) + std::abs(z)) *
            (std::abs(vx) + std::abs(vy) + std::abs(vz));
    }
};

void solve1(std::vector<std::string> data) {
    std::vector<Moon> moons;
    for (const auto &line : data) {
        int x, y, z;
        std::sscanf(line.c_str(), "<x=%d, y=%d, z=%d>", &x, &y, &z);
        moons.emplace_back(x, y, z);
    }
    for (int t = 0; t < 1000; t++) {
        for (auto &moon1 : moons) {
            for (const auto &moon2 : moons) {
                if (&moon1 != &moon2) {
                    moon1.apply_gravity(moon2);
                }
            }
        }
        for (auto &moon : moons) {
            moon.apply_velocity();
        }
    }
    int total_energy = 0;
    for (const auto &moon : moons) {
        total_energy += moon.energy();
    }
    std::cout << total_energy << std::endl;
}

void solve2(std::vector<std::string> data) {
    std::set<std::vector<int>> seen_x, seen_y, seen_z;
    long long x_cycle = -1, y_cycle = -1, z_cycle = -1;
    std::vector<Moon> moons;
    for (const auto &line : data) {
        int x, y, z;
        std::sscanf(line.c_str(), "<x=%d, y=%d, z=%d>", &x, &y, &z);
        moons.emplace_back(x, y, z);
    }
    for (int step = 0; x_cycle == -1 || y_cycle == -1 || z_cycle == -1; step++) {
        for (auto &moon1 : moons) {
            for (const auto &moon2 : moons) {
                if (&moon1 != &moon2) {
                    moon1.apply_gravity(moon2);
                }
            }
        }
        for (auto &moon : moons) {
            moon.apply_velocity();
        }
        std::vector<int> state_x, state_y, state_z;
        for (const auto &moon : moons) {
            state_x.push_back(moon.x);
            state_x.push_back(moon.vx);
            state_y.push_back(moon.y);
            state_y.push_back(moon.vy);
            state_z.push_back(moon.z);
            state_z.push_back(moon.vz);
        }
        if (x_cycle == -1) {
            if (seen_x.count(state_x)) {
                x_cycle = step;
            } else {
                seen_x.insert(state_x);
            }
        }
        if (y_cycle == -1) {
            if (seen_y.count(state_y)) {
                y_cycle = step;
            } else {
                seen_y.insert(state_y);
            }
        }
        if (z_cycle == -1) {
            if (seen_z.count(state_z)) {
                z_cycle = step;
            } else {
                seen_z.insert(state_z);
            }
        }
    }
    long long period = std::lcm(std::lcm(x_cycle, y_cycle), z_cycle);
    std::cout << period << std::endl;
}
