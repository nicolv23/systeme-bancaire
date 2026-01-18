# Makefile pour le projet système bancaire

CC= gcc
CFLAGS= -Wall -Wextra -Werror -std=c11

TARGET = SecureBank/compte_bancaire
SRC = SecureBank/compte_bancaire.c
OBJ = $(SRC:.c=.o)

.PHONY: all debug clean run format help

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(TARGET)

format:
	clang-format -i $(SRC)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Commandes disponibles :"
	@echo "make        -> compile"
	@echo "make run    -> exécute"
	@echo "make clean  -> nettoie"
	@echo "make debug  -> compile avec debug"
	@echo "make format -> formate le code"
