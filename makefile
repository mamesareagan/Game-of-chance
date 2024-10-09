# Variables
CC = gcc                # The compiler
CFLAGS = -Wall -g -Werror        # Flags for the compiler: enable all warnings and debugging
OBJ = main.o error.o   # Updated object files list
TARGET = myprogram       # Final executable

# Default rule: this will be executed when you type 'make'
all: $(TARGET)

# Rule for building the final executable
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

# Rule for compiling main.c into an object file
main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

# Rule for compiling error.c into an object file
error.o: error.c game.h
	$(CC) $(CFLAGS) -c error.c

# Rule for compiling file1.c into an object file


# Rule for compiling file2.c into an object file
#
# Clean rule to remove object files and the final executable
clean:
	rm -f $(OBJ) $(TARGET)

