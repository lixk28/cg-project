CXX := g++
CXXFLAGS := -g -Wall -O

INC_DIR := ./include
SRC_DIR := ./src
OBJ_DIR := ./build

INC := $(wildcard $(INC_DIR)/*.h)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TARGET := ./demo

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $(TARGET) -lglfw -lGLEW -lGL -lGLU -lm -lassimp

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

.PHONY: test
test:
	$(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)