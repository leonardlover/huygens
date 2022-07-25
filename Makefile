CC := gcc
CFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
SRC := huygens.c
OBJ := $(SRC:.c=.o)
EXE := huygens

all: $(EXE) clean

$(EXE): $(OBJ)
	@$(CC) $^ -o $@ $(CFLAGS)

$(OBJ): $(SRC)
	@$(CC) -c $(SRC)

.PHONY: clean
clean:
	@rm $(OBJ)
	@./$(EXE)
	@rm $(EXE)
