# Makefile pour le projet système bancaire

CC= gcc
CFLAGS= -Wall -Wextra -std=c11

TARGET = compte_bancaire
SRC = compte_bancaire.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
