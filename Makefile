RMFLAGS=-f
TARGET=lai
OBJS=argument.o help.o utils.o stats.o run.o snprintf.o main.o

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
endif


.PHONY: clean

debug: $(OBJS)
	$(CC) $(DEBUG) $(OBJ_TO_TARGET) $(OBJS) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)

release: $(OBJS)
	$(CC) $(OPTIMIZE) $(OBJ_TO_TARGET) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)

snprintf.o: snprintf.c
	$(CC) $(SRC_TO_OBJ) $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
ifeq ($(MAKECMDGOALS),rlease)
	$(CC) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) $(DEBUG) $(SRC_TO_OBJ) $(CFLAGS_INTERNAL) $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

clean:
	$(RM) $(OBJS) $(TARGET)
