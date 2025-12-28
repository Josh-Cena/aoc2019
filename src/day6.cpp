#include <iostream>
#include <unordered_map>
#include <queue>

void solve1(std::vector<std::string> data) {
    std::unordered_map<std::string, std::vector<std::string>> children;
    for (auto line : data) {
        auto delimiter_pos = line.find(")");
        std::string parent = line.substr(0, delimiter_pos);
        std::string child = line.substr(delimiter_pos + 1);
        children[parent].push_back(child);
    }
    int total_orbits = 0;
    std::queue<std::pair<std::string, int>> to_visit;
    to_visit.push({"COM", 0});
    while (!to_visit.empty()) {
        auto current = to_visit.front();
        to_visit.pop();
        int depth = current.second;
        total_orbits += depth;
        for (auto child : children[current.first]) {
            to_visit.push({child, depth + 1});
        }
    }
    std::cout << total_orbits << std::endl;
}

void solve2(std::vector<std::string> data) {
    std::unordered_map<std::string, std::vector<std::string>> connected;
    for (auto line : data) {
        auto delimiter_pos = line.find(")");
        std::string parent = line.substr(0, delimiter_pos);
        std::string child = line.substr(delimiter_pos + 1);
        connected[parent].push_back(child);
        connected[child].push_back(parent);
    }
    std::queue<std::pair<std::string, int>> to_visit;
    std::unordered_map<std::string, bool> visited;
    to_visit.push({"YOU", 0});
    visited["YOU"] = true;
    int dist = 0;
    while (!to_visit.empty()) {
        auto current = to_visit.front();
        to_visit.pop();
        std::string node = current.first;
        int depth = current.second;
        if (node == "SAN") {
            dist = depth - 2; // Not count start/end nodes
            break;
        }
        for (auto neighbor : connected[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                to_visit.push({neighbor, depth + 1});
            }
        }
    }
    std::cout << dist << std::endl;
}
