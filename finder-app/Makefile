


CC := $(CROSS_COMPILE)gcc


CFLAGS := -Wall -Werror -g


TARGET := writer


OBJS := writer.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c -o writer.o


clean:
	rm -f $(TARGET) $(OBJS)

