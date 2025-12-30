#include <iostream>
#include <sstream>
#include <map>
#include <numeric>

struct Recipe {
    std::map<std::string, long long> ingredients;
    long long output;
};

long long num_ores(
    const std::map<std::string, Recipe> &recipes,
    const std::string &name,
    long long quantity,
    std::map<std::string, long long> &leftovers
) {
    if (name == "ORE") return quantity;
    auto it = leftovers.find(name);
    if (it != leftovers.end()) {
        if (it->second > quantity) {
            it->second -= quantity;
            return 0;
        } else {
            quantity -= it->second;
            leftovers.erase(it);
        }
    }
    if (quantity == 0) return 0;
    const Recipe &recipe = recipes.at(name);
    long long times = (quantity + recipe.output - 1) / recipe.output;
    long long total_ingredients = 0;
    for (const auto &[subname, subquant] : recipe.ingredients) {
        total_ingredients += num_ores(recipes, subname, subquant * times, leftovers);
    }
    long long leftover = times * recipe.output - quantity;
    if (leftover > 0) leftovers[name] += leftover;
    return total_ingredients;
}

std::map<std::string, Recipe> parse_recipes(const std::vector<std::string> &data) {
    std::map<std::string, Recipe> recipes;
    for (const auto &line : data) {
        Recipe recipe;
        std::istringstream ss(line);
        std::string part;
        while (std::getline(ss, part, ' ')) {
            if (part == "=>") {
                std::getline(ss, part, ' ');
                recipe.output = std::stoi(part);
                std::getline(ss, part, ' ');
                recipes.insert({part, recipe});
                break;
            }
            int num = std::stoi(part);
            std::getline(ss, part, ' ');
            if (part.back() == ',') {
                part.pop_back();
            }
            recipe.ingredients.insert({part, num});
        }
    }
    return recipes;
}

void solve1(std::vector<std::string> data) {
    auto recipes = parse_recipes(data);
    std::map<std::string, long long> leftovers;
    long long ores = num_ores(recipes, "FUEL", 1, leftovers);
    std::cout << ores << std::endl;
}

void solve2(std::vector<std::string> data) {
    auto recipes = parse_recipes(data);
    long long ore_cap = 1000000000000LL;
    long long left = 1;
    long long right = ore_cap;
    while (left <= right) {
        long long mid = (left + right) / 2;
        std::map<std::string, long long> leftovers;
        long long ores = num_ores(recipes, "FUEL", mid, leftovers);
        if (ores <= ore_cap) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    std::cout << right << std::endl;
}
