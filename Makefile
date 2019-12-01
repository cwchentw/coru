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
	OBJS=argument.o help.o utils.o stats.o run.o main.o
else
	OBJS=argument.o help.o utils.o stats.o run.o snprintf.o main.o
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
	SRC_TO_OBJ=
	OBJ_TO_TARGET=/Fe $(TARGET)
	DEBUG=/D DEBUG
endif


.PHONY: debug release clean

debug: $(TARGET)

release: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(MAKECMDGOALS),release)
	$(CC) $(OBJ_TO_TARGET) $(OBJS) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(DEBUG) $(OBJ_TO_TARGET) $(OBJS) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

snprintf.o: snprintf.c
ifeq ($(MAKECMDGOALS),release)
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(SRC_TO_OBJ) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.o: %.c
ifeq ($(MAKECMDGOALS),release)
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

clean:
	$(RM) $(OBJS) $(TARGET)
