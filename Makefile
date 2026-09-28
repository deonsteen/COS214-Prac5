CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Werror -Iinclude

SRC_DIR = src
TEST_DIR = tests

SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(SRCS:.cpp=.o)
LIB = libcampusguard.a

all: campusguard

campusguard: $(SRC_DIR)/main.o $(LIB)
	$(CXX) $(CXXFLAGS) -o campusguard $(SRC_DIR)/main.o -L. -lcampusguard

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test-b: $(TEST_DIR)/test_b.o $(TEST_DIR)/fake_incident.o src/Commands.o src/OperatorConsole.o src/AccessControlSystem.o src/LegacyPAAdapter.o src/LegacyCampusPASystem.o
	$(CXX) $(CXXFLAGS) -o test-b $^

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(LIB) campusguard test-a test-b test-c

.PHONY: all clean test-a test-b test-c
