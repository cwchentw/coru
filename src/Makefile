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

INCLUDE_DIR=include
DIST_DIR=dist
TEST_DIR=test

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
	OBJS=cstring.obj language.obj hash_table.obj syntax_data.obj \
		coru_stats.obj \
		coru_token.obj coru_lexer.obj coru_ast.obj coru_parser.obj \
		coru_eval.obj coru.obj
	EXEC_OBJS=coru_argument.obj coru_command.obj coru_help.obj coru_cli.obj
	UNOBJS=cstring.obj language.obj hash_table.obj syntax_data.obj \
		uncoru_stats.obj uncoru_lexer.obj uncoru_eval.obj \
		uncoru.obj
	EXEC_UNOBJS=uncoru_argument.obj uncoru_command.obj uncoru_help.obj \
		uncoru_cli.obj
else
	OBJS=cstring.o language.o hash_table.o syntax_data.o \
		coru_stats.o \
		coru_token.o coru_lexer.o coru_ast.o coru_parser.o \
		coru_eval.o coru.o
	EXEC_OBJS=coru_command.o coru_argument.o coru_help.o coru_cli.o
	UNOBJS=cstring.o language.o hash_table.o syntax_data.o \
		uncoru_stats.o uncoru_lexer.o uncoru_eval.o \
		uncoru.o
	EXEC_UNOBJS=uncoru_command.o uncoru_help.o uncoru_argument.o \
		uncoru_cli.o
endif

COMPILER_GCC := cc gcc clang

ifneq (,$(filter $(COMPILER_GCC),$(CC)))
	CFLAGS=-Wall -Wextra -std=c89
	SRC_TO_OBJ=-c $<
	OBJ_TO_EXEC=-o ..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_EXEC)
	OBJ_TO_UNEXEC=-o ..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_EXEC)
else
ifeq ($(CC),cl)
	CFLAGS=/W4 /sdl
	SRC_TO_OBJ=/c $<
	OBJ_TO_EXEC=/Fe:..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_EXEC)
	OBJ_TO_UNEXEC=/Fe:..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_EXEC)
	LIBS=/link shlwapi.lib
else
	echo "Not supported yet"
endif
endif

ifneq (,$(DEBUG))
ifeq ($(CC),cl)
	CFLAGS+=/D DEBUG
else
	CFLAGS+=-DDEBUG -g
endif
else
ifeq ($(CC),cl)
	CFLAGS+=/O2
else
	CFLAGS+=-O2
endif
endif

ifeq ($(CC),cl)
	CFLAGS+=/I ..$(SEP)$(INCLUDE_DIR)
else
	CFLAGS+=-I ..$(SEP)$(INCLUDE_DIR)
endif


.PHONY: all test debug release static dynamic clean_objs clean

all:
	$(MAKE) dynamic
	$(MAKE) clean_objs
	$(MAKE) $(TARGET_EXEC)
	$(MAKE) $(UNTARGET_EXEC)

$(TARGET_EXEC): $(EXEC_OBJS) static
	$(CC) $(OBJ_TO_EXEC) $(EXEC_OBJS) \
		..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_STATIC) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)

$(UNTARGET_EXEC): $(EXEC_UNOBJS) static
	$(CC) $(OBJ_TO_UNEXEC) $(EXEC_UNOBJS) \
		..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_STATIC) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)

static: $(TARGET_LIB_STATIC) $(UNTARGET_LIB_STATIC)

$(TARGET_LIB_STATIC): $(OBJS)
ifeq ($(CC),cl)
	lib /out:..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_STATIC) $(OBJS)
else
ifeq ($(detected_OS),Darwin)
	libtool -static -o ..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_STATIC) $(OBJS)
else
ifneq ($(detected_OS),Linux)
	$(AR) rcs ..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_STATIC) $(OBJS)
else
	$(AR) rcs -o ..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_STATIC) $(OBJS)
endif  # Linux
endif  # Darwin
endif  # CC

$(UNTARGET_LIB_STATIC): $(UNOBJS)
ifeq ($(CC),cl)
	lib /out:..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_STATIC) $(UNOBJS)
else
ifeq ($(detected_OS),Darwin)
	libtool -static -o ..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_STATIC) $(UNOBJS)
else
ifneq ($(detected_OS),Linux)
	$(AR) rcs ..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_STATIC) $(UNOBJS)
else
	$(AR) rcs -o ..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_STATIC) $(UNOBJS)
endif  # Linux
endif  # Darwin
endif  # CC

dynamic: $(TARGET_LIB_DYNAMIC) $(UNTARGET_LIB_DYNAMIC)

$(TARGET_LIB_DYNAMIC): $(OBJS)
ifeq ($(CC),cl)
	link /DLL /OUT:..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_DYNAMIC) $(OBJS) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) -shared -o ..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_DYNAMIC) \
		$(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.obj: %.c
	$(CC) $(SRC_TO_OBJ) $(CFLAGS) $(LDFLAGS)

$(UNTARGET_LIB_DYNAMIC): $(UNOBJS)
ifeq ($(CC),cl)
	link /DLL /OUT..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_DYNAMIC) $(UNOBJS) \
		$(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) -shared -o ..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_DYNAMIC) \
		$(UNOBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

%.o: %.c
ifeq (dynamic,$(MAKECMDGOALS))
	$(CC) -fPIC $(SRC_TO_OBJ) $(CFLAGS) $(LDFLAGS)
else
	$(CC) $(SRC_TO_OBJ) $(CFLAGS) $(LDFLAGS)
endif  # dynamic

clean_objs:
	$(RM) $(OBJS)

clean:
	$(RM) $(OBJS) $(EXEC_OBJS) $(UNOBJS) $(EXEC_UNOBJS) \
		..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_EXEC) \
		..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_STATIC) \
		..$(SEP)$(DIST_DIR)$(SEP)$(TARGET_LIB_DYNAMIC) \
		..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_EXEC) \
		..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_STATIC) \
		..$(SEP)$(DIST_DIR)$(SEP)$(UNTARGET_LIB_DYNAMIC)
