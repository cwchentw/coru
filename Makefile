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

.PHONY: all exec debug san san-debug test dynamic static clean

all: exec

exec:
	$(MAKE) -C .$(SEP)$(SRC_DIR) \
		CC="$(CC)" \
		DEBUG="$(DEBUG)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"

debug:
	$(MAKE) -C .$(SEP)$(SRC_DIR) clean
	$(MAKE) -C .$(SEP)$(SRC_DIR) \
		DEBUG=1 \
		CC="$(CC)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"

san:
	$(MAKE) -C .$(SEP)$(SRC_DIR) clean
	$(MAKE) -C .$(SEP)$(SRC_DIR) \
		CC=clang \
		EXTRA_CFLAGS="-g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer" \
		EXTRA_LDFLAGS="-fsanitize=address,undefined"

san-debug:
	$(MAKE) -C .$(SEP)$(SRC_DIR) clean
	$(MAKE) -C .$(SEP)$(SRC_DIR) \
		DEBUG=1 \
		CC=clang \
		EXTRA_CFLAGS="-g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer" \
		EXTRA_LDFLAGS="-fsanitize=address,undefined"

test:
ifeq ($(detected_OS),Windows)
	echo "Not supported yet"
else
ifeq ($(detected_OS),SunOS)
	$(MAKE) -C .$(SEP)$(SRC_DIR) DEBUG=1 \
		CC="$(CC)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
	bash ./$(TEST_DIR)/coru/showHelp
	bash ./$(TEST_DIR)/coru/detectFiles
	bash ./$(TEST_DIR)/coru/runProgram
	bash ./$(TEST_DIR)/uncoru/showHelp
	$(MAKE) clean

	$(MAKE) -C .$(SEP)$(SRC_DIR) \
		CC="$(CC)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
	bash $(TEST_DIR)/coru/checkModifiedFiles
	$(MAKE) clean
else
	$(MAKE) -C .$(SEP)$(SRC_DIR) DEBUG=1 \
		CC="$(CC)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
	./$(TEST_DIR)/coru/showHelp
	./$(TEST_DIR)/coru/detectFiles
	./$(TEST_DIR)/coru/runProgram
	./$(TEST_DIR)/uncoru/showHelp
	$(MAKE) clean

	$(MAKE) -C .$(SEP)$(SRC_DIR) \
		CC="$(CC)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"
	$(TEST_DIR)/coru/checkModifiedFiles
	$(MAKE) clean
endif
endif

dynamic:
	$(MAKE) -C .$(SEP)$(SRC_DIR) dynamic \
		CC="$(CC)" \
		DEBUG="$(DEBUG)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"

static:
	$(MAKE) -C .$(SEP)$(SRC_DIR) static \
		CC="$(CC)" \
		DEBUG="$(DEBUG)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"

clean:
	$(MAKE) -C .$(SEP)$(SRC_DIR) clean