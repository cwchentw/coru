# Detect system OS.
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

ifeq ($(detected_OS),Windows)
	SEP=\\
else
	SEP=/
endif

SRC_DIR=src
TEST_DIR=test

.PHONY: all test dynamic static clean

all:
	$(MAKE) -C .$(SEP)$(SRC_DIR)

test:
ifeq ($(detected_OS),Windows)
	echo "Not supported yet"
else
ifeq ($(detected_OS),SunOS)
	$(MAKE) -C .$(SEP)$(SRC_DIR) DEBUG=1
	bash ./$(TEST_DIR)/coru/showHelp
	bash ./$(TEST_DIR)/coru/detectFiles
	bash ./$(TEST_DIR)/coru/runProgram
	bash ./$(TEST_DIR)/uncoru/showHelp
	$(MAKE) clean

	$(MAKE) -C .$(SEP)$(SRC_DIR)
	bash $(TEST_DIR)/coru/checkModifiedFiles
	$(MAKE) clean
else
	$(MAKE) -C .$(SEP)$(SRC_DIR) DEBUG=1
	./$(TEST_DIR)/coru/showHelp
	./$(TEST_DIR)/coru/detectFiles
	./$(TEST_DIR)/coru/runProgram
	./$(TEST_DIR)/uncoru/showHelp
	$(MAKE) clean

	$(MAKE) -C .$(SEP)$(SRC_DIR)
	$(TEST_DIR)/coru/checkModifiedFiles
	$(MAKE) clean
endif
endif

dynamic:
	$(MAKE) -C .$(SEP)$(SRC_DIR) dynamic

static:
	$(MAKE) -C .$(SEP)$(SRC_DIR) static

clean:
	$(MAKE) -C .$(SEP)$(SRC_DIR) clean
