CFLAGS=-Wall -Wextra -std=c89
RMFLAGS=-f
TARGET=lai
OBJS=argument.o help.o utils.o stats.o run.o snprintf.o main.o


.PHONY: clean

debug: $(OBJS)
	$(CC) -DDEBUG -o $(TARGET) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)

release: $(OBJS)
	$(CC) -O2 -o $(TARGET) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
ifeq ($(MAKECMDGOALS),rlease)
	$(CC) -c $< $(CFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) -DDEBUG -c $< $(CFLAGS) $(LDFLAGS) $(LIBS)
endif

clean:
	$(RM) $(OBJS) $(TARGET)
