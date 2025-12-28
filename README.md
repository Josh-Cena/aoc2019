# Advent of Code 2019

Language: ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) (C++14)
Package Manager: [CMake](https://cmake.org/)

This repo uses my standard AoC setup. Inputs are stored as `inputs/day{n}/{name}.txt`. By default `name` is `real` (the real question). To run a specific day's solution, use the following command:

```bash
make run {day} {part} {name}
```

For example, to run the solution for day 1, part 2 with the example input:

```bash
make run 1 2 ex
```

(And make sure that `inputs/day1/ex.txt` exists.)
