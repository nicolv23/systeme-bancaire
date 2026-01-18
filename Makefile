# Makefile pour le projet système bancaire

CC= gcc
CFLAGS= -Wall -Wextra -Werror -std=c11

DIR = SecureBank
SRC = $(DIR)/src/compte_bancaire.c
OBJ = $(DIR)/build/compte_bancaire.o
TARGET = $(DIR)/bin/compte_bancaire

.PHONY: all debug clean run format help

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(DIR)/build/%.o: $(DIR)/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(TARGET)

format:
	clang-format -i $(SRC)

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Commandes disponibles :"
	@echo "make        -> compile"
	@echo "make run    -> exécute"
	@echo "make clean  -> nettoie"
	@echo "make debug  -> compile avec debug"
	@echo "make format -> formate le code"
