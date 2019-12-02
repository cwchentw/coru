# Detect system OS.
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

ifeq ($(detected_OS),Windows)
	RM=del
endif

ifeq ($(detected_OS),Windows)
	TARGET=lai.exe
else
	TARGET=lai
endif

ifeq ($(CC),cl)
	OBJS=argument.obj help.obj language.obj utils.obj stats.obj run.obj main.obj
else ifeq ($(detected_OS),Darwin)
	OBJS=argument.o help.o language.o utils.o stats.o run.o main.o
else
	OBJS=argument.o help.o language.o utils.o stats.o run.o snprintf.o main.o
endif

ifeq ($(CC),cc)
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_TARGET=-o $(TARGET)
	DEBUG=-DDEBUG
else ifeq ($(CC),gcc)
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_TARGET=-o $(TARGET)
	DEBUG=-DDEBUG
else ifeq ($(CC),clang)
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_TARGET=-o $(TARGET)
	DEBUG=-DDEBUG
else ifeq ($(CC),cl)
	CFLAGS_INTERNAL=/W4
	OPTIMIZE=/O2
	SRC_TO_OBJ=/c $<
	OBJ_TO_TARGET=/Fe:$(TARGET)
	DEBUG=/D DEBUG
endif


.PHONY: all test debug release clean

all: debug

test: debug $(TARGET)
	if ! command -v bats 2>/dev/null 1>&2; then \
		echo "No bats on your system" >&2; \
		exit 1; \
	fi
	tests/detectFiles

debug: $(TARGET)

release: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(MAKECMDGOALS),release)
	$(CC) $(OBJ_TO_TARGET) $(OBJS) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(DEBUG) $(OBJ_TO_TARGET) $(OBJS) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.obj: %.c
ifeq ($(MAKECMDGOALS),release)
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.o: %.c
ifeq ($(MAKECMDGOALS),release)
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

clean:
	$(RM) $(OBJS) $(TARGET)
