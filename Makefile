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
	TARGET=coru.exe
else
	TARGET=coru
endif

ifeq ($(CC),cl)
	OBJS=argument.obj help.obj language.obj utils.obj stats.obj hash_table.obj \
		run.obj coru.obj
else
	OBJS=argument.o help.o language.o utils.o stats.o hash_table.o run.o coru.o
endif

ifeq ($(CC),cc)
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_TARGET=-o $(TARGET)
	DEBUG=-DDEBUG
	DEBUG_INFO=-g
else ifeq ($(CC),gcc)
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_TARGET=-o $(TARGET)
	DEBUG=-DDEBUG
	DEBUG_INFO=-g
else ifeq ($(CC),clang)
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_TARGET=-o $(TARGET)
	DEBUG=-DDEBUG
	DEBUG_INFO=-g
else ifeq ($(CC),cl)
	CFLAGS_INTERNAL=/W4
	OPTIMIZE=/O2
	SRC_TO_OBJ=/c $<
	OBJ_TO_TARGET=/Fe:$(TARGET)
	DEBUG=/D DEBUG
endif

GOAL_DEBUG := test debug


.PHONY: all test debug release clean

all: release

test: debug $(TARGET)
ifeq ($(detected_OS),Windows)
	echo "Not supported yet"
else
ifeq ($(detected_OS),SunOS)
	bash tests/detectFiles
else
	tests/detectFiles
endif
endif

debug: $(TARGET)

release: $(TARGET)

$(TARGET): $(OBJS)
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(OBJ_TO_TARGET) $(OBJS) \
		$(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) $(LDFLAGS) $(LIBS)
else
	$(CC) $(OBJ_TO_TARGET) $(OBJS) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.obj: %.c
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) \
		$(LDFLAGS) $(LIBS)
else
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.o: %.c
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) \
		$(LDFLAGS) $(LIBS)
else
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

clean:
	$(RM) $(OBJS) $(TARGET)
