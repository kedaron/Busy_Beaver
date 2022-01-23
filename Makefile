PROJECT := Busy_Beaver
CXX := g++
CXXFLAGS := -fopenmp -Wall --std=c++17 -O3
BIN_DIR := ./bin
BUILD_DIR := ./build
SRC_DIR := ./src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

$(BIN_DIR)/$(PROJECT): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm $(BUILD_DIR)/*.o $(BIN_DIR)/$(PROJECT)