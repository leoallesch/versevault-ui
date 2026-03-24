ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR := $(ROOT_DIR)/build/simulator
ESP_BUILD_DIR := $(ROOT_DIR)/build/esp32
PORT ?= /dev/ttyACM0
IDF_PATH ?= /home/leo/dev/lib/esp/esp-idf
IDF_PY := idf.py

.PHONY: all configure build run generate clean
.PHONY: esp32-build esp32-flash esp32-monitor esp32-clean

all: build

# ── Simulator ─────────────────────────────────────────────────────────────────

configure:
	cmake -DCMAKE_BUILD_TYPE=Debug -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR) -j$$(nproc)

run: build
	./build/simulator/src/platform/simulator/alarmclock-simulator

generate:
	node scripts/lved-cli.js generate src/app/ui

clean:
	rm -rf $(BUILD_DIR)

# ── ESP32-S3 ──────────────────────────────────────────────────────────────────

esp32-build:
	IDF_PATH=$(IDF_PATH) $(IDF_PY) --build-dir $(ESP_BUILD_DIR) build

esp32-flash:
	IDF_PATH=$(IDF_PATH) $(IDF_PY) --build-dir $(ESP_BUILD_DIR) -p $(PORT) flash

esp32-monitor:
	IDF_PATH=$(IDF_PATH) $(IDF_PY) --build-dir $(ESP_BUILD_DIR) -p $(PORT) monitor

esp32-clean:
	rm -rf $(ESP_BUILD_DIR)
