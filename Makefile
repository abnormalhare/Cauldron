CC = g++
CFLAGS = -g -std=c++17 -Iinclude -m64 -Wall

# Directories
SRC_DIR = src
BUILD_DIR = $(SRC_DIR)/build
OBJ_DIR = build

# Files
C_FILES = $(wildcard $(BUILD_DIR)/*.cpp $(SRC_DIR)/*.cpp)
OBJ_FILES = $(C_FILES:.cpp=.o)

# Build
OBJ = $(OBJ_DIR)/$(notdir $@)
E_OBJ = $(foreach obj,$^,$(OBJ_DIR)/$(notdir $(obj)))

# Output executable
EXEC = Cauldron

# Rule to build the executable
$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $(E_OBJ) $(CFLAGS)

# Rule to compile each .cpp file into a .o file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $(OBJ)

# Clean rule to remove compiled files
clean:
	rm -f $(OBJ_FILES) $(EXEC)

.PHONY: clean