# Makefile pour le projet système bancaire

CC= gcc
CFLAGS= -Wall -Wextra -std=c11

TARGET = compte_bancaire
SRC = compte_bancaire.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(TARGET)

format:
	clang-format -i $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "make        -> compile"
	@echo "make run    -> exécute"
	@echo "make clean  -> nettoie"
	@echo "make debug  -> compile avec debug"
