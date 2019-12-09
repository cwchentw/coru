# Detect system OS.
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

ifeq ($(detected_OS),Windows)
	RM=del
endif

TARGET=coru

ifeq ($(detected_OS),Windows)
	TARGET_EXEC=$(TARGET).exe
else
	TARGET_EXEC=$(TARGET)
endif

SRC_TARGET=coru_cli.c

ifeq ($(CC),cl)
	TARGET_LIB_DYNAMIC=$(TARGET).dll
	TARGET_LIB_STATIC=$(TARGET).lib
else
ifeq ($(detected_OS),Darwin)
	TARGET_LIB_DYNAMIC=lib$(TARGET).dylib
else
	TARGET_LIB_DYNAMIC=lib$(TARGET).so
endif
	TARGET_LIB_STATIC=lib$(TARGET).a
endif

ifeq ($(CC),cl)
	OBJS=utils.obj language.obj hash_table.obj coru_argument.obj \
		coru_command.obj coru_help.obj coru_stats.obj coru.obj
else
	OBJS=utils.o language.o hash_table.o coru_argument.o \
		coru_command.o coru_help.o coru_stats.o coru.o
endif

COMPILER_GCC := cc gcc clang

ifneq (,$(filter $(COMPILER_GCC),$(CC)))
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_EXEC=-o $(TARGET_EXEC)
	DEBUG=-DDEBUG
	DEBUG_INFO=-g
else ifeq ($(CC),cl)
	CFLAGS_INTERNAL=/W4
	OPTIMIZE=/O2
	SRC_TO_OBJ=/c $<
	OBJ_TO_EXEC=/Fe:$(TARGET_EXEC)
	DEBUG=/D DEBUG
endif

GOAL_DEBUG := test debug


.PHONY: all test debug release static clean

all: release

test: debug $(TARGET_EXEC)
ifeq ($(detected_OS),Windows)
	echo "Not supported yet"
else
ifeq ($(detected_OS),SunOS)
	bash tests/detectFiles
else
	tests/detectFiles
endif
endif

debug: $(TARGET_EXEC)

release: $(TARGET_EXEC)

$(TARGET_EXEC): $(SRC_TARGET) static
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(OBJ_TO_EXEC) $(SRC_TARGET) $(TARGET_LIB_STATIC) \
		$(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) $(LDFLAGS) $(LIBS)
else
	$(CC) $(OBJ_TO_EXEC) $(SRC_TARGET) $(TARGET_LIB_STATIC) $(OPTIMIZE) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)
endif

static: $(TARGET_LIB_STATIC)

$(TARGET_LIB_STATIC): $(OBJS)
ifeq ($(CC),cl)
	echo "Not supported yet"
else
ifeq ($(detected_OS),Darwin)
	libtool -static -o $(TARGET_LIB_STATIC) $(OBJS)
else
	$(AR) rcs -o $(TARGET_LIB_STATIC) $(OBJS)
endif
endif

%.obj: %.c
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) \
		$(LDFLAGS)
else
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS)
endif

%.o: %.c
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) \
		$(LDFLAGS)
else
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS)
endif

clean:
	$(RM) $(OBJS) $(TARGET_EXEC) $(TARGET_LIB_STATIC)
