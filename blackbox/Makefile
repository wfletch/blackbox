ASSEMBLY:= src
BUILD_DIR:= bin

COMPILER_FLAGS:= -std=c2x 
INCLUDE_FLAGS:= -Isrc -I$(VULKAN_SDK)/include -I$(VULKAN_SDK)/lib
LINKER_FLAGS:= -lvulkan
SOURCE_FILES:= $(shell find $(ASSEMBLY) -type f \( -name "*.c" -o -name "*.m" \))

.PHONY: build
build:
	@clang $(COMPILER_FLAGS) $(SOURCE_FILES) -o $(BUILD_DIR)/app $(INCLUDE_FLAGS) $(LINKER_FLAGS)

.PHONY: run
run:
	./bin/app