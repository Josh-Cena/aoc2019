CXX := g++
CXXFLAGS := -std=c++14 -O2

ARGS := $(wordlist 2,4,$(MAKECMDGOALS))
DAY := $(word 1,$(ARGS))
PROB := $(word 2,$(ARGS))
INPUT := $(word 3,$(ARGS))

OBJS := bin/main.o bin/day$(DAY).o
ifneq ($(filter 2 5,$(DAY)),)
	OBJS += bin/intcode.o
endif

run: bin/aoc2019
	./bin/aoc2019 $(DAY) $(PROB) $(INPUT)

bin/aoc2019: $(OBJS) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/main.o: src/main.cpp | bin
	$(CXX) $(CXXFLAGS) -c -o $@ $^

bin/day$(DAY).o: src/day$(DAY).cpp | bin
	$(CXX) $(CXXFLAGS) -c -o $@ $^

bin/intcode.o: src/intcode.cpp | bin
	$(CXX) $(CXXFLAGS) -c -o $@ $^

bin:
	mkdir -p bin

%:
	@:
