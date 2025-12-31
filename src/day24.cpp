#include <iostream>
#include <set>

typedef std::pair<int, int> CellGrid;
typedef std::tuple<int, int, int> CellRec; // (row, col, level)

std::vector<CellGrid> neighbors_grid(CellGrid cell) {
    auto [r, c] = cell;
    std::vector<CellGrid> res;
    if (r > 0) res.emplace_back(r - 1, c);
    if (r < 4) res.emplace_back(r + 1, c);
    if (c > 0) res.emplace_back(r, c - 1);
    if (c < 4) res.emplace_back(r, c + 1);
    return res;
}

/*
     |     |         |     |     
  1  |  2  |    3    |  4  |  5  
     |     |         |     |     
-----+-----+---------+-----+-----
     |     |         |     |     
  6  |  7  |    8    |  9  |  10 
     |     |         |     |     
-----+-----+---------+-----+-----
     |     |A|B|C|D|E|     |     
     |     |-+-+-+-+-|     |     
     |     |F|G|H|I|J|     |     
     |     |-+-+-+-+-|     |     
 11  | 12  |K|L|?|N|O|  14 |  15 
     |     |-+-+-+-+-|     |     
     |     |P|Q|R|S|T|     |     
     |     |-+-+-+-+-|     |     
     |     |U|V|W|X|Y|     |     
-----+-----+---------+-----+-----
     |     |         |     |     
 16  | 17  |    18   |  19 |  20 
     |     |         |     |     
-----+-----+---------+-----+-----
     |     |         |     |     
 21  | 22  |    23   |  24 |  25 
     |     |         |     |     
*/
std::vector<CellRec> neighbors_recursive(CellRec cell) {
    auto [r, c, level] = cell;
    std::vector<CellRec> res;
    // Up
    if (r == 0) {
        // A,B,C,D,E -> 8
        res.emplace_back(1, 2, level - 1);
    } else if (r == 3 && c == 2) {
        // 18 -> U,V,W,X,Y
        for (int subc = 0; subc < 5; subc++)
            res.emplace_back(4, subc, level + 1);
    } else {
        res.emplace_back(r - 1, c, level);
    }
    // Down
    if (r == 4) {
        // U,V,W,X,Y -> 18
        res.emplace_back(3, 2, level - 1);
    } else if (r == 1 && c == 2) {
        // 8 -> A,B,C,D,E
        for (int subc = 0; subc < 5; subc++)
            res.emplace_back(0, subc, level + 1);
    } else {
        res.emplace_back(r + 1, c, level);
    }
    // Left
    if (c == 0) {
        // A,F,K,P,U -> 12
        res.emplace_back(2, 1, level - 1);
    } else if (r == 2 && c == 3) {
        // 14 -> E,J,O,T,Y
        for (int subr = 0; subr < 5; subr++)
            res.emplace_back(subr, 4, level + 1);
    } else {
        res.emplace_back(r, c - 1, level);
    }
    // Right
    if (c == 4) {
        // E,J,O,T,Y -> 14
        res.emplace_back(2, 3, level - 1);
    } else if (r == 2 && c == 1) {
        // 12 -> A,F,K,P,U
        for (int subr = 0; subr < 5; subr++)
            res.emplace_back(subr, 0, level + 1);
    } else {
        res.emplace_back(r, c + 1, level);
    }
    return res;
}

template<typename T>
std::set<T> evolve(
    const std::set<T>& bugs, 
    std::vector<T> (*get_neighbors)(T)
) {
    std::set<T> new_bugs;
    std::set<T> to_check;
    for (auto &bug : bugs) {
        to_check.insert(bug);
        for (auto &neighbor : get_neighbors(bug))
            to_check.insert(neighbor);
    }
    for (auto &cell : to_check) {
        int alive_nbrs = 0;
        for (auto &neighbor : get_neighbors(cell)) {
            if (bugs.count(neighbor)) alive_nbrs++;
        }
        if (!bugs.count(cell) && alive_nbrs == 2 || alive_nbrs == 1) {
            new_bugs.insert(cell);
        }
    }
    return new_bugs;
}

void solve1(std::vector<std::string> data) {
    std::set<CellGrid> bugs;
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (data[r][c] == '#') bugs.insert({r, c});
    std::set<unsigned long long> seen;
    while (true) {
        unsigned long long rating = 0;
        unsigned long long power = 1;
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                if (bugs.count({r, c})) {
                    rating |= power;
                }
                power <<= 1;
            }
        }
        if (seen.count(rating)) {
            std::cout << rating << std::endl;
            return;
        }
        seen.insert(rating);
        bugs = evolve<CellGrid>(bugs, neighbors_grid);
    }
}

void solve2(std::vector<std::string> data) {
    std::set<CellRec> bugs;
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if (data[r][c] == '#')
                bugs.insert({r, c, 0});
    for (int t = 0; t < 200; t++)
        bugs = evolve<CellRec>(bugs, neighbors_recursive);
    std::cout << bugs.size() << std::endl;
}
