#include <iostream>
#include <map>
#include <cassert>

int portal_code(char c1, char c2) {
    return ((int)c1 << 8) | (int)c2;
}

std::vector<std::tuple<int, int, int, char, char>> donut_edges(
    const std::vector<std::string> &data, int width, int height, int donut_width
) {
    std::vector<std::tuple<int, int, int, char, char>> edges;
    int inner_l = donut_width + 1;
    int inner_r = width - (donut_width + 2);
    int inner_t = donut_width + 1;
    int inner_b = height - (donut_width + 2);
    for (int c = 2; c < width - 2; c++) {
        edges.emplace_back(2, c, 1, data[0][c], data[1][c]);
        edges.emplace_back(height - 3, c, 1, data[height - 2][c], data[height - 1][c]);
    }
    for (int r = 2; r < height - 2; r++) {
        edges.emplace_back(r, 2, 1, data[r][0], data[r][1]);
        edges.emplace_back(r, width - 3, 1, data[r][width - 2], data[r][width - 1]);
    }
    for (int c = inner_l + 1; c < inner_r; c++) {
        edges.emplace_back(inner_t, c, -1, data[inner_t + 1][c], data[inner_t + 2][c]);
        edges.emplace_back(inner_b, c, -1, data[inner_b - 2][c], data[inner_b - 1][c]);
    }
    for (int r = inner_t + 1; r < inner_b; r++) {
        edges.emplace_back(r, inner_l, -1, data[r][inner_l + 1], data[r][inner_l + 2]);
        edges.emplace_back(r, inner_r, -1, data[r][inner_r - 2], data[r][inner_r - 1]);
    }
    return edges;
}

void solve1(std::vector<std::string> data) {
    int width = data[0].size();
    int height = data.size();
    int donut_width;
    for (int c = width / 2; c > 0; c--) {
        if (data[height / 2][c] != ' ') {
            donut_width = c - 1;
            break;
        }
    }
    std::map<std::pair<int, int>, int> pos_to_portal;
    std::map<int, std::vector<std::pair<int, int>>> portal_to_poss;
    for (auto &[r, c, lvl, ch1, ch2] : donut_edges(data, width, height, donut_width)) {
        if (ch1 >= 'A' && ch1 <= 'Z' && ch2 >= 'A' && ch2 <= 'Z') {
            int code = portal_code(ch1, ch2);
            pos_to_portal[{r, c}] = code;
            portal_to_poss[code].emplace_back(r, c);
        }
    }
    std::pair<int, int> start_pos = portal_to_poss[portal_code('A', 'A')][0];
    std::pair<int, int> end_pos = portal_to_poss[portal_code('Z', 'Z')][0];
    // AA and ZZ aren't real portals
    portal_to_poss.erase(portal_code('A', 'A'));
    portal_to_poss.erase(portal_code('Z', 'Z'));
    pos_to_portal.erase(start_pos);
    pos_to_portal.erase(end_pos);
    std::queue<std::pair<int, int>> queue;
    std::map<std::pair<int, int>, int> dist;
    queue.push(start_pos);
    dist[start_pos] = 0;
    const std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    while (!queue.empty()) {
        auto [cr, cc] = queue.front();
        queue.pop();
        int cd = dist[{cr, cc}];
        for (const auto &[dr, dc] : directions) {
            int nr = cr + dr;
            int nc = cc + dc;
            std::pair<int, int> neighbor_pos = {nr, nc};
            if (nr >= 0 && nr < height && nc >= 0 && nc < width &&
                    data[nr][nc] == '.' &&
                    dist.find(neighbor_pos) == dist.end()) {
                if (nr == end_pos.first && nc == end_pos.second) {
                    std::cout << cd + 1 << std::endl;
                    return;
                }
                dist[neighbor_pos] = cd + 1;
                queue.push(neighbor_pos);
            }
        }
        auto it = pos_to_portal.find({cr, cc});
        if (it != pos_to_portal.end()) {
            int portal = it->second;
            bool found = false;
            for (const auto &pos : portal_to_poss[portal]) {
                auto [pr, pc] = pos;
                if (pr == cr && pc == cc) continue;
                found = true;
                if (dist.find(pos) == dist.end()) {
                    dist[pos] = cd + 1;
                    queue.push(pos);
                }
            }
            assert(found);
        }
    }
    throw std::runtime_error("No path found");
}

void solve2(std::vector<std::string> data) {
    int width = data[0].size();
    int height = data.size();
    int donut_width;
    for (int c = width / 2; c > 0; c--) {
        if (data[height / 2][c] != ' ') {
            donut_width = c - 1;
            break;
        }
    }
    std::map<std::pair<int, int>, int> pos_to_portal;
    std::map<int, std::vector<std::tuple<int, int, int>>> portal_to_poss;
    for (auto &[r, c, dl, ch1, ch2] : donut_edges(data, width, height, donut_width)) {
        if (ch1 >= 'A' && ch1 <= 'Z' && ch2 >= 'A' && ch2 <= 'Z') {
            int code = portal_code(ch1, ch2);
            pos_to_portal[{r, c}] = code;
            portal_to_poss[code].emplace_back(r, c, dl);
        }
    }
    auto [start_r, start_c, start_dl] = portal_to_poss[portal_code('A', 'A')][0];
    auto [end_r, end_c, end_dl] = portal_to_poss[portal_code('Z', 'Z')][0];
    // AA and ZZ aren't real portals
    portal_to_poss.erase(portal_code('A', 'A'));
    portal_to_poss.erase(portal_code('Z', 'Z'));
    pos_to_portal.erase({start_r, start_c});
    pos_to_portal.erase({end_r, end_c});
    std::queue<std::tuple<int, int, int>> queue;
    std::map<std::tuple<int, int, int>, int> dist;
    queue.push({start_r, start_c, 0});
    dist[{start_r, start_c, 0}] = 0;
    const std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    while (!queue.empty()) {
        auto [cr, cc, cl] = queue.front();
        queue.pop();
        int cd = dist[{cr, cc, cl}];
        for (const auto &[dr, dc] : directions) {
            int nr = cr + dr;
            int nc = cc + dc;
            std::tuple<int, int, int> neighbor_pos = {nr, nc, cl};
            if (nr >= 0 && nr < height && nc >= 0 && nc < width &&
                    data[nr][nc] == '.' &&
                    dist.find(neighbor_pos) == dist.end()) {
                if (nr == end_r && nc == end_c && cl == 0) {
                    std::cout << cd + 1 << std::endl;
                    return;
                }
                dist[neighbor_pos] = cd + 1;
                queue.push(neighbor_pos);
            }
        }
        auto it = pos_to_portal.find({cr, cc});
        if (it != pos_to_portal.end()) {
            int portal = it->second;
            bool found = false;
            for (const auto [pr, pc, dl] : portal_to_poss[portal]) {
                if (pr == cr && pc == cc) continue;
                found = true;
                int pl = cl + dl;
                if (pl < 0) continue;
                std::tuple<int, int, int> pos = {pr, pc, pl};
                if (dist.find(pos) == dist.end()) {
                    dist[pos] = cd + 1;
                    queue.push(pos);
                }
            }
            assert(found);
        }
    }
    throw std::runtime_error("No path found");
}
