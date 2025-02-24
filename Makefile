CXX = g++
CXXFLAGS = -O1 -std=c++20
LDFLAGS = 

BIGREAL_SRC = BigReal.cpp
PI_SRC = pi.cpp
TESTS_SRC = tests.cpp

PI_TARGET = pi_program
TESTS_TARGET = test_program

all: $(PI_TARGET) $(TESTS_TARGET)

$(PI_TARGET): $(PI_SRC) $(BIGREAL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TESTS_TARGET): $(TESTS_SRC) $(BIGREAL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TESTS_TARGET)
	./$(TESTS_TARGET)

pi: $(PI_TARGET)
	./$(PI_TARGET) $(precision)

clean:
	rm -f $(PI_TARGET) $(TESTS_TARGET)

rebuild: clean all

.PHONY: all test pi clean rebuild
