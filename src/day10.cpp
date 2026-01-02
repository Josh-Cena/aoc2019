#include <iostream>
#include <set>
#include <map>
#include <numeric>
#include <algorithm>

using direction_map = std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>;

direction_map locate_station(const std::set<std::pair<int, int>> &asteroids) {
    int max_visible = 0;
    std::pair<int, int> best_location;
    direction_map best_directions;
    for (auto from : asteroids) {
        direction_map directions;
        for (auto to : asteroids) {
            if (to == from) continue;
            int dx = to.first - from.first;
            int dy = to.second - from.second;
            int gcd = std::gcd(std::abs(dx), std::abs(dy));
            directions[{dx / gcd, dy / gcd}].push_back(to);
        }
        if (directions.size() > max_visible) {
            max_visible = directions.size();
            best_location = from;
            best_directions = directions;
        }
    }
    return best_directions;
}

// The angle is measured from the positive Y axis clockwise
bool compare_angle(const std::pair<int, int> &a, const std::pair<int, int> &b) {
    double angle_a = atan2(a.first, -a.second);
    double angle_b = atan2(b.first, -b.second);
    if (angle_a < 0) angle_a += 2 * M_PI;
    if (angle_b < 0) angle_b += 2 * M_PI;
    return angle_a < angle_b;
}

void solve1(std::vector<std::string> data) {
    std::set<std::pair<int, int>> asteroids;
    int width = data[0].size();
    int height = data.size();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (data[y][x] == '#') {
                asteroids.insert({x, y});
            }
        }
    }
    auto best_directions = locate_station(asteroids);
    std::cout << best_directions.size() << std::endl;
}

void solve2(std::vector<std::string> data) {
    std::set<std::pair<int, int>> asteroids;
    int width = data[0].size();
    int height = data.size();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (data[y][x] == '#') {
                asteroids.insert({x, y});
            }
        }
    }
    auto best_directions = locate_station(asteroids);
    std::map<std::pair<int, int>, int> num_vaporized;
    std::vector<std::pair<int, int>> direction_order;
    for (const auto &dir_pair : best_directions) {
        direction_order.push_back(dir_pair.first);
        num_vaporized[dir_pair.first] = 0;
    }
    std::sort(direction_order.begin(), direction_order.end(), compare_angle);
    int vaporized_count = 0;
    while (true) {
        for (const auto &direction : direction_order) {
            auto &targets = best_directions[direction];
            int &i = num_vaporized[direction];
            if (i == targets.size()) continue;
            auto target = targets[i];
            i++;
            vaporized_count++;
            if (vaporized_count == 200) {
                int res = target.first * 100 + target.second;
                std::cout << res << std::endl;
                return;
            }
        }
    }
}
