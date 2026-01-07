CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = source/main.c source/projet.c source/algo.c source/my_memoire.c
OBJ = $(SRC:.c=.o)

TARGET = projet

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	-cmd /C del /Q $(subst /,\,$(OBJ)) $(TARGET).exe 2>NUL
	-cmd /C del /Q $(TARGET) 2>NUL