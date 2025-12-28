#include <iostream>
#include <sstream>
#include <set>
#include <map>

struct Turn {
    char dir;
    int steps;
};

std::vector<Turn> parse_wire(const std::string &line) {
    std::vector<Turn> wire;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, ',')) {
        wire.push_back({part[0], std::stoi(part.substr(1))});
    }
    return wire;
}

void register_points(
    std::set<int> &x_points,
    std::set<int> &y_points,
    const std::vector<Turn> &wire
) {
    int x = 0;
    int y = 0;
    x_points.insert(x);
    y_points.insert(y);
    for (const Turn &turn : wire) {
        switch (turn.dir) {
            case 'U':
            x += turn.steps;
            break;
            case 'D':
            x -= turn.steps;
            break;
            case 'R':
            y += turn.steps;
            break;
            case 'L':
            y -= turn.steps;
            break;
        }
        x_points.insert(x);
        y_points.insert(y);
    }
}

std::vector<std::pair<int, int>> trace_wire(
    const std::vector<Turn> &wire,
    const std::map<int, int> &x_compressor,
    const std::map<int, int> &y_compressor,
    const std::vector<int> &x_decompressor,
    const std::vector<int> &y_decompressor
) {
    int x = x_compressor.at(0);
    int y = y_compressor.at(0);
    std::vector<std::pair<int, int>> path;
    for (const Turn &turn : wire) {
        switch (turn.dir) {
            case 'U': {
                int new_x = x_compressor.at(x_decompressor.at(x) + turn.steps);
                for (int i = x + 1; i <= new_x; i++) {
                    path.push_back({i, y});
                }
                x = new_x;
                break;
            }
            case 'D': {
                int new_x = x_compressor.at(x_decompressor.at(x) - turn.steps);
                for (int i = x - 1; i >= new_x; i--) {
                    path.push_back({i, y});
                }
                x = new_x;
                break;
            }
            case 'R': {
                int new_y = y_compressor.at(y_decompressor.at(y) + turn.steps);
                for (int i = y + 1; i <= new_y; i++) {
                    path.push_back({x, i});
                }
                y = new_y;
                break;
            }
            case 'L': {
                int new_y = y_compressor.at(y_decompressor.at(y) - turn.steps);
                for (int i = y - 1; i >= new_y; i--) {
                    path.push_back({x, i});
                }
                y = new_y;
                break;
            }
        }
    }
    return path;
}

std::vector<std::vector<int>> create_grid(
    const std::vector<Turn> &wire1,
    const std::vector<Turn> &wire2,
    std::map<int, int> &x_compressor,
    std::map<int, int> &y_compressor,
    std::vector<int> &x_decompressor,
    std::vector<int> &y_decompressor
) {
    std::set<int> x_points;
    std::set<int> y_points;
    register_points(x_points, y_points, wire1);
    register_points(x_points, y_points, wire2);
    int i = 0;
    for (auto x : x_points) {
        x_decompressor.push_back(x);
        x_compressor[x] = i;
        i++;
    }
    i = 0;
    for (auto y : y_points) {
        y_decompressor.push_back(y);
        y_compressor[y] = i;
        i++;
    }
    std::vector<std::vector<int>> grid(
        x_decompressor.size(),
        std::vector<int>(y_decompressor.size(), 0)
    );
    for (auto p : trace_wire(wire1, x_compressor, y_compressor,
        x_decompressor, y_decompressor)) {
        grid[p.first][p.second] |= 1;
    }
    for (auto p : trace_wire(wire2, x_compressor, y_compressor,
        x_decompressor, y_decompressor)) {
        grid[p.first][p.second] |= 2;
    }
    return grid;
}

std::map<std::pair<int, int>, int> get_dist_to_intersection(
    const std::vector<Turn> &wire,
    const std::vector<std::vector<int>> &grid,
    const std::map<int, int> &x_compressor,
    const std::map<int, int> &y_compressor,
    const std::vector<int> &x_decompressor,
    const std::vector<int> &y_decompressor
) {
    std::map<std::pair<int, int>, int> dist_to_intersection;
    int dist = 0;
    int x = 0;
    int y = 0;
    for (auto p : trace_wire(wire, x_compressor, y_compressor,
        x_decompressor, y_decompressor)) {
        int real_x = x_decompressor[p.first];
        int real_y = y_decompressor[p.second];
        dist += std::abs(real_x - x) + std::abs(real_y - y);
        x = real_x;
        y = real_y;
        if (grid[p.first][p.second] == 3) {
            dist_to_intersection[{p.first, p.second}] = dist;
        }
    }
    return dist_to_intersection;
}

void solve1(std::vector<std::string> data) {
    auto wire1 = parse_wire(data[0]);
    auto wire2 = parse_wire(data[1]);
    std::map<int, int> x_compressor;
    std::map<int, int> y_compressor;
    std::vector<int> x_decompressor;
    std::vector<int> y_decompressor;
    auto grid = create_grid(
        wire1, wire2, x_compressor, y_compressor, x_decompressor, y_decompressor
    );
    int best_dist = INT_MAX;
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 3) {
                int dist = std::abs(x_decompressor[i]) + std::abs(y_decompressor[j]);
                if (dist < best_dist) {
                    best_dist = dist;
                }
            }
        }
    }
    std::cout << best_dist << std::endl;
}

void solve2(std::vector<std::string> data) {
    auto wire1 = parse_wire(data[0]);
    auto wire2 = parse_wire(data[1]);
    std::map<int, int> x_compressor;
    std::map<int, int> y_compressor;
    std::vector<int> x_decompressor;
    std::vector<int> y_decompressor;
    auto grid = create_grid(
        wire1, wire2, x_compressor, y_compressor, x_decompressor, y_decompressor
    );
    auto dist1 = get_dist_to_intersection(
        wire1, grid, x_compressor, y_compressor, x_decompressor, y_decompressor
    );
    auto dist2 = get_dist_to_intersection(
        wire2, grid, x_compressor, y_compressor, x_decompressor, y_decompressor
    );
    int best_dist = INT_MAX;
    for (const auto &entry : dist1) {
        int total = entry.second + dist2[entry.first];
        if (total < best_dist) {
            best_dist = total;
        }
    }
    std::cout << best_dist << std::endl;
}
