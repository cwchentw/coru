CFLAGS=-O2 -Wall -Wextra -std=c89
RMFLAGS=-f
TARGET=lai
OBJS=argument.o help.o utils.o main.o


.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS) $(TARGET)
