CXX := g++
CXXFLAGS := -std=c++14 -O2

ARGS := $(wordlist 2,4,$(MAKECMDGOALS))
DAY := $(word 1,$(ARGS))
PROB := $(word 2,$(ARGS))
INPUT := $(word 3,$(ARGS))

run: bin/aoc2019
	./bin/aoc2019 $(DAY) $(PROB) $(INPUT)

bin/aoc2019: src/main.cpp src/day$(DAY).cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) src/main.cpp src/day$(DAY).cpp -o bin/aoc2019

%:
	@:
