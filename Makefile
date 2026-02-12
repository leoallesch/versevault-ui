BUILD_DIR := build

.PHONY: all configure build run generate clean

all: build

configure:
	cmake -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR) -j$$(nproc)

run: build
	./$(BUILD_DIR)/versevault-simulator

generate:
	cmake --build $(BUILD_DIR) --target ui-generate

clean:
	rm -rf $(BUILD_DIR)