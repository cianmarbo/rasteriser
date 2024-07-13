COMPILER=gcc

BUILD_DIR=build
SRC_DIR=src

# Source files
SOURCES=$(SRC_DIR)/rasteriser.c $(SRC_DIR)/display.c $(SRC_DIR)/image.c $(SRC_DIR)/vec.c $(SRC_DIR)/mesh.c

# Object files
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target for the executable
$(BUILD_DIR)/rasteriser: $(OBJECTS)
	$(COMPILER) -lm -lSDL2 -Wall -std=c99 -o $@ $^

# Rule to compile each source file to an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(COMPILER) -c -Wall -std=c99 -o $@ $<

# Phony target to force rebuild
.PHONY: build
build:
	$(MAKE) $(BUILD_DIR)/rasteriser

# Phony target for cleaning up build files
.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/rasteriser
