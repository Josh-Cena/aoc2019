#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <algorithm>

// Only paths between keys and doors matter.
std::map<char, std::map<char, int>> build_graph(const std::vector<std::string> &data) {
    int width = data[0].size();
    int height = data.size();
    std::map<char, std::map<char, int>> dists;
    const std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            char start = data[r][c];
            if (start != '.' && start != '#') {
                // BFS from (r, c)
                std::map<std::pair<int, int>, int> dist;
                std::queue<std::pair<int, int>> queue;
                queue.push({r, c});
                dist[{r, c}] = 0;
                while (!queue.empty()) {
                    auto [cr, cc] = queue.front();
                    queue.pop();
                    int cd = dist[{cr, cc}];
                    for (const auto &[dr, dc] : directions) {
                        int nr = cr + dr;
                        int nc = cc + dc;
                        if (nr >= 0 && nr < height && nc >= 0 && nc < width &&
                                data[nr][nc] != '#' &&
                                dist.find({nr, nc}) == dist.end()) {
                            dist[{nr, nc}] = cd + 1;
                            char cell = data[nr][nc];
                            if (cell != '.' && cell != '#') {
                                dists[start][cell] = cd + 1;
                            } else {
                                queue.push({nr, nc});
                            }
                        }
                    }
                }
            }
        }
    }
    return dists;
}

// The maze is a graph of states (location, collectedKeys), where location
// is either a key or a door.
// Edges go to (key', collectedKeys + [key']) and (door, collectedKeys)
// (only if door in collectedKeys).
// We want the shortest path from (start, []) to (?, allKeys).
std::vector<std::pair<std::pair<char, int>, int>> get_neighbors_single(
    const std::map<char, std::map<char, int>> &graph,
    const std::pair<char, int> &state
) {
    std::vector<std::pair<std::pair<char, int>, int>> neighbors;
    auto [node, keys_mask] = state;
    for (const auto &[neighbor, weight] : graph.at(node)) {
        // To go to a door, we should have its key.
        if (neighbor >= 'A' && neighbor <= 'Z') {
            char required_key = neighbor - 'A' + 'a';
            if ((keys_mask & (1 << (required_key - 'a'))) == 0) continue;
        }
        int new_keys_mask = keys_mask;
        if (neighbor >= 'a' && neighbor <= 'z') {
            new_keys_mask |= (1 << (neighbor - 'a'));
        }
        neighbors.push_back({{neighbor, new_keys_mask}, weight});
    }
    return neighbors;
}

std::vector<std::pair<std::pair<int, int>, int>> get_neighbors_multi(
    const std::map<char, std::map<char, int>> &graph,
    const std::pair<int, int> &state
) {
    std::vector<std::pair<std::pair<int, int>, int>> neighbors;
    auto [nodes, keys_mask] = state;
    for (int i = 0; i < 4; i++) {
        char node = (nodes >> (i * 8)) & 0xFF;
        for (const auto &[neighbor_state, weight] : get_neighbors_single(
            graph, {node, keys_mask}
        )) {
            int new_nodes = nodes;
            new_nodes &= ~(0xFF << (i * 8));
            new_nodes |= (neighbor_state.first << (i * 8));
            neighbors.push_back({{new_nodes, neighbor_state.second}, weight});
        }
    }
    return neighbors;
}

template<typename State>
int dijkstra(
    const std::map<char, std::map<char, int>> &graph,
    State initial_state,
    std::vector<std::pair<State, int>> (*get_neighbors)(
        const std::map<char, std::map<char, int>> &graph,
        const State &state
    )
) {
    std::map<State, int> dist;
    using PQElement = std::pair<int, State>; // (distance, State)
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    dist[initial_state] = 0;
    pq.push({0, initial_state});
    while (!pq.empty()) {
        auto [cdist, state] = pq.top();
        pq.pop();
        if (dist[state] < cdist) continue;
        auto [node, keys_mask] = state;
        for (const auto &[neighbor_state, weight] : get_neighbors(graph, state)) {
            int new_dist = cdist + weight;
            auto it = dist.find(neighbor_state);
            if (it == dist.end() || new_dist < it->second) {
                dist[neighbor_state] = new_dist;
                pq.push({new_dist, neighbor_state});
            }
        }
    }
    int all_keys_mask = 0;
    for (const auto &[node, _] : graph) {
        if (node >= 'a' && node <= 'z') {
            all_keys_mask |= (1 << (node - 'a'));
        }
    }
    int min_dist = INT_MAX;
    for (const auto &[state, d] : dist) {
        auto [node, keys_mask] = state;
        if (keys_mask == all_keys_mask) {
            min_dist = std::min(min_dist, d);
        }
    }
    return min_dist;
}

void solve1(std::vector<std::string> data) {
    std::map<char, std::map<char, int>> dists = build_graph(data);
    int dist = dijkstra<std::pair<char, int>>(dists, {'@', 0}, get_neighbors_single);
    std::cout << dist << std::endl;
}

void solve2(std::vector<std::string> data) {
    int width = data[0].size();
    int height = data.size();
    int start_r, start_c;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            if (data[r][c] == '@') {
                start_r = r;
                start_c = c;
                goto found;
            }
        }
    }
    found:
    char replacements[3][3] = {
        {'1', '#', '2'},
        {'#', '#', '#'},
        {'3', '#', '4'}
    };
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            data[start_r + dr][start_c + dc] = replacements[dr + 1][dc + 1];
        }
    }

    std::map<char, std::map<char, int>> dists = build_graph(data);
    int starts = ('1' << 0) | ('2' << 8) | ('3' << 16) | ('4' << 24);
    int dist = dijkstra<std::pair<int, int>>(
        dists, {starts, 0}, get_neighbors_multi
    );
    std::cout << dist << std::endl;
}
