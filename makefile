C = gcc
CFLAGS = -Wall -Wextra
SRC = src/main.c src/process.c src/globals.c
OBJ = $(SRC:.c=.o)
TARGET = proc_monitor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(C) -Wall -Wextra -o $(TARGET) $(OBJ) -lncurses

%.o: %.c
	$(C) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
