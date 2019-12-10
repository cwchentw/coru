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
UNTARGET=uncoru

ifeq ($(detected_OS),Windows)
	TARGET_EXEC=$(TARGET).exe
	UNTARGET_EXEC=$(UNTARGET).exe
else
	TARGET_EXEC=$(TARGET)
	UNTARGET_EXEC=$(UNTARGET)
endif

SRC_TARGET=coru_cli.c
SRC_UNTARGET=uncoru_cli.c

ifeq ($(CC),cl)
	TARGET_LIB_DYNAMIC=$(TARGET).dll
	TARGET_LIB_STATIC=$(TARGET).lib
	UNTARGET_LIB_DYNAMIC=$(UNTARGET).dll
	UNTARGET_LIB_STATIC=$(UNTARGET).lib
else
ifeq ($(detected_OS),Darwin)
	TARGET_LIB_DYNAMIC=lib$(TARGET).dylib
	UNTARGET_LIB_DYNAMIC=lib$(UNTARGET).dylib
else
	TARGET_LIB_DYNAMIC=lib$(TARGET).so
	UNTARGET_LIB_DYNAMIC=lib$(UNTARGET).so
endif
	TARGET_LIB_STATIC=lib$(TARGET).a
	UNTARGET_LIB_STATIC=lib$(UNTARGET).a
endif

ifeq ($(CC),cl)
	OBJS=utils.obj language.obj hash_table.obj coru_argument.obj \
		coru_command.obj coru_help.obj coru_stats.obj coru.obj
	UNOBJS=uncoru_argument.obj uncoru.obj
else
	OBJS=utils.o language.o hash_table.o coru_argument.o \
		coru_command.o coru_help.o coru_stats.o coru.o
	UNOBJS=uncoru_argument.o uncoru.o
endif

COMPILER_GCC := cc gcc clang

ifneq (,$(filter $(COMPILER_GCC),$(CC)))
	CFLAGS_INTERNAL=-Wall -Wextra -std=c89
	OPTIMIZE=-O2
	SRC_TO_OBJ=-c $<
	OBJ_TO_EXEC=-o $(TARGET_EXEC)
	OBJ_TO_UNEXEC=-o $(UNTARGET_EXEC)
	DEBUG=-DDEBUG
	DEBUG_INFO=-g
else
ifeq ($(CC),cl)
	CFLAGS_INTERNAL=/W4
	OPTIMIZE=/O2
	SRC_TO_OBJ=/c $<
	OBJ_TO_EXEC=/Fe:$(TARGET_EXEC)
	OBJ_TO_UNEXEC=/Fe:$(UNTARGET_EXEC)
	DEBUG=/D DEBUG
else
	echo "Not supported yet"
endif
endif

GOAL_DEBUG := test debug


.PHONY: all test debug release static dynamic clean_objs clean

all:
	$(MAKE) dynamic
	$(MAKE) clean_objs
	$(MAKE) release

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

debug: $(TARGET_EXEC) $(UNTARGET_EXEC)

release: $(TARGET_EXEC) $(UNTARGET_EXEC)

$(TARGET_EXEC): $(SRC_TARGET) static
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(OBJ_TO_EXEC) $(SRC_TARGET) $(TARGET_LIB_STATIC) \
		$(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) $(LDFLAGS) $(LIBS)
else
	$(CC) $(OBJ_TO_EXEC) $(SRC_TARGET) $(TARGET_LIB_STATIC) $(OPTIMIZE) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)
endif

$(UNTARGET_EXEC): $(SRC_UNTARGET) static
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(OBJ_TO_UNEXEC) $(SRC_UNTARGET) $(UNTARGET_LIB_STATIC) \
		$(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) $(LDFLAGS) $(LIBS)
else
	$(CC) $(OBJ_TO_UNEXEC) $(SRC_UNTARGET) $(UNTARGET_LIB_STATIC) $(OPTIMIZE) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)
endif

static: $(TARGET_LIB_STATIC) $(UNTARGET_LIB_STATIC)

$(TARGET_LIB_STATIC): $(OBJS)
ifeq ($(CC),cl)
	echo "Not supported yet"
else
ifeq ($(detected_OS),Darwin)
	libtool -static -o $(TARGET_LIB_STATIC) $(OBJS)
else
ifneq ($(detected_OS),Linux)
	$(AR) rcs $(TARGET_LIB_STATIC) $(OBJS)
else
	$(AR) rcs -o $(TARGET_LIB_STATIC) $(OBJS)
endif  # Linux
endif  # Darwin
endif  # CC

$(UNTARGET_LIB_STATIC): $(UNOBJS)
ifeq ($(CC),cl)
	echo "Not supported yet"
else
ifeq ($(detected_OS),Darwin)
	libtool -static -o $(UNTARGET_LIB_STATIC) $(UNOBJS)
else
ifneq ($(detected_OS),Linux)
	$(AR) rcs $(UNTARGET_LIB_STATIC) $(UNOBJS)
else
	$(AR) rcs -o $(UNTARGET_LIB_STATIC) $(UNOBJS)
endif  # Linux
endif  # Darwin
endif  # CC

dynamic: $(TARGET_LIB_DYNAMIC)

$(TARGET_LIB_DYNAMIC): $(OBJS)
ifeq ($(CC),cl)
	echo "Not supported yet"
else
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(GCC) -shared -o $(TARGET_LIB_DYNAMIC) $(OBJS) $(DEBUG) \
		$(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) $(LDFLAGS) $(LIBS)
else
	$(CC) -shared -o $(TARGET_LIB_DYNAMIC) $(OBJS) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)
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
ifeq (dynamic,$(MAKECMDGOALS))
ifeq ($(CC),cl)
	echo "Not supported yet"
else
	$(CC) -fPIC $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS)
endif  # CC
else
ifeq ($(CC),cl)
	echo "Not supported yet"
else
ifneq (,$(filter $(GOAL_DEBUG),$(MAKECMDGOALS)))
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(DEBUG_INFO) \
		$(LDFLAGS)
else
	$(CC) $(SRC_TO_OBJ) $(OPTIMIZE) $(CFLAGS) $(LDFLAGS)
endif  # DEBUG
endif  # CC
endif  # dynamic

clean_objs:
	$(RM) $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET_EXEC) $(TARGET_LIB_STATIC) $(TARGET_LIB_DYNAMIC) \
		$(UNOBJS) $(UNTARGET_EXEC) $(UNTARGET_LIB_STATIC)
