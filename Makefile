CC = gcc
CFLAGS = -Wall
LIBS = -lSDL2 -lSDL2main -lm -lSDL2_ttf -lSDL2_image
TARGET = Gob
SRCS = Gob.c functions.c board.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

$(OBJS): $(SRCS)
	$(CC) $(SRCS) -c
clean:
	rm *.o
